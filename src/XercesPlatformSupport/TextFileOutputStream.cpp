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
#include "TextFileOutputStream.hpp"



#include <cerrno>
#include <strstream>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanAutoPtr.hpp>




TextFileOutputStream::TextFileOutputStream(const DOMString&		theFileName) :
	XercesTextOutputStream(),
	m_fileName(theFileName),
	m_handle(0)
{
	const XalanArrayAutoPtr<char>	tmpName(theFileName.transcode());

	m_handle = fopen(tmpName.get(), "wt");

    if (m_handle == 0)
	{
		throw TextFileOutputStreamOpenException(theFileName,
												errno);
	}
}



TextFileOutputStream::~TextFileOutputStream()
{
    if (m_handle != 0)
	{
		fclose(m_handle);
	}
}



void
TextFileOutputStream::doFlush()
{
	fflush(m_handle);
}



void
TextFileOutputStream::writeData(
			const char*		theBuffer,
			unsigned long	theBufferLength)
{
	const size_t	theBytesWritten =
		fwrite(theBuffer,
			   1,
			   theBufferLength,
			   m_handle);

	if(theBytesWritten != theBufferLength)
	{
		throw TextFileOutputStreamWriteException(m_fileName,
												 errno);
	}
}



static const DOMString	theOpenExceptionType(XALAN_STATIC_UCODE_STRING("TextFileOutputStreamOpenException"));
static const DOMString	theWriteExceptionType(XALAN_STATIC_UCODE_STRING("TextFileOutputStreamWriteException"));



static DOMString
FormatMessageLocal(
			const DOMString&	theMessage,
			const DOMString&	theFileName,
			int					theErrorCode)
{
	DOMString	theResult(clone(theMessage));

	theResult += theFileName;

#if !defined(XALAN_NO_NAMESPACES)
using std::ostrstream;
#endif

	ostrstream	theFormatter;

	theFormatter << ".  The error code was "
				 << theErrorCode << "." << '\0';

	theResult += theFormatter.str();

	delete theFormatter.str();
	return theResult;
}



TextFileOutputStreamOpenException::TextFileOutputStreamOpenException(
		const DOMString&	theFileName,
		int					theErrorCode) :
	XercesTextOutputStreamException(FormatMessageLocal("Error opening file: ",
													 theFileName,
													 theErrorCode),
								   theOpenExceptionType)
{
}



TextFileOutputStreamOpenException::~TextFileOutputStreamOpenException()
{
}





TextFileOutputStreamWriteException::TextFileOutputStreamWriteException(
		const DOMString&	theFileName,
		int					theErrorCode) :
	XercesTextOutputStreamException(FormatMessageLocal("Error writing file: ",
													 theFileName,
													 theErrorCode),
								    theWriteExceptionType)
{
}



TextFileOutputStreamWriteException::~TextFileOutputStreamWriteException()
{
}
