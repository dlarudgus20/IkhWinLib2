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

#include "stdafx.h"
#include "CRendererCtrl.h"
#include "CMyApp.h"

#include <IkhWinLib2/CWndClass.h>

#include "SphereManager.h"
#include "Camera.h"

BEGIN_MSGMAP(CRendererCtrl, CIdleOpenGLWnd)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_LBUTTONDOWN(OnLButtonDown)
	MSGMAP_WM_LBUTTONDBLCLK(OnLButtonDown)
	MSGMAP_WM_RBUTTONDOWN(OnRButtonDown)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CRendererCtrl, CIdleOpenGLWnd)

void CRendererCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight, int id, HWND hWndParent)
{
	CWindow::CreateEx(
		dwExStyle,
		CWndClass(nullptr, COpenGLWnd::CS_STYLE | CS_DBLCLKS, nullptr),
		nullptr, dwStyle | COpenGLWnd::WS_STYLE,
		x, y, nWidth, nHeight, hWndParent, (HMENU)id
		);
}

BOOL CRendererCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CIdleOpenGLWnd, lpcs))
		return FALSE;

	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, L"glew 초기화에 실패했습니다.", L"에러", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if (!GLEW_VERSION_2_0)
	{
		MessageBox(NULL, L"이 프로그램은 opengl 2.0 이상을 요구합니다.", L"에러", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	m_pShader = std::make_unique<Shader>();

	IWLGetApp()->evtIdle += m_efpIdle.set(std::bind(&CRendererCtrl::OnIdle, this));

	m_InfoTextCtrl.Create(WS_CHILD /*| WS_VISIBLE*/ | WS_BORDER, 0, 0, 100, 100, 1, *this);

	// Test Sphere
	m_pSphereManager->AddSphere({ { -150, 0, 0 }, 100, 600000, { 1, 0, 0, 1 }, { 0, -22.31897804112007, 0 } });
	m_pSphereManager->AddSphere({ { 150, 0, 0 }, 100, 600000, { 1, 0, 0, 1 }, { 0, 22.31897804112007, 0 } });
	//m_pSphereManager->AddSphere({ { 0, 0, 0 }, 125, 1200000, { 1, 0, 0, 1 } });
	m_pSphereManager->AddSphere({ { 1000, 0, 0 }, 75, 30, { 0, 1, 0, 0.8 }, { 0, 24.449215467167857, 0 } });
	m_pSphereManager->AddSphere({ { 2000, 0, 0 }, 75, 20, { 0.3, 0.3, 1, 0.8 }, { 0, 12.28820605152542, 0 } });
	m_pSphereManager->AddSphere({ { 3200, 0, 0 }, 75, 8, { 1, 1, 1, 0.8 }, { 0, 12, 0 } });

	//m_pSphereManager->AddSphere({ { -75, 0, 0 }, 75, 30, { 1, 0, 0, 0.8 } });
	//m_pSphereManager->AddSphere({ { 150, 0, 0 }, 150, 600000, { 1, 0, 0, 0.8 }, { -50, 0, 0 } });

	return TRUE;
}

void CRendererCtrl::OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyflags)
{
	if (!fDoubleClick)
	{

	}
	else
	{

	}
}

void CRendererCtrl::OnRButtonDown(BOOL fDoubleClick, int x, int y, UINT keyflags)
{

}

void CRendererCtrl::OnDestroy()
{
	m_pSphereManager = nullptr;
	m_pCamera = nullptr;
	MSG_FORWARD_WM_DESTROY(CIdleOpenGLWnd);
}

void CRendererCtrl::OnIdle()
{
	static float rotate = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_CW);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_pCamera->Apply();

	m_pSphereManager->UpdateIfExpired();
	m_pSphereManager->Render();

	SwapBuf();

	CalcFPS();
}
