// Copyright (c) 2014, 임경현
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
#include "NonCopyable.h"

BEGIN_IKHWINLIB2()

template <typename T>
class CGdiObject : public CObject, private NonCopyable
{
private:
	T m_obj;

	void set(CGdiObject &&rhs) { m_obj = rhs.m_obj; rhs.m_obj = nullptr; }
	void destroy() { if (m_obj != nullptr) DeleteObject(m_obj); }

public:
	CGdiObject() : m_obj(nullptr) { }
	CGdiObject(T obj) : m_obj(obj) { }
	CGdiObject(CGdiObject &&rhs) { set(rhs); }
	CGdiObject &operator =(CGdiObject &&rhs) { destroy(); set(rhs); return *this; }
	virtual ~CGdiObject() { destroy(); }

	CGdiObject &operator =(T obj) { destroy(); m_obj = obj; }

	operator T() const NOEXCEPT { return m_obj; }
};

struct GdiObjectDeleter
{
	void operator ()(HGDIOBJ obj) const
	{
		DeleteObject(obj);
	}
};

template <typename T>
using gdi_unique_ptr = std::unique_ptr<typename std::remove_pointer<T>::type, GdiObjectDeleter>;

template <typename T>
using gdi_shared_ptr = std::shared_ptr<typename std::remove_pointer<T>::type>;

template <typename T>
inline gdi_shared_ptr<T> gdi_make_shared(T obj)
{
	return gdi_shared_ptr<T>(obj, GdiObjectDeleter());
}

END_IKHWINLIB2()
