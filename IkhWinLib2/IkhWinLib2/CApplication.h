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

#include "CWin32Thread.h"

#include "FnEvent.h"
#include "CSystemError.h"

int main();
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

BEGIN_IKHWINLIB2()

/**
 * @brief ���α׷��� ��Ÿ���� �߻� Ŭ�����Դϴ�.
 * @example CApplication.cpp
 */
class CApplication : public CWin32Thread
{
	friend int ::main();
	friend int APIENTRY ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	friend class CMsgTarget;
	friend CWin32Thread;

private:
	static std::unique_ptr<CApplication> ms_pApp;
	static int IWLMain();
public:
	/**
	 * @brief ���� ���α׷��� @ref CApplication ��ü�� ���ɴϴ�.
	 * @brief IWLGetApp
	 */
	static CApplication *GetApp() NOEXCEPT;

private:
	static DWORD ms_TlsModeless;

public:
	/**
	 * @brief �������Դϴ�.
	 * @param[in] bCollectOnIdle �� ���� true�̸� ���̵� Ÿ�ӿ� �����⸦ ���� �����մϴ�.
	 * @see OnIdle
	 */
	explicit CApplication(bool bCollectOnIdle = true) NOEXCEPT;

	~CApplication() NOEXCEPT;

private:
	/**
	 * @brief ���α׷��� ���� ������ �Ǵ� ���� �Լ��Դϴ�.
	 * @return ���α׷��� ���� �ڵ��Դϴ�.
	 */
	virtual int Main(int argc, TCHAR *argv[]) = 0;
};

inline CApplication *CApplication::GetApp() NOEXCEPT
{
	return ms_pApp.get();
}
/**
 * @brief ���� ���α׷��� @ref CApplication ��ü�� ���ɴϴ�.
 * @brief CApplication::GetApp
 */
inline CApplication *IWLGetApp() NOEXCEPT
{
	return CApplication::GetApp();
}

extern "C" CApplication *CreateApplication();
#define IKHWINLIB2_APP_CLS(cls) \
	extern "C" CApplication *CreateApplication() \
	{ \
		return new cls; \
	} \

END_IKHWINLIB2()
