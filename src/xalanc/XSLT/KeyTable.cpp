/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
// Class header file
#include "KeyTable.hpp"



#include <memory>



#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XPath.hpp>



#include "KeyDeclaration.hpp"
#include "StylesheetExecutionContext.hpp"
#include "XSLTProcessorException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const MutableNodeRefList	KeyTable::s_dummyList;



KeyTable::KeyTable(
			XalanNode*							startNode,
			const PrefixResolver&				resolver,
			const KeyDeclarationVectorType&		keyDeclarations,
			StylesheetExecutionContext&			executionContext) :
	m_keys()
{
    XalanNode*	pos = startNode;

	const KeyDeclarationVectorType::size_type	nDeclarations =
			keyDeclarations.size();

    // Do a non-recursive pre-walk over the tree.
    while(0 != pos)
    {
		// We're going to have to walk the attribute list 
		// if it's an element, so get the attributes.
		const XalanNamedNodeMap*	attrs = 0;

		int							nNodes = 0;

		if(XalanNode::ELEMENT_NODE == pos->getNodeType())
		{
			attrs = pos->getAttributes();

			nNodes = attrs->getLength();
        
			if(0 == nNodes)
			{
				attrs = 0;
			}
		}

		// Walk the primary node, and each of the attributes.
		// This loop is a little strange... it is meant to always 
		// execute once, then execute for each of the attributes.
		XalanNode*	testNode = pos;

		for(int nodeIndex = -1; nodeIndex < nNodes;)
		{
			// Walk through each of the declarations made with xsl:key
			for(unsigned int i = 0; i < nDeclarations; ++i)
			{
				const KeyDeclaration&	kd = keyDeclarations[i];

				if (executionContext.getInConstruction(kd) == true)			
				{
					assert(kd.getURI() != 0);

					throw XSLTProcessorException(
							XalanMessageLoader::getMessage(XalanMessages::UseOfFunctionIsIllegal_2Param,"key()","xsl:key"),
							*kd.getURI(),
							kd.getLineNumber(),
							kd.getColumnNumber(),
							TranscodeFromLocalCodePage("XSLTKeyIllegalKeyFunctionException"));
				}
				else
				{
					executionContext.beginConstruction(kd);

					// See if our node matches the given key declaration according to 
					// the match attribute on xsl:key.
					assert(kd.getMatchPattern() != 0);

					const XPath::eMatchScore	score =
							kd.getMatchPattern()->getMatchScore(testNode,
																resolver,
																executionContext);

					if(score != XPath::eMatchScoreNone)
					{
						processKeyDeclaration(
							m_keys,
							kd,
							testNode,
							resolver,
							executionContext);
					}

					executionContext.endConstruction(kd);
				} // if (kd.getInConstruction() == true)
			} // end for(int i = 0; i < nDeclarations; ++i)

			++nodeIndex;

			if(0 != attrs)
			{
				testNode = attrs->item(nodeIndex);
			}
		} // for(int nodeIndex = -1; nodeIndex < nNodes;)

		// The rest of this is getting the next pre-walk position in 
		// the tree.
		XalanNode*	nextNode = pos->getFirstChild();

		while(0 == nextNode)
		{
			if(startNode == pos)
			{
				break;
			}
			else
			{
				nextNode = pos->getNextSibling();

				if(0 == nextNode)
				{
					pos = pos->getParentNode();

					if((startNode == pos) || (0 == pos))
					{
						nextNode = 0;
						break;
					}
				}
			}
		}

		pos = nextNode;
    } // while(0 != pos)

	if (m_keys.empty() == false)
	{
		const KeysMapType::iterator		theEnd = m_keys.end();
		KeysMapType::iterator			theCurrent = m_keys.begin();
		assert(theCurrent != theEnd);

		do
		{
			NodeListMapType&	theCurrentNodeListMap = (*theCurrent).second;

			if (theCurrentNodeListMap.empty() == false)
			{
				const NodeListMapType::iterator		theEnd = theCurrentNodeListMap.end();
				NodeListMapType::iterator			theCurrent = theCurrentNodeListMap.begin();
				assert(theCurrent != theEnd);

				do
				{
					(*theCurrent).second.setDocumentOrder();

					++theCurrent;
				}
				while(theCurrent != theEnd);
			}

			++theCurrent;
		}
		while(theCurrent != theEnd);
	}	
} // end constructor



KeyTable::~KeyTable()
{
}



const MutableNodeRefList&
KeyTable::getNodeSetByKey(
					  const XalanQName&			qname, 
					  const XalanDOMString&		ref) const
{
	const KeysMapType::const_iterator	i = m_keys.find(qname);

	if (i != m_keys.end())
	{
		const NodeListMapType&	theMap = (*i).second;

		const NodeListMapType::const_iterator	j = theMap.find(ref);

		if (j != theMap.end())
		{
			return (*j).second;
		}
	}

	// It makes things much easier if we always return
	// a list of nodes.  So this is just an empty one
	// to return when the ref is not found.
	return s_dummyList;
}



inline void
addIfNotFound(
			StylesheetExecutionContext&		executionContext,
			MutableNodeRefList&				theNodeList,
			XalanNode*						theNode)
{
	theNodeList.addNodeInDocOrder(theNode, executionContext);
}



static const NodeRefList	theEmptyList;

void
KeyTable::processKeyDeclaration(
			KeysMapType&					theKeys,
			const KeyDeclaration&			kd,
			XalanNode*						testNode,
			const PrefixResolver&			resolver,
			StylesheetExecutionContext&		executionContext)
{
	// Query from the node, according the the select pattern in the
	// use attribute in xsl:key.
	assert(kd.getUse() != 0);

	const XObjectPtr	xuse(kd.getUse()->execute(testNode, resolver, theEmptyList, executionContext));

	if(xuse->getType() != XObject::eTypeNodeSet)
	{
		assert(kd.getQName() != 0);

		addIfNotFound(
			executionContext,
			theKeys[*kd.getQName()][xuse->str()],
			testNode);
	}
	else
	{
		const NodeRefListBase&	nl = xuse->nodeset();

		// Use each node in the node list as a key value that we'll be 
		// able to use to look up the given node.
		const NodeRefListBase::size_type	nUseValues = nl.getLength();

		StylesheetExecutionContext::GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&		nodeData = theGuard.get();

		// Use each node in the node list as a key value that we'll be 
		// able to use to look up the given node.
		for(unsigned int i = 0; i < nUseValues; ++i)
		{
			// Get the string value of the node to use as the result of the
			// expression.
			assert(nl.item(i) != 0);

			DOMServices::getNodeData(*nl.item(i), nodeData);

			assert(kd.getQName() != 0);

			addIfNotFound(
				executionContext,
				theKeys[*kd.getQName()][nodeData],
				testNode);

			clear(nodeData);
		}
	}  
}



XALAN_CPP_NAMESPACE_END
