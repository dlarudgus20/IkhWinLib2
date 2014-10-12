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

#include "CModelessDialog.h"

BEGIN_IKHWINLIB2()

class CFindReplaceDialog : public CModelessDialog
{
private:
	FINDREPLACE m_fp;
	HWND (WINAPI *m_pfnCreate)(LPFINDREPLACE lpfr);
	TCHAR m_lpFind[1024];
	TCHAR m_lpReplace[1024];

public:
	static UINT GetFRMessage();

	explicit CFindReplaceDialog(bool bFind = true, DWORD flags = 0,
		LPCTSTR lpFind = TEXT(""), LPCTSTR lpReplace = TEXT(""));

	virtual void CreateModeless(HWND hWndParent) override;

	LPCTSTR GetFindStr() const NOEXCEPT;
	LPCTSTR GetReplaceStr() const NOEXCEPT;

	DWORD GetFlag() const NOEXCEPT;

	bool IsPushFindNext() const NOEXCEPT;
	bool IsMatchCase() const NOEXCEPT;
	bool IsPushReplace() const NOEXCEPT;
	bool IsPushReplaceAll() const NOEXCEPT;
	bool IsWholeWord() const NOEXCEPT;
};

inline UINT CFindReplaceDialog::GetFRMessage()
{
	return RegisterWindowMessage(FINDMSGSTRING);
}

inline LPCTSTR CFindReplaceDialog::GetFindStr() const NOEXCEPT
{
	return m_lpFind;
}
inline LPCTSTR CFindReplaceDialog::GetReplaceStr() const NOEXCEPT
{
	return m_lpReplace;
}

inline DWORD CFindReplaceDialog::GetFlag() const NOEXCEPT
{
	return m_fp.Flags;
}

inline bool CFindReplaceDialog::IsPushFindNext() const NOEXCEPT
{
	return (GetFlag() & FR_FINDNEXT) != 0;
}
inline bool CFindReplaceDialog::IsMatchCase() const NOEXCEPT
{
	return (GetFlag() & FR_MATCHCASE) != 0;
}
inline bool CFindReplaceDialog::IsPushReplace() const NOEXCEPT
{
	return (GetFlag() & FR_REPLACE) != 0;
}
inline bool CFindReplaceDialog::IsPushReplaceAll() const NOEXCEPT
{
	return (GetFlag() & FR_REPLACEALL) != 0;
}
inline bool CFindReplaceDialog::IsWholeWord() const NOEXCEPT
{
	return (GetFlag() & FR_WHOLEWORD) != 0;
}

END_IKHWINLIB2()
