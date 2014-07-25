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
#include "IkhWinLib2/CIdleOpenGLWnd.h"

BEGIN_IKHWINLIB2()

BEGIN_MSGMAP(CIdleOpenGLWnd, COpenGLWnd)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_SIZE(OnSize)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CIdleOpenGLWnd, COpenGLWnd)

BOOL CIdleOpenGLWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(COpenGLWnd, lpcs))
		return FALSE;

	m_hrc = InitGL();
	return TRUE;
}

void CIdleOpenGLWnd::OnSize(UINT state, int cx, int cy)
{
	MSG_FORWARD_WM_SIZE(COpenGLWnd, state, cx, cy);
	SizeChangeProc();
}

void CIdleOpenGLWnd::OnDestroy()
{
	DestroyGL(m_hrc);

	MSG_FORWARD_WM_DESTROY(COpenGLWnd);
}

END_IKHWINLIB2()
