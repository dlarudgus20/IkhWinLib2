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

#pragma once

#include "COpenGLWnd.h"

BEGIN_IKHWINLIB2()

class CThreadOpenGLWnd : public COpenGLWnd
{
public:
	using ProcFunc = std::function<void(COpenGLWnd &)>;

	DECLARE_MSGMAP();
private:
	HANDLE m_hStopEvent, m_hUpdateEvent;
	HANDLE m_hThread;

	ProcFunc m_startup, m_ondraw, m_onupdate;

	static unsigned __stdcall ThreadFunc(void *lpParam);

public:
	static const DWORD CS_STYLE = CS_OWNDC;
	static const DWORD WS_STYLE = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	CThreadOpenGLWnd();

	template <typename F1, typename F2, typename F3>
	void ThreadRun(F1 startup, F2 ondraw, F3 onupdate);

	void RaiseUpdate();
	void RaiseStop();

	HANDLE GetThread() const;

private:
	void implThreadRun();

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnSize(UINT state, int cx, int cy);
	void OnDestroy();
};

template <typename F1, typename F2, typename F3>
void CThreadOpenGLWnd::ThreadRun(F1 startup, F2 ondraw, F3 onupdate)
{
	assert(this != nullptr);
	assert(m_hThread == nullptr);

	assert(startup != nullptr);
	assert(ondraw != nullptr);
	assert(onupdate != nullptr);

	m_startup = startup;
	m_ondraw = ondraw;
	m_onupdate = onupdate;

	implThreadRun();
}

inline void CThreadOpenGLWnd::RaiseUpdate()
{
	assert(this != nullptr);
	assert(m_hThread != nullptr);
	SetEvent(m_hUpdateEvent);
}

inline void CThreadOpenGLWnd::RaiseStop()
{
	assert(this != nullptr);
	assert(m_hThread != nullptr);
	SetEvent(m_hStopEvent);
}

inline HANDLE CThreadOpenGLWnd::GetThread() const
{
	assert(this != nullptr);
	return m_hThread;
}

END_IKHWINLIB2()
