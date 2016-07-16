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
#include "IkhWinLib2/COpenGLWnd.h"

BEGIN_IKHWINLIB2()

BEGIN_MSGMAP(COpenGLWnd, CWindow)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_SIZE(OnSize)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(COpenGLWnd, CWindow)

COpenGLWnd::COpenGLWnd(BYTE cDepthBits /* = 24 */, BYTE cStencilBits /* = 8 */)
	: m_cDepthBits(cDepthBits), m_cStencilBits(cStencilBits)
	, m_fps(0), m_hdc(nullptr)
	, m_count(0), m_gap(0), m_PrevTime(0), m_bCalced(false)
{
}

BOOL COpenGLWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CWindow, lpcs))
		return FALSE;

	PIXELFORMATDESCRIPTOR pfd;
	int nPixelFormat;

	m_hdc = GetDC(*this);

	RECT rt;
	GetClientRect(*this, &rt);
	m_cx = rt.right;
	m_cy = rt.bottom;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = m_cDepthBits;
	pfd.cStencilBits = m_cStencilBits;
	pfd.iLayerType = PFD_MAIN_PLANE;

	nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);
	if (nPixelFormat != 0)
	{
		if (SetPixelFormat(m_hdc, nPixelFormat, &pfd))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void COpenGLWnd::OnSize(UINT state, int cx, int cy)
{
	MSG_FORWARD_WM_SIZE(CWindow, state, cx, cy);

	m_cx = cx;
	m_cy = cy;
}

void COpenGLWnd::OnDestroy()
{
	wglMakeCurrent(m_hdc, nullptr);

	MSG_FORWARD_WM_DESTROY(CWindow);
}

HGLRC COpenGLWnd::InitGL()
{
	assert(this != nullptr);

	HGLRC hrc = wglCreateContext(m_hdc);
	wglMakeCurrent(m_hdc, hrc);

	return hrc;
}

void COpenGLWnd::DestroyGL(HGLRC hrc)
{
	assert(this != nullptr);

	wglDeleteContext(hrc);
}

void COpenGLWnd::SizeChangeProc()
{
	glViewport(0, 0, m_cx, m_cy);
}

unsigned COpenGLWnd::CalcFPS()
{
	assert(this != nullptr);

	DWORD now = GetTickCount();

	if (!m_bCalced)
	{
		m_PrevTime = now;
		m_bCalced = true;
	}

	m_gap = now - m_PrevTime;

	m_count++;

	if (m_gap >= 1000)
	{
		m_PrevTime = now;

		m_gap -= 1000;
		m_fps = m_count;
		m_count = 0;
	}

	return m_fps;
}

END_IKHWINLIB2()
