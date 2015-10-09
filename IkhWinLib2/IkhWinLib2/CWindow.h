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

#include "CMsgTarget.h"
#include "FnEvent.h"

BEGIN_IKHWINLIB2()

/**
 * @brief 창 하나를 나타내는 클래스입니다.
 */
class CWindow : public CMsgTarget
{
	friend class CModalDialog;
protected:
	static void AssertCreation(CWindow *pWnd);

	static void HookCreatingWindow(CWindow *pWnd);
	static const LPCTSTR CntStr_WindowPtrProp;
	static void CleanHookCreatingWindow(CWindow *pWnd);
private:
	// CALLBACK 함수
	static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	WNDPROC m_OldProc;

public:
	/**
	 * @brief 기본 생성자입니다.
	 */
	CWindow();
	/**
	 * @brief 생성자입니다. 이 객체가 인수로 주어진 창에 부착됩니다.
	 * @param[in] hWnd 부착할 창 핸들입니다.
	 * @see Attach
	 */
	explicit CWindow(HWND hWnd);

	/**
	 * @brief 파괴자입니다.
	 */
	virtual ~CWindow() NOEXCEPT;

	/**
	 * @brief 지정한 창에 이 객체를 부착합니다.
	 * @param[in] hWnd 부착할 창 핸들입니다.
	 * @exception CInvalidOperationError 창이 이미 부착되어 있습니다.
	 */
	void Attach(HWND hWnd);
	/**
	 * @brief 부착을 해제합니다.
	 */
	void Detach();

	/**
	 * @brief 창을 생성합니다. 인수의 의미는 <c>CreateWindowEx</c> API 함수와 같습니다.
	 * @example CWindow_CreateEx.cpp
	 */
	void CreateEx(DWORD dwExStyle,
		LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL, HMENU hMenu = NULL);
	/**
	* @brief 창을 생성합니다. 인수의 의미는 <c>CreateWindowEx</c> API 함수와 같습니다.
	* @example CWindow_CreateEx.cpp
	*/
	void CreateEx(DWORD dwExStyle,
		LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
		int x, int y,
		int nWidth, int nHeight,
		HWND hWndParent, int id);
	/**
	 * @brief 창을 생성합니다. 인수의 의미는 <c>CreateWindow</c> API 함수와 같습니다.
	 * @example CWindow_Create.cpp
	 */
	void Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL, HMENU hMenu = NULL);
	/**
	* @brief 창을 생성합니다. 인수의 의미는 <c>CreateWindowEx</c> API 함수와 같습니다.
	* @example CWindow_CreateEx.cpp
	*/
	void Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
		int x, int y,
		int nWidth, int nHeight,
		HWND hWndParent, int id);

	/**
	 * @brief 이 창을 전체 화면으로 만들거나 전체 화면을 해제합니다.
	 * @param[in] bFullscreen 이 값이 true이면 전체 화면으로 만들고, false이면 전체 화면을 해제합니다.
	 * @exception CChangeDisplaySettingsError <c>ChangeDisplaySettings</c> API 함수가 실패하였습니다.
	 */
	void SetFullscreen(bool bFullscreen = true);

	/**
	* @brief 창 핸들을 반환합니다.
	*/
	HWND GetHWND() const NOEXCEPT;
	/**
	 * @brief HWND로의 캐스트 연산자입니다. 창 핸들을 반환합니다.
	 */
	operator HWND() const NOEXCEPT;

	/**
	 * @brief 이 창에 메시지를 보냅니다. <c>SendMessage</c> API 함수를 사용합니다. 이 함수는 재정의될 수 없습니다.
	 * @see CMsgTarget::SendMsg
	 */
	virtual LRESULT SendMsg(UINT iMessage, WPARAM wParam, LPARAM lParam) override final;

	virtual boost::future<LRESULT> PostMsgEx(UINT iMessage, WPARAM wParam, LPARAM lParam) override final;

	/**
	 * @brief 이 창에 메시지를 붙힙니다. <c>PostMessage</c> API 함수를 사용합니다. 이 함수는 재정의될 수 없습니다.
	 * @exception CPostMsgError 메시지를 붙히지 못했습니다.
	 */
	void PostMsg(UINT iMessage, WPARAM wParam, LPARAM lParam);

	virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) override;

private:
	/**
	 * @brief 이 창이 WndProc에서 메시지를 받기 전에 호출됩니다.
	 * @param[in] iMessage 받은 메시지입니다.
	 * @param[in] wParam 메시지의 <c>WPARAM</c>입니다.
	 * @param[in] lParam 메시지의 <c>LPARAM</c>입니다.
	 * @param[out] result 반환값이 false인 경우, 메시지를 처리한 결과입니다.
	 * @return true인 경우 메시지가 정상적으로 전달됩니다. false인 경우 <c>result</c> 인수로 리턴된 값을 리턴합니다.
	 */
	virtual bool PreMessage(UINT iMessage, WPARAM wParam, LPARAM lParam, LRESULT &result);
	/**
	 * @brief 이 창이 만들어지기 전에 호출됩니다. 이 가상 함수는 WM_NCCREATE 메시지가 처리되기 전에 호출됩니다.
	 * @param[in, out] lpcs <c>CREATESTRUCT</c> 구조체의 포인터입니다. 원할 경우 이 구조체를 변경할 수 있습니다.
	 */
	virtual void PreNcCreate(LPCREATESTRUCT lpcs);
	/**
	 * @brief 창이 만들어진 후 호출됩니다. 이 가상 함수는 WM_CREATE 메시지를 처리한 후 호출됩니다.
	 */
	virtual void PostCreate();
	/**
	 * @brief 창이 모두 파괴된 후 호출됩니다. 이 가상 함수는 WM_NCDESTROY 메시지를 처리한 후 호출됩니다.
	 */
	virtual void PostNcDestroy();

public:
	// for CForm
	FnEvent<CWindow, void(CWindow *)> evtPosSizeChanged;
	FnEvent<CWindow, void(CWindow *)> evtDestroy;
};

inline CWindow::CWindow() : m_hWnd(NULL), m_OldProc(NULL) { }
inline CWindow::CWindow(HWND hWnd) : m_hWnd(NULL), m_OldProc(NULL) { Attach(hWnd); }

inline void CWindow::CreateEx(DWORD dwExStyle,
	LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, HWND hWndParent, int id)
{
	assert(this != nullptr);
	assert(id >= 0);
	CreateEx(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, (HMENU)(uintptr_t)id);
}
inline void CWindow::Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
	assert(this != nullptr);
	CreateEx(0, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu);
}
inline void CWindow::Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, HWND hWndParent, int id)
{
	assert(this != nullptr);
	CreateEx(0, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, id);
}

inline HWND CWindow::GetHWND() const NOEXCEPT
	{ assert(this != nullptr);  return m_hWnd; }
inline CWindow::operator HWND() const NOEXCEPT
	{ return GetHWND(); }

#ifndef _DEBUG
inline void CWindow::AssertCreation(CWindow *pWnd) { }
#endif

END_IKHWINLIB2()
