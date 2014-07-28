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
#include "IkhWinLib2/CFontDialog.h"

BEGIN_IKHWINLIB2()

CFontDialog::CFontDialog(DWORD dwFlags, int nSizeMin, int nSizeMax, HDC hDC)
{
	ZeroMemory(&m_cf, sizeof(m_cf));

	m_cf.lStructSize = sizeof(m_cf);
	m_cf.hInstance = GetModuleHandle(NULL);
	m_cf.lpLogFont = &m_lf;

	m_cf.Flags = dwFlags;
	m_cf.nSizeMin = nSizeMin;
	m_cf.nSizeMax = nSizeMax;
	m_cf.hDC = hDC;
}

CFontDialog::CFontDialog(DWORD dwFlags, const LOGFONT &LogFont, int nSizeMin, int nSizeMax, HDC hDC)
{
	ZeroMemory(&m_cf, sizeof(m_cf));

	memcpy(&m_lf, &LogFont, sizeof(m_lf));

	m_cf.lStructSize = sizeof(m_cf);
	m_cf.hInstance = GetModuleHandle(NULL);
	m_cf.lpLogFont = &m_lf;

	m_cf.Flags = dwFlags | CF_INITTOLOGFONTSTRUCT;
	m_cf.nSizeMin = nSizeMin;
	m_cf.nSizeMax = nSizeMax;
	m_cf.hDC = hDC;
}

INT_PTR CFontDialog::DoModal(HWND hWndParent)
{
	CWindow::AssertCreation(this);

	m_cf.hwndOwner = hWndParent;

	CWindow::HookCreatingWindow(this);
	if (ChooseFont(&m_cf))
	{
		return IDOK;
	}
	else
	{
		CWindow::CleanHookCreatingWindow(this);
		return IDCANCEL;
	}
}

END_IKHWINLIB2()
