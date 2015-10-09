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

BEGIN_IKHWINLIB2()

class CDbBufDC : public CObject, private boost::noncopyable
{
	friend class CDbBufBitmap;
private:
	HDC m_hMemDC;
	HGDIOBJ m_hOld;
	CDbBufBitmap *m_pbit;

	void init();
	void destroy();
	void shallow_copy(CDbBufDC &&rhs);

	explicit CDbBufDC(CDbBufBitmap &bit);
public:
	virtual ~CDbBufDC();

	CDbBufDC(CDbBufDC &&rhs);
	CDbBufDC &operator =(CDbBufDC &&rhs);

	void Draw(HWND hWnd = nullptr) const;
	void Clear() const;

	operator HDC() const NOEXCEPT;
	CDbBufBitmap &GetDbBufBitmap() const NOEXCEPT;
};

class CDbBufBitmap : public CObject, private boost::noncopyable
{
	friend CDbBufDC;
private:
	HWND m_hWnd;
	HBITMAP m_hBitmap;
	HBRUSH m_hBrush;
	RECT m_rt;

	void init();
	void destroy();
	void shallow_copy(CDbBufBitmap &&rhs);

public:
	explicit CDbBufBitmap(HWND hWnd, HBRUSH hBrush);
	virtual ~CDbBufBitmap();

	CDbBufBitmap(CDbBufBitmap &&rhs);
	CDbBufBitmap &operator =(CDbBufBitmap &&rhs);

	CDbBufDC GetDbBufDC();
	void Resize();

	HBRUSH GetBrush() const NOEXCEPT;
	void SetBrush(HBRUSH hBrush) NOEXCEPT;

	const RECT &GetBitmapSize() const NOEXCEPT;
};

inline CDbBufBitmap::CDbBufBitmap(CDbBufBitmap &&rhs)
{
	shallow_copy(std::forward<CDbBufBitmap>(rhs));
}
inline CDbBufBitmap &CDbBufBitmap::operator =(CDbBufBitmap &&rhs)
{
	destroy();
	shallow_copy(std::forward<CDbBufBitmap>(rhs));
	return *this;
}

inline HBRUSH CDbBufBitmap::GetBrush() const NOEXCEPT
{
	return m_hBrush;
}
inline void CDbBufBitmap::SetBrush(HBRUSH hBrush) NOEXCEPT
{
	m_hBrush = hBrush;
}
inline const RECT &CDbBufBitmap::GetBitmapSize() const NOEXCEPT
{
	return m_rt;
}

inline CDbBufDC::CDbBufDC(CDbBufDC &&rhs)
{
	shallow_copy(std::forward<CDbBufDC>(rhs));
}
inline CDbBufDC &CDbBufDC::operator =(CDbBufDC &&rhs)
{
	destroy();
	shallow_copy(std::forward<CDbBufDC>(rhs));
	return *this;
}

inline CDbBufDC::operator HDC() const NOEXCEPT
{
	return m_hMemDC;
}
inline CDbBufBitmap &CDbBufDC::GetDbBufBitmap() const NOEXCEPT
{
	return *m_pbit;
}

END_IKHWINLIB2()
