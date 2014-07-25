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
#include "IkhWinLib2/CWindow.h"
#include "IkhWinLib2/CSystemError.h"
#include "IkhWinLib2/CLogicError.h"

using namespace IkhProgram::IkhWinLib2;

namespace
{
	const LPCTSTR CntStr_WindowPtrProp = TEXT("IkhWinLib_WinProp_WindowPtr");

	__declspec(thread) CWindow *Static_pHookingWnd;
	__declspec(thread) HHOOK Static_hHook = nullptr;
}

BEGIN_IKHWINLIB2()

void CWindow::Attach(HWND hWnd)
{
	assert(this != nullptr);

	if (m_hWnd != nullptr)
		throw CInvalidOperationError(L"이 객체는 이미 창에 부착되어 있습니다.");

	m_hWnd = hWnd;

	SetProp(m_hWnd, CntStr_WindowPtrProp, (HANDLE)this);
	m_OldProc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
}

void CWindow::Deattach()
{
	assert(this != nullptr);
	assert(m_hWnd != nullptr);
	assert(m_OldProc != nullptr);

	SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)m_OldProc);
	RemoveProp(m_hWnd, CntStr_WindowPtrProp);

	m_OldProc = nullptr;
	m_hWnd = nullptr;
}

void CWindow::CreateEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
	AssertCreation(this);

	HookCreatingWindow(this);
	CreateWindowEx(dwExStyle, lpClassName, lpWindowName,
		dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr), nullptr);

	if (m_hWnd == nullptr)
	{
		CleanHookCreatingWindow(this);
		throw CWindowCreationError();
	}
}

void CWindow::SetFullscreen(bool bFullscreen /* = true */)
{
	LONG code;

	if (bFullscreen)
	{
		DEVMODE dm = { 0 };

		RECT rt;
		GetWindowRect(*this, &rt);

		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = rt.right - rt.left;
		dm.dmPelsHeight = rt.bottom - rt.top;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		code = ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
	}
	else
	{
		code = ChangeDisplaySettings(NULL, 0);
	}

	if (code != DISP_CHANGE_SUCCESSFUL)
		throw CChangeDisplaySettingsError(code);
}

CWindow::~CWindow()
{
	if (m_hWnd != nullptr)
		Deattach();
}

// protected static 함수
#ifdef _DEBUG
void CWindow::AssertCreation(CWindow *pWnd)
{
	assert(pWnd != nullptr);
	assert(pWnd->m_hWnd == nullptr);
	assert(Static_hHook == nullptr);
}
#endif

void CWindow::HookCreatingWindow(CWindow *pWnd)
{
	Static_pHookingWnd = pWnd;
	Static_hHook = SetWindowsHookEx(WH_CBT, HookProc, nullptr, GetCurrentThreadId());
}

void CWindow::CleanHookCreatingWindow(CWindow *pWnd)
{
	if (Static_hHook != nullptr)
	{
		UnhookWindowsHookEx(Static_hHook);
	}
	Static_hHook = nullptr;
}

// CALLBACK 함수
LRESULT CALLBACK CWindow::HookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0)
	{
		switch (code)
		{
			case HCBT_CREATEWND:
				HWND hWnd = (HWND)wParam;

				Static_pHookingWnd->Attach(hWnd);

				UnhookWindowsHookEx(Static_hHook);
				Static_hHook = nullptr;

				return 0;
		}
	}
	return CallNextHookEx(Static_hHook, code, wParam, lParam);
}

LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	CWindow *pWindow = (CWindow *)GetProp(hWnd, CntStr_WindowPtrProp);
	LRESULT result;

	assert(pWindow != nullptr);
	assert(pWindow->m_hWnd != nullptr);
	assert(pWindow->m_OldProc != nullptr);

	if (!pWindow->PreMessage(iMessage, wParam, lParam, result))
	{
		return result;
	}

	switch (iMessage)
	{
		case WM_NCCREATE:
			pWindow->PreNcCreate((LPCREATESTRUCT)lParam);
			break;
	}

	result = pWindow->MessageProc(iMessage, wParam, lParam);

	switch (iMessage)
	{
		case WM_CREATE:
			pWindow->PostCreate();
			break;
		case WM_NCDESTROY:
			pWindow->PostNcDestroy();
			pWindow->m_hWnd = nullptr;
			break;
	}

	return result;
}

LRESULT CWindow::MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(m_OldProc, *this, iMessage, wParam, lParam);
}

LRESULT CWindow::SendMsg(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return SendMessage(m_hWnd, iMessage, wParam, lParam);
}
void CWindow::PostMsg(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (!PostMessage(m_hWnd, iMessage, wParam, lParam))
		throw CPostMsgError();
}

boost::future<LRESULT> CWindow::PostMsgEx(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	throw CNotImplementedException();
}

bool CWindow::PreMessage(UINT iMessage, WPARAM wParam, LPARAM lParam, LRESULT &result) { return true; }
void CWindow::PreNcCreate(LPCREATESTRUCT lpcs) { }
void CWindow::PostCreate() { }
void CWindow::PostNcDestroy() { }

END_IKHWINLIB2()
