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
 *
 * $ Id: $
 *
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <deque>



#include <XalanDOM/XalanDOMString.hpp>



// Base class include file.
#include <XPath/XPathExecutionContext.hpp>



#include <XPath/NodeRefList.hpp>



class XPathEnvSupport;



/**
 * A basic implementation of the class XPathExecutionContext.
 */
class XALAN_XPATH_EXPORT XPathExecutionContextDefault : public XPathExecutionContext
{
public:

	/**
	 * Construct an XPathExecutionContextDefault object
	 *
	 * @param theXpathEnvSupport XPath environment support class instance
	 * @param theXPathSupport    XPath support class instance
	 * @param theXobjectFactory  factory class instance for XObjects
	 * @param theCurrentNode     current node in the source tree
	 * @param theContextNodeList node list for current context
	 * @param thePrefixResolver  pointer to prefix resolver to use
	 */
	explicit
	XPathExecutionContextDefault(
			XPathEnvSupport&		theXPathEnvSupport,
			XPathSupport&			theXPathSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase*	theContextNodeList = 0,
			const PrefixResolver*	thePrefixResolver = 0);

	virtual
	~XPathExecutionContextDefault();


	// These interfaces are inherited from XPathExecutionContext...

	virtual void
	reset();

	virtual XalanNode*
	getCurrentNode() const;

	virtual void
	setCurrentNode(XalanNode*		theCurrentNode);

	virtual XObjectFactory&
	getXObjectFactory() const;

	virtual XObject*
	createNodeSet(XalanNode&	theNode);

	virtual bool
	isIgnorableWhitespace(const XalanText&	node) const;

	virtual XalanDOMString
	getNamespaceOfNode(const XalanNode&		n) const;

	virtual XalanDOMString
	getNameOfNode(const XalanNode&	n) const;

	virtual XalanDOMString
	getLocalNameOfNode(const XalanNode&		n) const;

	virtual XalanNode*
	getParentOfNode(const XalanNode&	n) const;

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const;

	virtual XalanDOMString
	getNodeData(const XalanNode&	n) const;

	virtual XalanElement*
	getElementByID(
			const XalanDOMString&	id,
			const XalanDocument&	doc) const;

	virtual const NodeRefListBase&
	getContextNodeList() const;

	virtual void	
	setContextNodeList(const NodeRefListBase&	theList);

	virtual unsigned int
	getContextNodeListLength() const;

	virtual unsigned int
	getContextNodeListPosition(const XalanNode&		contextNode) const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const;

	virtual XObjectArgVectorType&
	pushArgVector();

	virtual void
	popArgVector();

	virtual const XObject*
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec);

	virtual XLocator*
	getXLocatorFromNode(const XalanNode*	node) const;

	virtual void
	associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator);

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const;

	virtual MutableNodeRefList*
	borrowMutableNodeRefList();

	virtual bool
	returnMutableNodeRefList(MutableNodeRefList*	theList);

	virtual MutableNodeRefList*
	createMutableNodeRefList() const;

	virtual bool
	getProcessNamespaces() const;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver);

	virtual const XObject*
	getVariable(
			const QName&	name) const;

	virtual const PrefixResolver*
	getPrefixResolver() const;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver);

	virtual XalanDOMString
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual XalanDocument*
	getDOMFactory() const;

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual XalanDOMString
	getUnparsedEntityURI(
			const XalanDOMString&		theName,
			const XalanDocument&		theDocument) const;

	virtual bool
	shouldStripSourceNode(const XalanNode&	node);

	virtual bool
	getThrowFoundIndex() const;

	virtual void
	setThrowFoundIndex(bool 	fThrow);

	virtual void
	setCurrentPattern(const XalanDOMString&		thePattern);

	virtual XalanDOMString
	getCurrentPattern() const;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name);


	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

protected:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<MutableNodeRefList*>			NodeRefListCacheType;
	typedef deque<XObjectArgVectorType>			XObjectArgVectorStackType;
#else
	typedef std::vector<MutableNodeRefList*>	NodeRefListCacheType;
	typedef std::deque<XObjectArgVectorType>	XObjectArgVectorStackType;
#endif

	typedef XObjectArgVectorStackType::iterator		ArgVectorStackIteratorType;

	enum { eMutableNodeRefListCacheMax = 50,
		   eArgVectorStackMax = 25,
		   eCachedArgVectorDefaultSize = 10 };

	XPathEnvSupport&				m_xpathEnvSupport;

	XPathSupport&					m_xpathSupport;

	XObjectFactory&					m_xobjectFactory;

	XalanNode*						m_currentNode;

	const NodeRefListBase*			m_contextNodeList;

	const PrefixResolver*			m_prefixResolver;

	bool							m_throwFoundIndex;

	XalanDOMString					m_currentPattern;

	NodeRefListCacheType			m_availableCachedNodeLists;

	NodeRefListCacheType			m_busyCachedNodeLists;

	XObjectArgVectorStackType		m_argVectorsStack;

	ArgVectorStackIteratorType		m_argVectorsStackPosition;

	static const NodeRefList		s_dummyList;
};



#endif	// XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
