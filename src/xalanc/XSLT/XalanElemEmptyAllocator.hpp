/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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

#if !defined(XALANELEMEMPTYALLOCATOR_INCLUDE_GUARD_12455133)
#define XALANELEMEMPTYALLOCATOR_INCLUDE_GUARD_12455133



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XSLT/ElemEmpty.hpp>



#include <xalanc/PlatformSupport/ReusableArenaAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XalanElemEmptyAllocator
{
public:

	typedef ElemEmpty							data_type;

	typedef ReusableArenaAllocator<data_type>	ArenaAllocatorType;

	typedef ArenaAllocatorType::size_type		size_type;

	/**
	 * Construct an instance that will allocate blocks of the specified size.
	 *
	 * @param theBlockSize The block size.
	 */
	XalanElemEmptyAllocator(size_type		theBlockCount);

	~XalanElemEmptyAllocator();
	
	/**
	 * Construct an instance
	 * 
	 * @param constructionContext The current construction context
	 * @param stylesheetTree The stylesheet containing element
	 * @param lineNumber The line number in the document
	 * @param columnNumber The column number in the document
	 * @param elementName The name of element.  Can be 0.
	 *
	 * @return A pointer to the new instance.
	 */
	data_type*
	create(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			const XalanDOMString*			elementName = 0);

	/**
	 * Construct an instance
	 * 
	 * @param constructionContext The current construction context
	 * @param stylesheetTree The stylesheet containing element
	 * @param elementName The name of element.  Can be 0.
	 */
	data_type*
	create(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMString*			elementName = 0);

	/**
	 * Destroy an instance previously created.
	 *
	 * @param theObject A pointer to the instance to destroy.
	 *
	 * @return true if the instance was destroyed, false if not.
	 */
	bool
	destroy(data_type*	theObject)
	{
		return m_allocator.destroyObject(theObject);
	}

	/**
	 * Determine if an object is owned by the allocator...
	 */
	bool
	ownsObject(const data_type*		theObject)
	{
		return m_allocator.ownsObject(theObject);
	}

	/**
	 * Delete all objects from the allocator.	 
	 */	
	void
	reset()
	{
		m_allocator.reset();
	}

	/**
	 * Get the number of ArenaBlocks currently allocated.
	 *
	 * @return The number of blocks.
	 */
	size_type
	getBlockCount() const
	{
		return m_allocator.getBlockCount();
	}

	/**
	 * Get size of an ArenaBlock, that is, the number
	 * of objects in each block.
	 *
	 * @return The size of the block
	 */
	size_type
	getBlockSize() const
	{
		return m_allocator.getBlockSize();
	}

private:

	// Not implemented...
	XalanElemEmptyAllocator(const XalanElemEmptyAllocator&);

	XalanElemEmptyAllocator&
	operator=(const XalanElemEmptyAllocator&);

	// Data members...
	ArenaAllocatorType	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANELEMEMPTYALLOCATOR_INCLUDE_GUARD_12455133
