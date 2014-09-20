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

#include <IkhWinLib2/CButtonCtrl.h>
#include <IkhWinLib2/CComboBoxCtrl.h>
#include <IkhWinLib2/CDateTimePickCtrl.h>
#include <IkhWinLib2/CEditCtrl.h>
#include <IkhWinLib2/CHotkeyCtrl.h>
#include <IkhWinLib2/CListBoxCtrl.h>
#include <IkhWinLib2/CProgressCtrl.h>
#include <IkhWinLib2/CPropViewCtrl.h>
#include <IkhWinLib2/CScrollBarCtrl.h>
#include <IkhWinLib2/CSpinbtnCtrl.h>
#include <IkhWinLib2/CStaticCtrl.h>
#include <IkhWinLib2/CTrackbarCtrl.h>
#include <IkhWinLib2/CTreeViewCtrl.h>

#include "CDesignerCtrl.h"

static CWndClass s_WndClass(L"DesignerCtrl");

BEGIN_MSGMAP(CDesignerCtrl, CForm)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_LBUTTONDOWN(OnLButtonDown)
	MSGMAP_WM_MOUSEMOVE(OnMouseMove)
	MSGMAP_WM_LBUTTONUP(OnLButtonUp)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CDesignerCtrl, CForm)

void CDesignerCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, int id, HWND hWndParent)
{
	CWindow::CreateEx(dwExStyle, s_WndClass, NULL, dwStyle,
		x, y, nWidth, nHeight, hWndParent, (HMENU)id);
}

CDesignerCtrl::CDesignerCtrl()
{

}

stlgc::vector<CDesignerCtrl::CtrlCtor> CDesignerCtrl::m_vtCtrlCtor = {
	[] { return std::shared_ptr<IControl>(); },
	[] { return std::shared_ptr<IControl>(new CStaticCtrl); },
	[] { return std::shared_ptr<IControl>(new CEditCtrl); },
	[] { return std::shared_ptr<IControl>(new CButtonCtrl); },
	[] { return std::shared_ptr<IControl>(new CListBoxCtrl); },
	[] { return std::shared_ptr<IControl>(new CProgressCtrl); },
	[] { return std::shared_ptr<IControl>(new CPropViewCtrl); },
	[] { return std::shared_ptr<IControl>(new CScrollbarCtrl); },
	[] { return std::shared_ptr<IControl>(new CSpinbtnCtrl); },
	[] { return std::shared_ptr<IControl>(new CTrackbarCtrl); },
	[] { return std::shared_ptr<IControl>(new CTreeViewCtrl); },
	[] { return std::shared_ptr<IControl>(new CDateTimePickCtrl); },
	[] { return std::shared_ptr<IControl>(new CComboBoxCtrl); },
	[] { return std::shared_ptr<IControl>(new CHotkeyCtrl); },
};
void CDesignerCtrl::InitToolList(CListBoxCtrl &ToolList)
{
	ListBox_AddString(ToolList, L"포인터");
	ListBox_AddString(ToolList, L"Static Text");
	ListBox_AddString(ToolList, L"Edit Box");
	ListBox_AddString(ToolList, L"Button");
	ListBox_AddString(ToolList, L"List Box");
	ListBox_AddString(ToolList, L"Progress");
	ListBox_AddString(ToolList, L"Property View");
	ListBox_AddString(ToolList, L"Scrollbar");
	ListBox_AddString(ToolList, L"Spin Button");
	ListBox_AddString(ToolList, L"Trackbar");
	ListBox_AddString(ToolList, L"Tree View");
	ListBox_AddString(ToolList, L"DateTime Pick");
	ListBox_AddString(ToolList, L"Combobox");
	ListBox_AddString(ToolList, L"Hotkey");
}
void CDesignerCtrl::OnToolListSelChange(int idx)
{
	assert(0 <= idx && static_cast<size_t>(idx) < m_vtCtrlCtor.size());
	m_NowCtrlCtor = (idx != 0) ? m_vtCtrlCtor[idx] : nullptr;
}

BOOL CDesignerCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CForm, lpcs))
		return FALSE;

	return TRUE;
}

void CDesignerCtrl::OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	m_SelRect.left = x;
	m_SelRect.top = y;
	SetCapture(*this);
}
void CDesignerCtrl::OnMouseMove(int x, int y, UINT keyFlags)
{
	if (GetCapture() == *this)
	{
		ChangeSelRect(x, y);
	}
}
void CDesignerCtrl::OnLButtonUp(int x, int y, UINT keyFlags)
{
	ChangeSelRect(x, y);
}
void CDesignerCtrl::ChangeSelRect(int x, int y)
{
	m_SelRect.right = x;
	m_SelRect.top = y;
	if (m_SelRect.left > m_SelRect.right)
		swap(m_SelRect.left, m_SelRect.right);
	if (m_SelRect.top > m_SelRect.bottom)
		swap(m_SelRect.top, m_SelRect.bottom);

	HDC hdc = GetDC(*this);
	HGDIOBJ hOld = SelectObject(hdc, CreatePen(PS_DOT, 0, RGB(128, 128, 128)));
	Rectangle(hdc, m_SelRect.left, m_SelRect.top, m_SelRect.right, m_SelRect.bottom);
	DeleteObject(SelectObject(hdc, hOld));
	ReleaseDC(*this, hdc);
}

void CDesignerCtrl::OnDestroy()
{
	MSG_FORWARD_WM_DESTROY(CForm);
}
