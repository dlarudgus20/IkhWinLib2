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

/**
 * @file Utility.h
 * @brief 유틸리티들
 */

#pragma once

#include "Defines.h"

BEGIN_IKHWINLIB2()

/** @brief <c>TextOut</c> API 함수의 래퍼입니다. */
inline BOOL TextOut(HDC hdc, int x, int y, LPCTSTR lpString)
{
	return ::TextOut(hdc, x, y, lpString, lstrlen(lpString));
}
/** @brief <c>TextOut</c> API 함수의 래퍼입니다. */
inline BOOL TextOut(HDC hdc, int x, int y, const std::wstring &str)
{
	assert(str.size() <= INT_MAX);
	return ::TextOut(hdc, x, y, str.c_str(), (int)str.size());
}

/** @brief <c>SetWindowText</c> API 함수의 래퍼입니다. */
inline BOOL SetWindowText(HWND hWnd, const std::wstring &str)
{
	return ::SetWindowText(hWnd, str.c_str());
}

/** @brief <c>GetWindowText</c> API 함수의 래퍼입니다. */
inline std::wstring GetWindowText(HWND hWnd)
{
	std::vector<wchar_t> vt(::GetWindowTextLength(hWnd) + 1);
	::GetWindowText(hWnd, vt.data(), (int)vt.size());
	return std::wstring(vt.begin(), std::prev(vt.end()));
}

/** @brief <c>OutputDebugStringW</c> API 함수의 래퍼입니다. */
inline void OutputDebugStringW(const std::wstring &str)
{
	::OutputDebugStringW(str.c_str());
}
/** @brief <c>OutputDebugStringA</c> API 함수의 래퍼입니다. */
inline void OutputDebugStringA(const std::string &str)
{
	::OutputDebugStringA(str.c_str());
}

/** @brief <c>std::lock_guard</c> 표준 클래스의 래퍼입니다. */
#define SYN_LOCK_GUARD(obj) std::lock_guard<decltype(obj)> _l_g_(obj)

END_IKHWINLIB2()

#if !defined(IKHWINLIB2_NO_USING_UTIL) && !defined(IKHWINLIB2_NO_USING)
using IkhProgram::IkhWinLib2::TextOut;
using IkhProgram::IkhWinLib2::GetWindowText;
#endif

/**
 * @macro IKHWINLIB2_NO_USING_UTIL
 * @brief 유틸리티 함수/클래스들을 using하지 않습니다.
 */
