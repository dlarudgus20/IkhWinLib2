// Copyright (c) 2014, ÀÓ°æÇö
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "CObject.h"

BEGIN_IKHWINLIB2()

class CException : public CObject
{
private:
	std::wstring m_str;

public:
	explicit CException(const std::wstring &str);
	explicit CException(std::wstring &&str = L"");

	virtual const std::wstring &Message() const NOEXCEPT;
};

#define IKHWINLIB2_MAKE_EXCEPTION(cls, base, defmsg) \
	class cls : public base \
	{ \
	public: \
		explicit cls(const std::wstring &str) : base(str) { } \
		explicit cls(std::wstring &&str = defmsg) : base(std::forward<std::wstring>(str)) { } \
	};

inline CException::CException(const std::wstring &str)
	: m_str(str) { }
inline CException::CException(std::wstring &&str)
	: m_str(std::forward<std::wstring>(str)) { }

inline const std::wstring &CException::Message() const { return m_str; }

END_IKHWINLIB2()
