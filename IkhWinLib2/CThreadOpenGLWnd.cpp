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
#include "IkhWinLib2/CThreadOpenGLWnd.h"
#include "IkhWinLib2/CSystemError.h"

BEGIN_IKHWINLIB2()

BEGIN_MSGMAP(CThreadOpenGLWnd, CWindow)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_SIZE(OnSize)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CThreadOpenGLWnd, CWindow)

CThreadOpenGLWnd::CThreadOpenGLWnd()
	: m_hThread(nullptr)
{
}

BOOL CThreadOpenGLWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(COpenGLWnd, lpcs))
		return FALSE;

	return TRUE;
}

void CThreadOpenGLWnd::OnSize(UINT state, int cx, int cy)
{
	MSG_FORWARD_WM_SIZE(COpenGLWnd, state, cx, cy);

	if (m_hThread != nullptr)
		SetEvent(m_hUpdateEvent);
}

void CThreadOpenGLWnd::OnDestroy()
{
	if (m_hThread != nullptr)
	{
		SetEvent(m_hStopEvent);
		WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = nullptr;
	}

	MSG_FORWARD_WM_DESTROY(COpenGLWnd);
}

void CThreadOpenGLWnd::implThreadRun()
{
	unsigned id;

	m_hUpdateEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_hUpdateEvent != nullptr)
	{
		m_hStopEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_hUpdateEvent != nullptr)
		{
			m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, &id);
			if (m_hThread != nullptr)
			{
				return;
			}
			else
			{
				CloseHandle(m_hUpdateEvent); m_hUpdateEvent = nullptr;
				CloseHandle(m_hStopEvent); m_hStopEvent = nullptr;
				throw CThreadCreationError();
			}
		}
		CloseHandle(m_hStopEvent);
		m_hStopEvent = nullptr;
	}
	CloseHandle(m_hUpdateEvent);
	m_hUpdateEvent = nullptr;
	throw CEventCreationError();
}

unsigned __stdcall CThreadOpenGLWnd::ThreadFunc(void *lpParam)
{
	CThreadOpenGLWnd *thiz = (CThreadOpenGLWnd *)lpParam;

	BOOST_SCOPE_EXIT((thiz))
	{
		CloseHandle(thiz->m_hStopEvent);
		thiz->m_hUpdateEvent = nullptr;
		CloseHandle(thiz->m_hUpdateEvent);
		thiz->m_hUpdateEvent = nullptr;
		thiz->m_hThread = nullptr;
	}
	BOOST_SCOPE_EXIT_END

	GLInitor initor(thiz);

	thiz->m_startup(*thiz);

	do
	{
		if (WaitForSingleObject(thiz->m_hUpdateEvent, 0) == WAIT_OBJECT_0)
		{
			thiz->m_onupdate(*thiz);
		}

		thiz->m_ondraw(*thiz);

	} while (WaitForSingleObject(thiz->m_hStopEvent, 0) == WAIT_TIMEOUT);

	return 0;
}

END_IKHWINLIB2()
