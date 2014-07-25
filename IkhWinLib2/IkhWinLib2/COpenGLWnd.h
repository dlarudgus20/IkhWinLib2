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

#include "CWindow.h"
#include "NonCopyable.h"

BEGIN_IKHWINLIB2()

class COpenGLWnd : public CWindow
{
	DECLARE_MSGMAP();
private:
	unsigned m_fps;

	DWORD m_PrevTime;
	DWORD m_gap;
	DWORD m_count;
	bool m_bCalced;

	HDC m_hdc;

	int m_cx, m_cy;

public:
	static const DWORD CS_STYLE = CS_OWNDC;
	static const DWORD WS_STYLE = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	COpenGLWnd();

	HDC GetOwnDC();
	void SwapBuf();

	HGLRC InitGL();
	void DestroyGL(HGLRC hrc);
	struct GLInitor : private NonCopyableMovable
	{
		HGLRC m_hrc;
		COpenGLWnd *m_pWnd;
		explicit GLInitor(COpenGLWnd *pWnd) : m_pWnd(pWnd)
		{
			m_hrc = pWnd->InitGL();
		}
		~GLInitor()
		{
			m_pWnd->DestroyGL(m_hrc);
		}
	};

	void SizeChangeProc();

	unsigned GetFPS();

protected:
	unsigned CalcFPS();

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnSize(UINT state, int cx, int cy);
	void OnDestroy();
};

inline HDC COpenGLWnd::GetOwnDC()
{
	return m_hdc;
}

inline void COpenGLWnd::SwapBuf()
{
	SwapBuffers(m_hdc);
}

inline unsigned COpenGLWnd::GetFPS()
{
	return m_fps;
}

END_IKHWINLIB2()
