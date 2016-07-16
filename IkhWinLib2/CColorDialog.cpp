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
#include "IkhWinLib2/CColorDialog.h"

BEGIN_IKHWINLIB2()

CColorDialog::CColorDialog(DWORD flags, COLORREF defColor, const COLORREF arCustomColor[16])
{
	ZeroMemory(&m_cc, sizeof(m_cc));

	if (arCustomColor != NULL)
	{
		memcpy(m_CustColor, arCustomColor, sizeof(m_CustColor));
	}
	else
	{
		ZeroMemory(m_CustColor, sizeof(m_CustColor));
	}

	m_cc.lStructSize = sizeof(m_cc);
	m_cc.Flags = flags;
	m_cc.rgbResult = defColor;
	m_cc.lpCustColors = m_CustColor;
}

INT_PTR CColorDialog::DoModal(HWND hWndParent)
{
	CWindow::AssertCreation(this);

	m_cc.hwndOwner = hWndParent;

	CWindow::HookCreatingWindow(this);
	if (ChooseColor(&m_cc))
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
