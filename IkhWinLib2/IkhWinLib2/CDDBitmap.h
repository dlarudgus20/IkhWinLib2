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

class CDDBitmap : public CObject, private boost::noncopyable
{
private:
	class CMemDC : private boost::noncopyable
	{
	private:
		HDC m_hMemDC;
		HGDIOBJ m_hOld;
	public:
		explicit CMemDC(HDC hdc, HBITMAP hBit);
		~CMemDC();
		operator HDC() const NOEXCEPT { return m_hMemDC; }
	};

	HBITMAP m_hBit;

public:
	explicit CDDBitmap(LPCTSTR lpResBitmap);
	virtual ~CDDBitmap();

	BOOL DrawBit(HDC hdc, int x, int y, int cx = -1, int cy = -1,
		int BitX = 0, int BitY = 0, DWORD dwRop = SRCCOPY) const;
	BOOL DrawStretch(HDC hdc, int x, int y, int cx, int cy,
		int BitX, int BitY, int BitCx, int BitCy, DWORD dwRop = SRCCOPY) const;
	BOOL DrawTransparent(HDC hdc, int x, int y, int cx, int cy,
		int BitX, int BitY, int BitCx, int BitCy, UINT crTransparent) const;

	void GetBitmapStruct(BITMAP &ret) const;

	operator HBITMAP() const NOEXCEPT;
};

inline CDDBitmap::operator HBITMAP() const NOEXCEPT
{
	return m_hBit;
}

END_IKHWINLIB2()
