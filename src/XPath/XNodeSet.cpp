/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
// Class header file.
#include "XNodeSet.hpp"



#include <limits>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include "ResultTreeFrag.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



XNodeSet::XNodeSet(
			XPathEnvSupport&		envSupport,
			XPathSupport&			support,
			const NodeRefListBase&	value) :
	XObject(&envSupport),
	m_support(support),
	m_value(value),
	m_resultTreeFrag()
{
}



XNodeSet::XNodeSet(
			XPathEnvSupport&			envSupport,
			XPathSupport&				support,
			const MutableNodeRefList&	value) :
	XObject(&envSupport),
	m_support(support),
	m_value(value),
	m_resultTreeFrag()
{
}



XNodeSet::XNodeSet(
			XPathEnvSupport&	envSupport,
			XPathSupport&		support,
			const DOM_Node&		value) :
	XObject(&envSupport),
	m_support(support),
	m_value(),
	m_resultTreeFrag()
{
	m_value.addNode(value);
}



XNodeSet::XNodeSet(const XNodeSet&	source,
				   bool				deepClone) :
	XObject(source),
	m_support(source.m_support),
	m_value(source.m_value),
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						dynamic_cast<ResultTreeFragBase*>(source.m_resultTreeFrag->clone(deepClone)))
#else
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						source.m_resultTreeFrag->clone(deepClone))
#endif
{
}



XNodeSet::~XNodeSet()
{
}



XNodeSet::eObjectType
XNodeSet::getType() const
{
	return eTypeNodeSet;
}



DOMString
XNodeSet::getTypeString() const
{
	return "#NODESET";
}



double
XNodeSet::num() const
{
	double result = DoubleSupport::getNaN();

	if(m_value.getLength() > 0)
	{
		const DOMString		s = m_support.getNodeData(m_value.item(0));

		if(0 == length(s))
		{
			result = 0;
		}
		else
		{
			result = DOMStringToDouble(s);
		}
	}

	return result;
}



bool
XNodeSet::boolean() const
{
	return m_value.getLength() > 0 ? true : false;
}



DOMString
XNodeSet::str() const
{
	DOMString	theResult;

	if (m_value.getLength() > 0)
	{
		const DOM_Node	theNode = m_value.item(0);

		const int		theType = theNode.getNodeType();

		if (theType == DOM_Node::COMMENT_NODE ||
			theType == DOM_Node::PROCESSING_INSTRUCTION_NODE)
		{
			theResult = theNode.getNodeValue();
		}
		else
		{
			theResult = m_support.getNodeData(theNode);
		}
	}

	return theResult;
}



const ResultTreeFragBase&
XNodeSet::rtree() const
{
	assert(m_envSupport != 0);

	if (m_resultTreeFrag.get() == 0)
	{
		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(m_envSupport->getDOMFactory(),
							   m_support);

#if defined(XALAN_OLD_AUTO_PTR)
		m_resultTreeFrag = std::auto_ptr<ResultTreeFragBase>(theFrag);
#else
	    m_resultTreeFrag.reset(theFrag);
#endif

		const int	nNodes = m_value.getLength();

		for(int i = 0; i < nNodes; i++)
		{
			m_resultTreeFrag->appendChild(m_value.item(i).cloneNode(true));
		}
	}

	return *m_resultTreeFrag.get();
}



ResultTreeFragBase&
XNodeSet::rtree()
{
	assert(m_envSupport != 0);

	if (m_resultTreeFrag.get() == 0)
	{
		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(m_envSupport->getDOMFactory(),
							   m_support);

#if defined(XALAN_OLD_AUTO_PTR)
		m_resultTreeFrag = std::auto_ptr<ResultTreeFragBase>(theFrag);
#else
	    m_resultTreeFrag.reset(theFrag);
#endif

		const int	nNodes = m_value.getLength();

		for(int i = 0; i < nNodes; i++)
		{
			m_resultTreeFrag->appendChild(m_value.item(i).cloneNode(true));
		}
	}

	return *m_resultTreeFrag.get();
}



const NodeRefListBase&
XNodeSet::nodeset() const
{
	return m_value;
}



const MutableNodeRefList&
XNodeSet::mutableNodeset() const
{
	return m_value;
}



MutableNodeRefList&
XNodeSet::mutableNodeset()
{
	return m_value;
}



void
XNodeSet::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



void
XNodeSet::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



bool
XNodeSet::equals(const XObject&		theRHS) const
{
	bool							fResult = false;

	const XNodeSet::eObjectType 	theType =
		theRHS.getType();

	if (XObject::eTypeNodeSet == theType)
	{
		// See the XPath spec., section 3.4 for an explanation of this lunacy...
		const int			theLHSLength = m_value.getLength();

		if (theLHSLength > 0)
		{
			const XNodeSet&		theRHSNodeSet = static_cast<const XNodeSet&>(theRHS);

			const int			theRHSLength = theRHSNodeSet.m_value.getLength();

			// See if there's at least one pair of nodes in the two nodes sets
			// whose "string" values are equal...
			for(int	i = 0; i < theLHSLength && fResult == false; i++)
			{
				const DOM_Node	theCurrent(m_value.item(i));

				for (int j = 0; j < theRHSLength && fResult == false; j++)
				{
					if (::equals(m_support.getNodeData(theCurrent),
								 m_support.getNodeData(theRHSNodeSet.m_value.item(j))) == true)
					{
						fResult = true;
					}
				}
			}
		}
	}
	else if(XObject::eTypeString == theType ||
	   XObject::eTypeResultTreeFrag == theType)
	{
		if (::equals(str(), theRHS.str()) == true)
		{
			fResult = true;
		}
	}
	else if(XObject::eTypeBoolean == theType)
	{
		if (boolean() == theRHS.boolean())
		{
			fResult = true;
		}
	}
	else if(XObject::eTypeNumber == theType)
	{
		if (num() == theRHS.num())
		{
			fResult = true;
		}
	}

	return fResult;
}
