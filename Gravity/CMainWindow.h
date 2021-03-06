// Copyright (c) 2014, �Ӱ���
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

#include <IkhWinLib2/CForm.h>
#include <IkhWinLib2/CEditCtrl.h>
#include <IkhWinLib2/FnEvent.h>

#include "CCmdEditCtrl.h"
#include "CRendererCtrl.h"
#include "SphereManager.h"
#include "Camera.h"
#include "Scripter.h"
#include "Projection.h"

class CMainWindow final : public CForm, public virtual IScriptHost
{
	DECLARE_MSGMAP();
public:
	void Create();

private:
	CRendererCtrl m_RendererCtrl;
	CEditCtrl m_CmdListEdit;
	CCmdEditCtrl m_CmdInputEdit;

	EventFnPtr<void(CCmdEditCtrl *, const std::wstring &)> m_efpCmdInput;

	SphereManager m_SphereManager;
	Camera m_Camera;
	Projection m_projection;

	Scripter m_Scripter;

	bool m_bForceScroll = true;

public:
	CMainWindow() : m_RendererCtrl(&m_SphereManager, &m_Camera, &m_projection), m_Scripter(this) { }

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnTimer(UINT id);
	void OnDestroy();

	void OnCmdInput(CCmdEditCtrl *pCtrl, const std::wstring &input);

private:

public:
	// IScriptHost
	virtual void WriteLine(const std::wstring &str) override;
	virtual void WriteMultiLine(const std::wstring &str) override;
	virtual void UpdateTitle() override;
	virtual void ForceScroll(bool bScroll) override;
	virtual void UseOrtho(bool bOrtho) override;
	virtual SphereManager *GetSphereManager() override;
};
