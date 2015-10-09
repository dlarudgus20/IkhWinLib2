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

#include "CObject.h"
#include "CLogicError.h"

BEGIN_IKHWINLIB2()

class CMenu : public CObject, private boost::noncopyable
{
private:
	HMENU m_hMenu;
	bool m_bRequireDestroy;

public:
	explicit CMenu(HMENU hMenu);
	explicit CMenu(LPCTSTR lpMenu);
	virtual ~CMenu();

	void Attach(HMENU hMenu);

	operator HMENU() const NOEXCEPT;
};

inline CMenu::CMenu(HMENU hMenu)
	: m_bRequireDestroy(false)
{
	Attach(hMenu);
}

inline CMenu::CMenu(LPCTSTR lpMenu)
	: m_bRequireDestroy(true)
{
	HMENU hMenu = LoadMenu(GetModuleHandle(nullptr), lpMenu);
	if (hMenu == nullptr)
		throw CInvalidArgError(L"유효한 메뉴 리소스가 아닙니다.");

	Attach(hMenu);
}

inline void CMenu::Attach(HMENU hMenu)
{
	m_hMenu = hMenu;
}

inline CMenu::operator HMENU() const NOEXCEPT
{
	return m_hMenu;
}

END_IKHWINLIB2()
