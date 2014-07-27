﻿// Copyright (c) 2014, 임경현
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
#include "IkhWinLib2/CSplitterCtrl.h"
#include "IkhWinLib2/CWndClass.h"
using namespace IkhProgram::IkhWinLib2;

static CWndClass s_WndClass(L"IkhProgram::IkhWinLib2::CSplitterCtrl");

BEGIN_IKHWINLIB2()

BEGIN_MSGMAP(CSplitterCtrl, CWindow)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_PAINT(OnPaint)
	MSGMAP_WM_SIZE(OnSize)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CSplitterCtrl, CWindow)

void CSplitterCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, int id, HWND hWndParent)
{
	CWindow::CreateEx(dwExStyle, s_WndClass, NULL, dwStyle,
		x, y, nWidth, nHeight, hWndParent, (HMENU)id);
}

BOOL CSplitterCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CWindow, lpcs))
		return FALSE;

	return TRUE;
}

void CSplitterCtrl::OnPaint()
{
	PAINTSTRUCT ps;
	BeginPaint(*this, &ps);

	;

	EndPaint(*this, &ps);
}

void CSplitterCtrl::OnSize(UINT state, int cx, int cy)
{
	MSG_FORWARD_WM_SIZE(CWindow, state, cx, cy);
}

void CSplitterCtrl::OnDestroy()
{
	MSG_FORWARD_WM_DESTROY(CWindow);
}

END_IKHWINLIB2()
