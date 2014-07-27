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

#include "CMyWindow.h"

#include <IkhWinLib2/CWndClass.h>
#include <IkhWinLib2/CApplication.h>

#include <gl/gl.h>

BEGIN_MSGMAP(CMyWindow, CIdleOpenGLWnd)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_LBUTTONDOWN(OnLButtonDown)
	MSGMAP_WM_LBUTTONDBLCLK(OnLButtonDown)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CMyWindow, CIdleOpenGLWnd)

void CMyWindow::Create()
{
	const DWORD style = WS_POPUP | COpenGLWnd::WS_STYLE;

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	CWindow::CreateEx(
		WS_EX_APPWINDOW,
		CWndClass(nullptr, COpenGLWnd::CS_STYLE | CS_DBLCLKS),
		L"OpenGL IWL", style,
		0, 0, width, height
		);
	SetFullscreen();
}

BOOL CMyWindow::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CIdleOpenGLWnd, lpcs))
		return FALSE;

	m_cbx.Create(WS_BORDER | WS_VISIBLE | WS_CHILD | CBS_DROPDOWN, 0, 50, 100, 30, 0, *this);
	ComboBox_AddString(m_cbx, L"asdf");

	IWLGetApp()->evtIdle += m_efpIdle.set(std::bind(&CMyWindow::OnIdle, this));

	return TRUE;
}

class TestGCFeature : public CObject
{
public:
	TestGCFeature()
	{
		OutputDebugString(
			(std::to_wstring(GetTickCount()) + L" >> TestGCFeature::TestGCFeature()\n").c_str()
			);
	}
	~TestGCFeature()
	{
		OutputDebugString(
			(std::to_wstring(GetTickCount()) + L" >> TestGCFeature::~TestGCFeature()\n").c_str()
			);
	}
};
void CMyWindow::OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyflags)
{
	auto fu = boost::async(
		WrapGCThread([this, fDoubleClick] {
			GCPtr<TestGCFeature> *test = GCNew<TestGCFeature>();

			Sleep(3000);
			MessageBox(*this, L"메시지!", L"박스?", MB_OK | MB_ICONINFORMATION);

			if (fDoubleClick)
				IWLGetApp()->Post(std::bind(PostQuitMessage, 0));
		}));
}

void CMyWindow::OnDestroy()
{
	SetFullscreen(false);
	PostQuitMessage(0);

	MSG_FORWARD_WM_DESTROY(COpenGLWnd);
}

void CMyWindow::OnIdle()
{
	static float rotate = 0.0f;
	static DWORD prev = GetTickCount();
	DWORD time, span;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_CW);
	glEnable(GL_BLEND);
	glEnable(GL_POLYGON_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
	{
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(0.0f, 1.0f, 0.8f);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(-0.7f, -0.87f, 0.8f);
		glColor3f(0.3f, 0.3f, 0.3f);
		glVertex3f(0.7f, -0.87f, 0.8f);
	}
	glEnd();

	glRotatef(rotate, 0.8f, 1, 0.5f);

	time = GetTickCount();
	span = time - prev;
	if (span >= 100)
	{
		rotate += 0.0125f * (span / 100);
		prev += span % 100;
	}

	struct Vertex
	{
		float vertex[3];
		float color[4];
	};
	static Vertex data[] = {
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 0.6f } },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 0.6f } },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 1.0f, 0.6f } },
		{ { -0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f, 0.6f } },
		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 0.6f } },
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 0.6f } },
		{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.6f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 0.0f, 0.6f } }
	};
	static int index[][3] = {
		{ 3, 1, 0 },
		{ 2, 1, 3 },
		{ 0, 5, 4 },
		{ 1, 5, 0 },
		{ 3, 4, 7 },
		{ 0, 4, 3 },
		{ 1, 6, 5 },
		{ 2, 6, 1 },
		{ 2, 7, 6 },
		{ 3, 7, 2 },
		{ 6, 4, 5 },
		{ 7, 4, 6 }
	};

	for (int *ar : index)
	{
		glBegin(GL_TRIANGLES);
		{
			for (int *p = ar; p < ar + 3; p++)
			{
				glColor4fv(data[*p].color);
				glVertex3fv(data[*p].vertex);
			}
		}
		glEnd();
	}

	SwapBuf();

	TCHAR buf[1024];
	wsprintf(buf, L"FPS : %u", GetFPS());

	HDC hdc = GetDC(*this);
	TextOut(hdc, 0, 0, buf);
	ReleaseDC(*this, hdc);

	unsigned fps = CalcFPS();
	//if (fps >= 300)
	//	Sleep(1);
}
