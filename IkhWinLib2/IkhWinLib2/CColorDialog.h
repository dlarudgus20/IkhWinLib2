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

class CColorDialog : public CModalDialog
{
private:
	CHOOSECOLOR m_cc;
	COLORREF m_CustColor[16];

public:
	explicit CColorDialog(DWORD flags = CC_ANYCOLOR,
		COLORREF defColor = 0, const COLORREF arCustomColor[16] = NULL);

	virtual INT_PTR DoModal(HWND hWndParent) override;

	COLORREF GetColor() const NOEXCEPT;
	void GetCustomColors(COLORREF *arCustomColor) const NOEXCEPT;
};

inline COLORREF CColorDialog::GetColor() const NOEXCEPT
{
	return m_cc.rgbResult;
}
inline void CColorDialog::GetCustomColors(COLORREF *arCustomColor) const NOEXCEPT
{
	memcpy(arCustomColor, m_CustColor, sizeof(m_CustColor));
}

END_IKHWINLIB2()
