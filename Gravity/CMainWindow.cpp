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

#include "stdafx.h"
#include "CMainWindow.h"

#include <IkhWinLib2/CWndClass.h>

#include <boost/algorithm/string.hpp>

BEGIN_MSGMAP(CMainWindow, CForm)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_TIMER(OnTimer)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CMainWindow, CForm)

void CMainWindow::Create()
{
	CWindow::Create(CWndClass(), L"Gravity (day:0.0000000)");
}

BOOL CMainWindow::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CForm, lpcs))
		return FALSE;

	const int gap = 10;

	SuspendLayout();
	// Create
	m_RendererCtrl.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 100, 100, 1, *this);
	auto chd_RendererCtrl = AddChild(&m_RendererCtrl);
	m_CmdListEdit.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL
			| ES_MULTILINE | ES_NOHIDESEL | ES_READONLY,
		0, 0, 100, 150, 2, *this);
	auto chd_CmdListEdit = AddChild(&m_CmdListEdit);
	m_CmdInputEdit.Create(
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL,
		0, 0, 100, 26, 3, *this);
	auto chd_CmdInputEdit = AddChild(&m_CmdInputEdit);
	// m_RendererCtrl
	DockChild(chd_RendererCtrl, CForm::NoTargetOpt(), CForm::DockLeft, gap);
	DockChild(chd_RendererCtrl, CForm::NoTargetOpt(), CForm::DockTop, gap);
	DockChild(chd_RendererCtrl, CForm::NoTargetOpt(), CForm::DockRight, gap);
	DockChild(chd_RendererCtrl, chd_CmdListEdit, CForm::DockBottom, gap);
	// m_CmdListEdit
	DockChild(chd_CmdListEdit, CForm::NoTargetOpt(), CForm::DockLeft, gap);
	DockChild(chd_CmdListEdit, CForm::NoTargetOpt(), CForm::DockRight, gap);
	DockChild(chd_CmdListEdit, chd_CmdInputEdit, CForm::DockBottom, 0);
	// m_CmdInputEdit
	DockChild(chd_CmdInputEdit, CForm::NoTargetOpt(), CForm::DockLeft, gap);
	DockChild(chd_CmdInputEdit, CForm::NoTargetOpt(), CForm::DockRight, gap);
	DockChild(chd_CmdInputEdit, CForm::NoTargetOpt(), CForm::DockBottom, gap);
	ResumeLayout();

	m_CmdInputEdit.evtInput += m_efpCmdInput.set(std::bind(&CMainWindow::OnCmdInput, this, _1, _2));

	SetTimer(*this, 0, 1000, nullptr);

	return TRUE;
}

void CMainWindow::OnTimer(UINT id)
{
	switch (id)
	{
		case 0:
			SetWindowText(*this, L"Gravity (day:" + std::to_wstring(m_SphereManager.GetDay()) + L")");
			break;
	}
}

void CMainWindow::OnDestroy()
{
	KillTimer(*this, 0);

	MSG_FORWARD_WM_DESTROY(CForm);
	PostQuitMessage(0);
}

void CMainWindow::OnCmdInput(CCmdEditCtrl *pCtrl, const std::wstring &input)
{
	try
	{
		WriteLine(L"> " + input);
		m_Scripter.ExecuteLine(input);
	}
	catch (ScripterException &e)
	{
		WriteLine(L"<err> " + e.Message());
	}
}

void CMainWindow::WriteLine(const std::wstring &str)
{
	DWORD size = GetWindowTextLength(m_CmdListEdit);

	DWORD beg, end;
	SendMessage(m_CmdListEdit, EM_GETSEL, reinterpret_cast<WPARAM>(&beg), reinterpret_cast<LPARAM>(&end));

	SendMessage(m_CmdListEdit, EM_SETSEL, -1, 0);
	SendMessage(m_CmdListEdit, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(str.c_str()));

	SendMessage(m_CmdListEdit, EM_SETSEL, -1, 0);
	SendMessage(m_CmdListEdit, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(L"\r\n"));

	if (!(size == beg && beg == end))
	{
		SendMessage(m_CmdListEdit, EM_SETSEL, beg, end);
	}
}

void CMainWindow::WriteMultiLine(const std::wstring &str)
{
	WriteLine(boost::algorithm::replace_all_copy(str, L"\n", L"\r\n"));
}

void CMainWindow::CreateSphere(const Sphere &sp)
{
	m_SphereManager.AddSphere(sp);
}
