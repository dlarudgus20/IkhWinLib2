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

#pragma once

#include "CMsgTarget.h"
#include "FnEvent.h"

BEGIN_IKHWINLIB2()

/**
 * @brief â �ϳ��� ��Ÿ���� Ŭ�����Դϴ�.
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
	// CALLBACK �Լ�
	static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hWnd;
	WNDPROC m_OldProc;

public:
	/**
	 * @brief �⺻ �������Դϴ�.
	 */
	CWindow();
	/**
	 * @brief �������Դϴ�. �� ��ü�� �μ��� �־��� â�� �����˴ϴ�.
	 * @param[in] hWnd ������ â �ڵ��Դϴ�.
	 * @see Attach
	 */
	explicit CWindow(HWND hWnd);

	/**
	 * @brief �ı����Դϴ�.
	 */
	virtual ~CWindow() NOEXCEPT;

	/**
	 * @brief ������ â�� �� ��ü�� �����մϴ�.
	 * @param[in] hWnd ������ â �ڵ��Դϴ�.
	 * @exception CInvalidOperationError â�� �̹� �����Ǿ� �ֽ��ϴ�.
	 */
	void Attach(HWND hWnd);
	/**
	 * @brief ������ �����մϴ�.
	 */
	void Detach();

	/**
	 * @brief â�� �����մϴ�. �μ��� �ǹ̴� <c>CreateWindowEx</c> API �Լ��� �����ϴ�.
	 * @example CWindow_CreateEx.cpp
	 */
	void CreateEx(DWORD dwExStyle,
		LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL, HMENU hMenu = NULL);
	/**
	* @brief â�� �����մϴ�. �μ��� �ǹ̴� <c>CreateWindowEx</c> API �Լ��� �����ϴ�.
	* @example CWindow_CreateEx.cpp
	*/
	void CreateEx(DWORD dwExStyle,
		LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
		int x, int y,
		int nWidth, int nHeight,
		HWND hWndParent, int id);
	/**
	 * @brief â�� �����մϴ�. �μ��� �ǹ̴� <c>CreateWindow</c> API �Լ��� �����ϴ�.
	 * @example CWindow_Create.cpp
	 */
	void Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = NULL, HMENU hMenu = NULL);
	/**
	* @brief â�� �����մϴ�. �μ��� �ǹ̴� <c>CreateWindowEx</c> API �Լ��� �����ϴ�.
	* @example CWindow_CreateEx.cpp
	*/
	void Create(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwStyle,
		int x, int y,
		int nWidth, int nHeight,
		HWND hWndParent, int id);

	/**
	 * @brief �� â�� ��ü ȭ������ ����ų� ��ü ȭ���� �����մϴ�.
	 * @param[in] bFullscreen �� ���� true�̸� ��ü ȭ������ �����, false�̸� ��ü ȭ���� �����մϴ�.
	 * @exception CChangeDisplaySettingsError <c>ChangeDisplaySettings</c> API �Լ��� �����Ͽ����ϴ�.
	 */
	void SetFullscreen(bool bFullscreen = true);

	/**
	* @brief â �ڵ��� ��ȯ�մϴ�.
	*/
	HWND GetHWND() const NOEXCEPT;
	/**
	 * @brief HWND���� ĳ��Ʈ �������Դϴ�. â �ڵ��� ��ȯ�մϴ�.
	 */
	operator HWND() const NOEXCEPT;

	/**
	 * @brief �� â�� �޽����� �����ϴ�. <c>SendMessage</c> API �Լ��� ����մϴ�. �� �Լ��� �����ǵ� �� �����ϴ�.
	 * @see CMsgTarget::SendMsg
	 */
	virtual LRESULT SendMsg(UINT iMessage, WPARAM wParam, LPARAM lParam) override final;

	virtual boost::future<LRESULT> PostMsgEx(UINT iMessage, WPARAM wParam, LPARAM lParam) override final;

	/**
	 * @brief �� â�� �޽����� �����ϴ�. <c>PostMessage</c> API �Լ��� ����մϴ�. �� �Լ��� �����ǵ� �� �����ϴ�.
	 * @exception CPostMsgError �޽����� ������ ���߽��ϴ�.
	 */
	void PostMsg(UINT iMessage, WPARAM wParam, LPARAM lParam);

	virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) override;

private:
	/**
	 * @brief �� â�� WndProc���� �޽����� �ޱ� ���� ȣ��˴ϴ�.
	 * @param[in] iMessage ���� �޽����Դϴ�.
	 * @param[in] wParam �޽����� <c>WPARAM</c>�Դϴ�.
	 * @param[in] lParam �޽����� <c>LPARAM</c>�Դϴ�.
	 * @param[out] result ��ȯ���� false�� ���, �޽����� ó���� ����Դϴ�.
	 * @return true�� ��� �޽����� ���������� ���޵˴ϴ�. false�� ��� <c>result</c> �μ��� ���ϵ� ���� �����մϴ�.
	 */
	virtual bool PreMessage(UINT iMessage, WPARAM wParam, LPARAM lParam, LRESULT &result);
	/**
	 * @brief �� â�� ��������� ���� ȣ��˴ϴ�. �� ���� �Լ��� WM_NCCREATE �޽����� ó���Ǳ� ���� ȣ��˴ϴ�.
	 * @param[in, out] lpcs <c>CREATESTRUCT</c> ����ü�� �������Դϴ�. ���� ��� �� ����ü�� ������ �� �ֽ��ϴ�.
	 */
	virtual void PreNcCreate(LPCREATESTRUCT lpcs);
	/**
	 * @brief â�� ������� �� ȣ��˴ϴ�. �� ���� �Լ��� WM_CREATE �޽����� ó���� �� ȣ��˴ϴ�.
	 */
	virtual void PostCreate();
	/**
	 * @brief â�� ��� �ı��� �� ȣ��˴ϴ�. �� ���� �Լ��� WM_NCDESTROY �޽����� ó���� �� ȣ��˴ϴ�.
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
