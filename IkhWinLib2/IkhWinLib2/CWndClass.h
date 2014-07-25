// Copyright (c) 2014, �Ӱ���
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
 * @brief CWndClass Ŭ����
 */

#pragma once

#include "CObject.h"

BEGIN_IKHWINLIB2()

/**
 * @brief <c>WNDCLASSEX</c> ����ü�� �����ϴ� Ŭ�����Դϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�.
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
	 * @brief �������Դϴ�.
	 * @param[in] lpszClassName ������ Ŭ������ �̸��Դϴ�. <c>nullptr</c>�� ��� ���̺귯���� �˾Ƽ� �����մϴ�.
	 * @param[in] style ������ Ŭ������ ��Ÿ���Դϴ�.
	 * @param[in] hbrBackground ��� �귯���Դϴ�.
	 * @param[in] lpszMenuName �޴��Դϴ�.
	 * @param[in] hCursor Ŀ���Դϴ�.
	 * @param[in] hIcon �������Դϴ�.
	 * @param[in] hIconSm ���� �������Դϴ�.
	 */
	explicit CWndClass(LPCTSTR lpszClassName = nullptr,
		UINT style = CS_HREDRAW | CS_VREDRAW,
		HBRUSH hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		LPCTSTR lpszMenuName = nullptr,
		HCURSOR hCursor = nullptr, HICON hIcon = nullptr, HICON hIconSm = nullptr);

	/**
	 * @brief <c>WNDCLASS</c>���� ���� �������Դϴ�.
	 */
	CWndClass(const WNDCLASS &wc);
	/**
	 * @brief <c>WNDCLASSEX</c>���� ���� �������Դϴ�.
	 */
	CWndClass(const WNDCLASSEX &wc);

	/**
	 * @brief <c>LPCTSTR</c> ���� ĳ��Ʈ �������Դϴ�. ������ Ŭ���� �̸��� �����մϴ�.
	 * @return ������ Ŭ���� �̸��Դϴ�.
	 */
	operator LPCTSTR() const;

	/**
	 * @brief �� ��ü�� ������ <c>WNDCLASSEX</c> ����ü�� ���ɴϴ�.
	 * @return �� ��ü�� ������ <c>WNDCLASSEX</c> ����ü�Դϴ�.
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
