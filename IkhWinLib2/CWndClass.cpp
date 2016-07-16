// Copyright (c) 2014, ÀÓ°æÇö
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
#include "IkhWinLib2/CWndClass.h"
#include "IkhWinLib2/CSystemError.h"

namespace
{
	unsigned s_id = 0;
}

BEGIN_IKHWINLIB2()

std::wstring CWndClass::Generate()
{
	TCHAR name[33];
	wsprintf(name, L"IkhWinLib2_Window_Class_%08x", s_id++);
	return name;
}

CWndClass::CWndClass(LPCTSTR lpszClassName, UINT style, HBRUSH hbrBackground, LPCTSTR lpszMenuName,
	HCURSOR hCursor, HICON hIcon, HICON hIconSm)
{
	m_wc.cbSize = sizeof(m_wc);
	m_wc.cbClsExtra = 0;
	m_wc.cbWndExtra = 0;
	m_wc.hbrBackground = hbrBackground;
	m_wc.hCursor = (hCursor == nullptr) ? LoadCursor(nullptr, IDC_ARROW) : hCursor;
	m_wc.hIcon = (hIcon == nullptr) ? LoadIcon(nullptr, IDI_APPLICATION) : hIcon;
	m_wc.hIconSm = hIconSm;
	m_wc.hInstance = GetModuleHandle(nullptr);
	m_wc.lpfnWndProc = ::DefWindowProc;
	m_wc.lpszMenuName = lpszMenuName;
	m_wc.style = style;

	m_name = (lpszClassName == nullptr) ? Generate() : lpszClassName;
	m_wc.lpszClassName = m_name.c_str();

	if (RegisterClassEx(&m_wc) == NULL)
		throw CRegisterClassError();
}

CWndClass::CWndClass(const WNDCLASS &wc)
{
	m_wc.cbSize = sizeof(m_wc);
	m_wc.cbClsExtra = wc.cbClsExtra;
	m_wc.cbWndExtra = wc.cbWndExtra;
	m_wc.hbrBackground = wc.hbrBackground;
	m_wc.hCursor = wc.hCursor;
	m_wc.hIcon = wc.hIcon;
	m_wc.hIconSm = nullptr;
	m_wc.hInstance = GetModuleHandle(nullptr);
	m_wc.lpfnWndProc = wc.lpfnWndProc;
	m_wc.lpszMenuName = wc.lpszMenuName;
	m_wc.style = wc.style;

	if (RegisterClassEx(&m_wc) == NULL)
		throw CRegisterClassError();
}

CWndClass::CWndClass(const WNDCLASSEX &wc)
{
	assert(wc.cbSize == sizeof(wc));

	m_wc = wc;

	m_name = wc.lpszClassName;
	m_wc.lpszClassName = m_name.c_str();

	if (RegisterClassEx(&m_wc) == NULL)
		throw CRegisterClassError();
}

END_IKHWINLIB2()
