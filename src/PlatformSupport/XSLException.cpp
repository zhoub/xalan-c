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
// Class header file...
#include "XSLException.hpp"



#include <sax/Locator.hpp>



#include "XalanUnicode.hpp"
#include "DOMStringHelper.hpp"



XSLException::XSLException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theURI,
		int						theLineNumber,
		int						theColumnNumber,
		const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(theURI),
	m_lineNumber(theLineNumber),
	m_columnNumber(theColumnNumber),
	m_type(theType)
{
}



XSLException::XSLException(
			const Locator&			theLocator,
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(theLocator.getSystemId()),
	m_lineNumber(theLocator.getLineNumber()),
	m_columnNumber(theLocator.getColumnNumber()),
	m_type(theType)
{
}



XSLException::XSLException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theType) :
	m_message(theMessage),
	m_uri(),
	m_lineNumber(-1),
	m_columnNumber(-1),
	m_type(theType)
{
}



XSLException::~XSLException()
{
}



XalanDOMString
XSLException::defaultFormat() const
{
	XalanDOMString	theBuffer;

	defaultFormat(theBuffer);

	return theBuffer;
}



void
XSLException::defaultFormat(XalanDOMString&		theBuffer) const
{
	defaultFormat(m_message, m_uri, m_lineNumber, m_columnNumber, m_type, theBuffer);
}



static
XalanDOMChar	colonString[] =
{
	XalanUnicode::charColon,
	XalanUnicode::charSpace,
	0
};



static
XalanDOMChar	lineString[] =
{
	XalanUnicode::charComma,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_e,
	XalanUnicode::charSpace,
	0
};



static
XalanDOMChar	columnString[] =
{
	XalanUnicode::charComma,
	XalanUnicode::charSpace,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_n,
	XalanUnicode::charSpace,
	0
};



void
XSLException::defaultFormat(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
			const XalanDOMString&	theType,
			XalanDOMString&			theBuffer)
{
	theBuffer += theType;
	theBuffer += colonString;
	theBuffer += theMessage;
	theBuffer += XalanDOMChar(XalanUnicode::charSpace);
	theBuffer += XalanDOMChar(XalanUnicode::charLeftParenthesis);
	theBuffer += theURI;
	theBuffer += lineString;
	LongToDOMString(theLineNumber, theBuffer);
	theBuffer += columnString;
	LongToDOMString(theColumnNumber, theBuffer);
	theBuffer += XalanDOMChar(XalanUnicode::charRightParenthesis);
}
