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

class CFontDialog : public CModalDialog
{
private:
	CHOOSEFONT m_cf;
	LOGFONT m_lf;
public:
	explicit CFontDialog(DWORD dwFlags = CF_SCREENFONTS | CF_EFFECTS,
		int nSizeMin = 0, int nSizeMax = 0, HDC hDC = NULL);
	explicit CFontDialog(DWORD dwFlags, const LOGFONT &LogFont,
		int nSizeMin = 0, int nSizeMax = 0, HDC hDC = NULL);

	virtual INT_PTR DoModal(HWND hWndParent) override;

	const LOGFONT &GetLogFont() const NOEXCEPT;
	const CHOOSEFONT &GetChooseFont() const NOEXCEPT;
};

inline const LOGFONT &CFontDialog::GetLogFont() const NOEXCEPT
{
	return m_lf;
}
inline const CHOOSEFONT &CFontDialog::GetChooseFont() const NOEXCEPT
{
	return m_cf;
}

END_IKHWINLIB2()
