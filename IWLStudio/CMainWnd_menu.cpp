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

#include <IkhWinLib2/CFileDialog.h>

#include "CMainWnd.h"

void CMainWnd::OnFileProjNew(int id, HWND hCtl, UINT codeNotify)
{
	CFileDialog dialog(false, L"IWL Studio 프로젝트 파일\0*.iwlproj\0", L"iwlproj", L"새 프로젝트");

	if (dialog.DoModal(*this) == IDOK)
	{
		m_pProject.reset(new CProject(m_ProjTree, dialog.GetFileName()));
	}
}

void CMainWnd::OnFileProjOpen(int id, HWND hCtl, UINT codeNotify)
{
	CFileDialog dialog(true, L"IWL Studio 프로젝트 파일\0*.iwlproj\0", L"iwlproj", L"프로젝트 열기");

	if (dialog.DoModal(*this) == IDOK)
	{
		try
		{
			auto sp = std::make_unique<CProject>(m_ProjTree, dialog.GetFileName());
			sp->Load();

			m_pProject = std::move(sp);
		}
		catch (json_parser::json_parser_error &e)
		{
			MessageBox(*this, L"프로젝트 파일 파싱에 실패하였습니다.", L"에러", MB_OK | MB_ICONERROR);
			OutputDebugStringA((std::string("error on opening projet file : ") + e.what() + "\n").c_str());
		}
	}
}

void CMainWnd::OnFileSave(int id, HWND hCtl, UINT codeNotify)
{
	try
	{
		if (m_pProject)
		{
			m_pProject->Save();
		}
	}
	catch (json_parser::json_parser_error &e)
	{
		MessageBox(*this, L"프로젝트 파일 저장에 실패하였습니다.", L"에러", MB_OK | MB_ICONERROR);
		OutputDebugStringA((std::string("error on saving projet file : ") + e.what() + "\n").c_str());
	}
}

void CMainWnd::OnFileAllSave(int id, HWND hCtl, UINT codeNotify)
{
	OutputDebugStringA("not impl: CMainWnd::OnFileAllSave()\n");
}

void CMainWnd::OnFileExit(int id, HWND hCtl, UINT codeNotify)
{
	SendMessage(*this, WM_CLOSE, 0, 0);
}
