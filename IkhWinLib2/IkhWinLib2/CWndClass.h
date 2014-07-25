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
 * @file CWndClass.h
 * @brief CWndClass 클래스
 */

#pragma once

#include "CObject.h"

BEGIN_IKHWINLIB2()

/**
 * @brief <c>WNDCLASSEX</c> 구조체를 래핑하는 클래스입니다. 이 클래스는 상속될 수 없습니다.
 * @example CWindow_Create.cpp
 * @example CWindow_CreateEx.cpp
 */
class CWndClass final : public CObject
{
private:
	WNDCLASSEX m_wc;
	std::wstring m_name;

public:
	/**
	 * @brief 생성자입니다.
	 * @param[in] lpszClassName 윈도우 클래스의 이름입니다. <c>nullptr</c>일 경우 라이브러리가 알아서 결정합니다.
	 * @param[in] style 윈도우 클래스의 스타일입니다.
	 * @param[in] hbrBackground 배경 브러시입니다.
	 * @param[in] lpszMenuName 메뉴입니다.
	 * @param[in] hCursor 커서입니다.
	 * @param[in] hIcon 아이콘입니다.
	 * @param[in] hIconSm 작은 아이콘입니다.
	 */
	explicit CWndClass(LPCTSTR lpszClassName = nullptr,
		UINT style = CS_HREDRAW | CS_VREDRAW,
		HBRUSH hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		LPCTSTR lpszMenuName = nullptr,
		HCURSOR hCursor = nullptr, HICON hIcon = nullptr, HICON hIconSm = nullptr);

	/**
	 * @brief <c>WNDCLASS</c>와의 복사 생성자입니다.
	 */
	CWndClass(const WNDCLASS &wc);
	/**
	 * @brief <c>WNDCLASSEX</c>와의 복사 생성자입니다.
	 */
	CWndClass(const WNDCLASSEX &wc);

	/**
	 * @brief <c>LPCTSTR</c> 로의 캐스트 연산자입니다. 윈도우 클래스 이름을 리턴합니다.
	 * @return 윈도우 클래스 이름입니다.
	 */
	operator LPCTSTR() const;

	/**
	 * @brief 이 객체가 가지는 <c>WNDCLASSEX</c> 구조체를 얻어옵니다.
	 * @return 이 객체가 가지는 <c>WNDCLASSEX</c> 구조체입니다.
	 */
	const WNDCLASSEX &GetWndCls() const NOEXCEPT;
};

inline CWndClass::operator LPCTSTR() const
{
	return m_name.c_str();
}

inline const WNDCLASSEX &CWndClass::GetWndCls() const NOEXCEPT
{
	return m_wc;
}

END_IKHWINLIB2()
