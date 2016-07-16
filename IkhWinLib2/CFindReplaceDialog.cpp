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
#include "IkhWinLib2/CFindReplaceDialog.h"
#include "IkhWinLib2/CSystemError.h"

BEGIN_IKHWINLIB2()

CFindReplaceDialog::CFindReplaceDialog(bool bFind, DWORD flags,
	LPCTSTR lpFind, LPCTSTR lpReplace)
{
	ZeroMemory(&m_fp, sizeof(m_fp));

	m_pfnCreate = bFind ? FindText : ReplaceText;

	if (lpFind != NULL)
		lstrcpy(m_lpFind, lpFind);
	else
		m_lpFind[0] = L'\0';

	if (lpReplace != NULL)
		lstrcpy(m_lpReplace, lpReplace);
	else
		m_lpFind[0] = L'\0';

	m_fp.lStructSize = sizeof(m_fp);

	m_fp.Flags = flags;

	m_fp.lpstrFindWhat = m_lpFind;
	m_fp.wFindWhatLen = sizeof(m_lpFind) / sizeof(m_lpFind[0]);
	m_fp.lpstrReplaceWith = m_lpReplace;
	m_fp.wReplaceWithLen = sizeof(m_lpReplace) / sizeof(m_lpReplace[0]);
}

void CFindReplaceDialog::CreateModeless(HWND hWndParent)
{
	CWindow::AssertCreation(this);

	m_fp.hwndOwner = hWndParent;

	CWindow::HookCreatingWindow(this);
	if (!m_pfnCreate(&m_fp))
	{
		CWindow::CleanHookCreatingWindow(this);
		throw CWindowCreationError(L"대화상자를 생성하지 못했습니다.");
	}
}

END_IKHWINLIB2()
