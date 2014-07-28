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
#include "IkhWinLib2/CFileDialog.h"

BEGIN_IKHWINLIB2()

CFileDialog::CFileDialog(bool bOpen, LPCTSTR lpFilter, LPCTSTR lpDefExt, LPCTSTR lpTitle,
	DWORD flags, DWORD nFilterIndex, LPCTSTR lpFile, LPCTSTR lpInitDir)
{
	ZeroMemory(&m_of, sizeof(m_of));

	m_pfDlgOpen = bOpen ? GetOpenFileName : GetSaveFileName;

	if (lpFile != NULL)
	{
		lstrcpy(m_lpFile, lpFile);
	}
	else
	{
		m_lpFile[0] = L'\0';
	}
	m_of.lpstrFile = m_lpFile;
	m_of.nMaxFile = sizeof(m_lpFile) / sizeof(m_lpFile[0]);

	m_of.lStructSize = sizeof(m_of);

	m_of.Flags = flags;
	m_of.lpstrTitle = lpTitle;
	m_of.lpstrFilter = lpFilter;
	m_of.lpstrDefExt = lpDefExt;
	m_of.nFilterIndex = nFilterIndex;
	m_of.lpstrInitialDir = lpInitDir;

	m_lpFileTitle[0] = L'\0';
	m_of.lpstrFileTitle = m_lpFileTitle;
	m_of.nMaxFileTitle = sizeof(m_lpFileTitle) / sizeof(m_lpFileTitle[0]);

	m_lpCustomFilter[0] = L'\0';
	m_of.lpstrCustomFilter = m_lpCustomFilter;
	m_of.nMaxCustFilter = sizeof(m_lpCustomFilter) / sizeof(m_lpCustomFilter[0]);
}

INT_PTR CFileDialog::DoModal(HWND hWndParent)
{
	CWindow::AssertCreation(this);

	m_of.hwndOwner = hWndParent;

	CWindow::HookCreatingWindow(this);
	if (m_pfDlgOpen(&m_of))
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
