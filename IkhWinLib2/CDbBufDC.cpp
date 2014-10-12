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
#include "IkhWinLib2/CDbBufDC.h"
#include "IkhWinLib2/CSystemError.h"

BEGIN_IKHWINLIB2()

CDbBufBitmap::CDbBufBitmap(HWND hWnd, HBRUSH hBrush)
	: m_hWnd(hWnd), m_hBrush(hBrush)
{
	init();
}
CDbBufBitmap::~CDbBufBitmap()
{
	destroy();
}

void CDbBufBitmap::init()
{
	GetClientRect(m_hWnd, &m_rt);

	HDC hdc = GetDC(m_hWnd);
	if (hdc == nullptr)
		throw CCreationError(L"<CDbBufBitmap> 화면 DC를 생성하지 못했습니다.");

	BOOST_SCOPE_EXIT(hdc, this_)
	{
		ReleaseDC(this_->m_hWnd, hdc);
	} BOOST_SCOPE_EXIT_END ;

	HDC hMemDC = CreateCompatibleDC(hdc);
	if (hMemDC == nullptr)
		throw CCreationError(L"<CDbBufBitmap> 메모리 DC를 생성하지 못했습니다.");

	m_hBitmap = CreateCompatibleBitmap(hdc, m_rt.right, m_rt.bottom);
	if (m_hBitmap == nullptr)
	{
		DeleteDC(hMemDC);
		throw CCreationError(L"<CDbBufBitmap> 버퍼 비트맵을 생성하지 못했습니다.");
	}
}

void CDbBufBitmap::destroy()
{
	if (m_hBitmap != nullptr)
		DeleteObject(m_hBitmap);
}

void CDbBufBitmap::shallow_copy(CDbBufBitmap &&rhs)
{
	m_hWnd = rhs.m_hWnd;
	m_hBitmap = rhs.m_hBitmap;
	m_hBrush = rhs.m_hBrush;
	m_rt = rhs.m_rt;
	rhs.m_hWnd = nullptr;
	rhs.m_hBitmap = nullptr;
	rhs.m_hBrush = nullptr;
}

CDbBufDC CDbBufBitmap::GetDbBufDC()
{
	return CDbBufDC(*this);
}

void CDbBufBitmap::Resize()
{
	destroy();
	init();
}

CDbBufDC::CDbBufDC(CDbBufBitmap &bit)
	: m_hMemDC(nullptr), m_hOld(nullptr), m_pbit(&bit)
{
	init();
}
CDbBufDC::~CDbBufDC()
{
	destroy();
}

void CDbBufDC::init()
{
	HDC hdc = GetDC(m_pbit->m_hWnd);
	if (hdc == nullptr)
		throw CCreationError(L"<CDbBufDC> 화면 DC를 생성하지 못했습니다.");

	m_hMemDC = CreateCompatibleDC(hdc);
	ReleaseDC(m_pbit->m_hWnd, hdc);
	if (m_hMemDC == nullptr)
		throw CCreationError(L"<CDbBufDC> 메모리 DC를 생성하지 못했습니다.");
	
	m_hOld = SelectObject(m_hMemDC, m_pbit->m_hBitmap);
}

void CDbBufDC::destroy()
{
	if (m_hMemDC != nullptr)
	{
		SelectObject(m_hMemDC, m_hOld);
		DeleteDC(m_hMemDC);
	}
}

void CDbBufDC::shallow_copy(CDbBufDC &&rhs)
{
	m_hMemDC = rhs.m_hMemDC;
	m_hOld = rhs.m_hOld;
	m_pbit = rhs.m_pbit;
	rhs.m_hMemDC = nullptr;
	rhs.m_hOld = nullptr;
	rhs.m_pbit = nullptr;
}

void CDbBufDC::Draw(HWND hWnd) const
{
	if (hWnd == nullptr)
		hWnd = m_pbit->m_hWnd;

	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);

	LONG ar[] = {
		ps.rcPaint.left,
		ps.rcPaint.top,
		max(min(ps.rcPaint.right, m_pbit->m_rt.right) - ps.rcPaint.left, 0),
		max(min(ps.rcPaint.bottom, m_pbit->m_rt.bottom) - ps.rcPaint.top, 0)
	};
	BitBlt(ps.hdc, ar[0], ar[1], ar[2], ar[3], m_hMemDC, ar[0], ar[1], SRCCOPY);

	EndPaint(hWnd, &ps);
}

void CDbBufDC::Clear() const
{
	if (m_pbit->m_hBrush != nullptr)
		FillRect(m_hMemDC, &m_pbit->m_rt, m_pbit->m_hBrush);
}

END_IKHWINLIB2()
