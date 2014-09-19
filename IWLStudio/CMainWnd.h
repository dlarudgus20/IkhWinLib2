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

#include <IkhWinLib2/CForm.h>
#include <IkhWinLib2/CListBoxCtrl.h>
#include <IkhWinLib2/CTreeViewCtrl.h>
#include <IkhWinLib2/CPropViewCtrl.h>
using namespace IkhProgram::IkhWinLib2;

#include "CDesignerCtrl.h"
#include "CProject.h"

class CMainWnd final : public CForm
{
	DECLARE_MSGMAP();
public:
	void Create();

private:
	enum
	{
		TOOLLIST_ID,
		DESIGNERCTRL_ID,
		PROJTREE_ID,
		PROPVIEW_ID,
	};
	CListBoxCtrl m_ToolList;
	CDesignerCtrl m_DesignerCtrl;
	CTreeViewCtrl m_ProjTree;
	CPropViewCtrl m_PropView;

	std::unique_ptr<CProject> m_pProject;

public:
	CMainWnd();

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnToolListSelChange(int id, HWND hCtl, UINT codeNotify);
	void OnClose();
	void OnDestroy();

	void OnFileProjNew(int id, HWND hCtl, UINT codeNotify);
	void OnFileProjOpen(int id, HWND hCtl, UINT codeNotify);
	void OnFileSave(int id, HWND hCtl, UINT codeNotify);
	void OnFileAllSave(int id, HWND hCtl, UINT codeNotify);
	void OnFileExit(int id, HWND hCtl, UINT codeNotify);
};
