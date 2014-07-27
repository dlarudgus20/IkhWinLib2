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

#include <IkhWinLib2/CWndClass.h>

#include "CMainWnd.h"

BEGIN_MSGMAP(CMainWnd, CForm)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CMainWnd, CForm)

void CMainWnd::Create()
{
	CWindow::Create(CWndClass(), L"IkhWinLib2 Studio", WS_OVERLAPPEDWINDOW);
}

BOOL CMainWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CForm, lpcs))
		return FALSE;

	SuspendLayout();

	m_ed2.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 100, 30, 1, *this);
	auto chded1 = AddChild(&m_ed1);
	m_ed1.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 0, 0, 100, 30, 0, *this);
	auto chded2 = AddChild(&m_ed2);

	DockChild(chded1, CForm::TargetForm(), CForm::DockTop, 10);
	DockChild(chded1, CForm::TargetForm(), CForm::DockLeft, 10);
	DockChild(chded1, CForm::TargetForm(), CForm::DockRight, 10);
	//DockChild(chded1, chded2, CForm::DockBottom, 10);
	DockChild(chded2, chded1, CForm::DockTop, 10);
	DockChild(chded2, CForm::TargetForm(), CForm::DockLeft, 10);
	DockChild(chded2, CForm::TargetForm(), CForm::DockRight, 10);
	DockChild(chded2, CForm::TargetForm(), CForm::DockBottom, 10);

	ResumeLayout();

	return TRUE;
}

void CMainWnd::OnDestroy()
{
	PostQuitMessage(0);
	MSG_FORWARD_WM_DESTROY(CForm);
}
