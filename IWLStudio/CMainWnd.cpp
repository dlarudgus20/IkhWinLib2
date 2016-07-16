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

#include <IkhWinLib2/CWndClass.h>

#include "resource.h"
#include "CMainWnd.h"

BEGIN_MSGMAP(CMainWnd, CForm)
	MSGMAP_WM_CREATE(OnCreate)
	BEGIN_CMDMAP()
		CMDMAP_ID(ID_FILE_PROJNEW, OnFileProjNew)
		CMDMAP_ID(ID_FILE_PROJOPEN, OnFileProjOpen)
		CMDMAP_ID(ID_FILE_SAVE, OnFileSave)
		CMDMAP_ID(ID_FILE_ALLSAVE, OnFileAllSave)
		CMDMAP_ID(ID_FILE_EXIT, OnFileExit)

		CMDMAP_CTRL(LBN_SELCHANGE, TOOLLIST_ID, OnToolListSelChange)
	END_CMDMAP()
	MSGMAP_WM_CLOSE(OnClose)
	MSGMAP_WM_DESTROY(OnDestroy)
	BEGIN_CHAIN()
		MSG_CHAIN(m_pProject.get())
	END_CHAIN()
END_MSGMAP(CMainWnd, CForm)

void CMainWnd::Create()
{
	CWindow::Create(CWndClass(), L"IkhWinLib2 Studio v0.0.1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAIN_MENU))
		);
}

CMainWnd::CMainWnd()
{

}

BOOL CMainWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CForm, lpcs))
		return FALSE;

	SuspendLayout();
	// Create
	m_ToolList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOINTEGRALHEIGHT,
		0, 0, 100, 100, TOOLLIST_ID, *this);
	auto chd_ToolList = AddChild(&m_ToolList);
	m_DesignerCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 100, 100, DESIGNERCTRL_ID, *this);
	auto chd_DesignerCtrl = AddChild(&m_DesignerCtrl);
	m_ProjTree.Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 100, 100, PROJTREE_ID, *this);
	auto chd_ProjTree = AddChild(&m_ProjTree);
	m_PropView.Create(WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 100, 100, PROPVIEW_ID, *this);
	auto chd_PropView = AddChild(&m_PropView);
	// m_ToolList
	DockChild(chd_ToolList, CForm::NoTargetOpt(), CForm::DockLeft, 10);
	DockChild(chd_ToolList, CForm::NoTargetOpt(), CForm::DockTop, 10);
	DockChild(chd_ToolList, CForm::NoTargetOpt(), CForm::DockBottom, 10);
	// m_DesignerCtrl
	DockChild(chd_DesignerCtrl, chd_ToolList, CForm::DockLeft, 10);
	DockChild(chd_DesignerCtrl, CForm::NoTargetOpt(), CForm::DockTop, 10);
	DockChild(chd_DesignerCtrl, chd_ProjTree, CForm::DockRight, 10);
	DockChild(chd_DesignerCtrl, CForm::NoTargetOpt(), CForm::DockBottom, 10);
	// m_ProjTree
	DockChild(chd_ProjTree, CForm::NoTargetOpt(), CForm::DockTop, 10);
	DockChild(chd_ProjTree, CForm::NoTargetOpt(), CForm::DockRight, 10);
	DockChild(chd_ProjTree, chd_PropView, CForm::DockBottom, 10);
	// m_PropView
	DockChild(chd_PropView, CForm::NoTargetOpt(), CForm::DockBottom, 10);
	DockChild(chd_PropView, CForm::NoTargetOpt(), CForm::DockRight, 10);
	ResumeLayout();

	m_DesignerCtrl.InitToolList(m_ToolList);

	return TRUE;
}

void CMainWnd::OnToolListSelChange(int id, HWND hCtl, UINT codeNotify)
{
	m_DesignerCtrl.OnToolListSelChange(ListBox_GetCurSel(m_ToolList));
}

void CMainWnd::OnClose()
{
	DestroyWindow(*this);
}

void CMainWnd::OnDestroy()
{
	PostQuitMessage(0);
	MSG_FORWARD_WM_DESTROY(CForm);
}

