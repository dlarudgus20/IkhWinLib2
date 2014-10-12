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

#include "stdafx.h"
#include "IkhWinLib2/CDDBitmap.h"
#include "IkhWinLib2/CSystemError.h"

BEGIN_IKHWINLIB2()

CDDBitmap::CMemDC::CMemDC(HDC hdc, HBITMAP hBit)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	if (m_hMemDC == nullptr)
		throw CCreationError(L"<CDDBitmap> 메모리 DC를 생성하지 못했습니다.");

	m_hOld = SelectObject(m_hMemDC, hBit);
}
CDDBitmap::CMemDC::~CMemDC()
{
	if (m_hMemDC != nullptr)
	{
		SelectObject(m_hMemDC, m_hOld);
		DeleteDC(m_hMemDC);
	}
}

CDDBitmap::CDDBitmap(LPCTSTR lpResBitmap)
{
	m_hBit = LoadBitmap(GetModuleHandle(NULL), lpResBitmap);
	if (m_hBit == nullptr)
		throw CCreationError(L"<CDDBitmap> 비트맵 리소스를 불러오는 데 실패했습니다.");
}

CDDBitmap::~CDDBitmap()
{
	if (m_hBit != nullptr)
		DeleteObject(m_hBit);
}

BOOL CDDBitmap::DrawBit(HDC hdc, int x, int y, int cx, int cy,
	int BitX, int BitY, DWORD dwRop) const
{
	assert(this != nullptr);
	assert(m_hBit != nullptr);

	CMemDC MemDC(hdc, m_hBit);

	BITMAP bit;
	if (cx == -1 || cy == -1)
		GetBitmapStruct(bit);

	if (cx == -1)
		cx = bit.bmWidth;
	if (cy == -1)
		cy = bit.bmHeight;

	return BitBlt(hdc, x, y, cx, cy, MemDC, BitX, BitY, dwRop);
}

BOOL CDDBitmap::DrawStretch(HDC hdc, int x, int y, int cx, int cy,
	int BitX, int BitY, int BitCx, int BitCy, DWORD dwRop) const
{ 
	assert(this != nullptr);
	assert(m_hBit != nullptr);

	CMemDC MemDC(hdc, m_hBit);

	BITMAP bit;
	if (cx == -1 || cy == -1 || BitCx == -1 || BitCy == -1)
		GetBitmapStruct(bit);

	if (cx == -1)
		cx = bit.bmWidth;
	if (cy == -1)
		cy = bit.bmHeight;
	if (BitCx == -1)
		BitCx = bit.bmWidth;
	if (BitCy == -1)
		BitCy = bit.bmHeight;

	return StretchBlt(hdc, x, y, cx, cy, MemDC, BitX, BitY, BitCx, BitCy, dwRop);
}

BOOL CDDBitmap::DrawTransparent(HDC hdc, int x, int y, int cx, int cy,
	int BitX, int BitY, int BitCx, int BitCy, UINT crTransparent) const
{
	assert(this != nullptr);
	assert(m_hBit != nullptr);

	CMemDC MemDC(hdc, m_hBit);

	BITMAP bit;
	if (cx == -1 || cy == -1 || BitCx == -1 || BitCy == -1)
		GetBitmapStruct(bit);

	if (cx == -1)
		cx = bit.bmWidth;
	if (cy == -1)
		cy = bit.bmHeight;
	if (BitCx == -1)
		BitCx = bit.bmWidth;
	if (BitCy == -1)
		BitCy = bit.bmHeight;

	return TransparentBlt(hdc, x, y, cx, cy, MemDC, BitX, BitY, BitCx, BitCy, crTransparent);
}

void CDDBitmap::GetBitmapStruct(BITMAP &ret) const
{
	assert(this != nullptr);
	assert(m_hBit != nullptr);
	GetObject(m_hBit, sizeof(BITMAP), &ret);
}

END_IKHWINLIB2()
