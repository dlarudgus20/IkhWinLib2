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
#include "IkhWinLib2/CWin32Thread.h"
#include "IkhWinLib2/CSystemError.h"
#include "IkhWinLib2/CApplication.h"

BEGIN_IKHWINLIB2()

UINT CWin32Thread::ms_WM_POST;

void CWin32Thread::StaticCtor() NOEXCEPT
{
	ms_WM_POST = RegisterWindowMessage((
		L"IkhWinLib2 CWin32Thread StaticCtor ms_WM_POST " + std::to_wstring((intptr_t)GetModuleHandle(NULL))
		).c_str());
}

CWin32Thread::~CWin32Thread() NOEXCEPT
{
	if (m_hThread != nullptr)
	{
		if (WaitForSingleObject(m_hThread, 0) != WAIT_OBJECT_0)
			assert(!"CValidWin32ThreadDestroyError on CWin32Thread::~CWin32Thread");

		CloseHandle(m_hThread);
	}
}

void CWin32Thread::SetModeless(HWND hModeless) NOEXCEPT
{
	assert(this != nullptr);
	TlsSetValue(CApplication::ms_TlsModeless, (LPVOID)hModeless);
}

namespace
{
	typedef std::function<void()> FromFunctionFunc;

	unsigned __stdcall FromFunctionThrd(void *lpParam)
	{
		std::unique_ptr<FromFunctionFunc> pf { static_cast<FromFunctionFunc *>(lpParam) };
		(*pf)();
		return 0;
	}
}
CWin32Thread CWin32Thread::FromFunctionImpl(const std::function<void()> &f)
{
	FromFunctionFunc *pf { new FromFunctionFunc(f) };
	try
	{
		return CWin32Thread(FromFunctionThrd, pf);
	}
	catch (CThreadCreationError &)
	{
		delete pf;
		throw;
	}
}

PollReturn CWin32Thread::Poll(MSG &msg, bool bCallOnIdle /* = true*/) NOEXCEPT
{
#ifdef IKHWINLIB2_ENABLE_CATCH_DURING_POLL
	try
#endif
	{
		assert(this != nullptr);
	
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message != WM_QUIT)
			{
				if (msg.hwnd == nullptr)
				{
					if (msg.message == ms_WM_POST)
					{
						auto *pf = reinterpret_cast<PostImplStructBase *>(msg.lParam);
						pf->run();
					}
					else
					{
						OnGlobalMsg(msg);
					}
	
					return PollReturn::GlobalMessage;
				}
				else
				{
					HWND hModeless = (HWND)TlsGetValue(CApplication::ms_TlsModeless);
					if (!IsWindow(hModeless) || !IsDialogMessage(hModeless, &msg))
					{
						if (m_hAccel == nullptr || !TranslateAccelerator(GetActiveWindow(), m_hAccel, &msg))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
					}
				}
			}
			return PollReturn::NormalMessage;
		}
		else
		{
			if (bCallOnIdle)
				OnIdle();
			return PollReturn::NoMessage;
		}
	}
#ifdef IKHWINLIB2_ENABLE_CATCH_DURING_POLL
	catch (...)
	{
		OnUnexpectedException(std::current_exception());
		while (1) { }	// cannot reach here
	}
#endif
}
void CWin32Thread::OnUnexpectedException(const std::exception_ptr &eptr) NOEXCEPT
{
	try
	{
		std::rethrow_exception(eptr);
	}
	catch (std::exception &e)
	{
		OutputDebugString(L"[IkhWinLib2] unexpected exception (std::exception) : ");
		OutputDebugStringA(e.what());
		OutputDebugString(L"\n");
	}
	catch (CException &e)
	{
		OutputDebugString(L"[IkhWinLib2] unexpected exception (CException) : ");
		OutputDebugString(e.Message().c_str());
		OutputDebugString(L"\n");
	}
	catch (...)
	{
		OutputDebugString(L"[IkhWinLib2] unexpected exception (unknown exception)\n");
	}
	abort();
}

WPARAM CWin32Thread::Run(HACCEL hAccel /*= nullptr*/) NOEXCEPT
{
	assert(this != nullptr);

	MSG msg;

	assert(m_hAccel == nullptr || hAccel == nullptr);
	if (m_hAccel == nullptr)
		m_hAccel = hAccel;

	while (1)
	{
		if (Poll(msg) != PollReturn::NoMessage)
		{
			if (msg.message == WM_QUIT)
				return msg.wParam;
		}
	}
}

void CWin32Thread::OnIdle()
{
	assert(this != nullptr);

	if (evtIdle.IsEmpty())
	{
		WaitMessage();
	}
	else
	{
		evtIdle();
	}
}

void CWin32Thread::OnGlobalMsg(const MSG &msg)
{
	assert(this != nullptr);
	evtGlobalMsg(msg);
}

END_IKHWINLIB2()
