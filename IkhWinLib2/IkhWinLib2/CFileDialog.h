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

#pragma once

#include "CModalDialog.h"

BEGIN_IKHWINLIB2()

class CFileDialog : public CModalDialog
{
private:
	OPENFILENAME m_of;
	BOOL (APIENTRY *m_pfDlgOpen)(LPOPENFILENAME);
	TCHAR m_lpFile[MAX_PATH];
	TCHAR m_lpFileTitle[MAX_PATH];
	TCHAR m_lpCustomFilter[MAX_PATH];
public:
	explicit CFileDialog(bool bOpen = true,
		LPCTSTR lpFilter = TEXT("모든 파일\0*.*\0"),
		LPCTSTR lpDefExt = NULL,
		LPCTSTR lpTitle = NULL,
		DWORD flags = OFN_NOCHANGEDIR,
		DWORD nFilterIndex = 0,
		LPCTSTR lpFile = NULL,
		LPCTSTR lpInitDir = NULL);

	virtual INT_PTR DoModal(HWND hWndParent) override;

	LPCTSTR GetFileName() const NOEXCEPT;
	LPCTSTR GetFileTitleName() const NOEXCEPT;
	LPCTSTR GetCustomFilter() const NOEXCEPT;
	DWORD GetSelFilter() const NOEXCEPT;
	DWORD GetExtOffset() const NOEXCEPT;
};

inline LPCTSTR CFileDialog::GetFileName() const NOEXCEPT { return m_lpFile; }
inline LPCTSTR CFileDialog::GetFileTitleName() const NOEXCEPT { return m_lpFileTitle; }
inline LPCTSTR CFileDialog::GetCustomFilter() const NOEXCEPT { return m_lpCustomFilter; }
inline DWORD CFileDialog::GetSelFilter() const NOEXCEPT { return m_of.nFilterIndex; }
inline DWORD CFileDialog::GetExtOffset() const NOEXCEPT { return m_of.nFileExtension; }

END_IKHWINLIB2()
