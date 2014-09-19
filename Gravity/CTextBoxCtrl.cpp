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

#include "CTextBoxCtrl.h"

#include <IkhWinLib2/CWndClass.h>
#include <IkhWinLib2/CDbBufDC.h>

BEGIN_MSGMAP(CTextBoxCtrl, CWindow)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_PAINT(OnPaint)
END_MSGMAP(CTextBoxCtrl, CWindow)

void CTextBoxCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, int id, HWND hWndParent)
{
	CWindow::CreateEx(
		dwExStyle,
		CWndClass(nullptr, CS_HREDRAW | CS_VREDRAW, nullptr),
		nullptr, dwStyle,
		x, y, nWidth, nHeight, hWndParent, (HMENU)id
		);
}

BOOL CTextBoxCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CWindow, lpcs))
		return FALSE;

	return TRUE;
}

void CTextBoxCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(*this, &ps);
	TextOut(ps.hdc, 0, 0, m_str);
	EndPaint(*this, &ps);
}

void CTextBoxCtrl::SetString(const std::wstring &str)
{
	m_str = str;

	HDC hdc = GetDC(*this);
	RECT rt = { 0, 0, 0, 0 };
	DrawText(hdc, m_str.c_str(), -1, &rt, DT_LEFT | DT_TOP | DT_CALCRECT);
	ReleaseDC(*this, hdc);

	SetWindowPos(*this, nullptr, 0, 0, rt.right, rt.bottom,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	InvalidateRect(*this, nullptr, FALSE);
}
