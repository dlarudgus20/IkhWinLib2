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
#include <IkhWinLib2/IControl.h>
#include <IkhWinLib2/CListBoxCtrl.h>
using namespace IkhProgram::IkhWinLib2;

class CDesignerCtrl final : public CForm, public virtual IControl
{
	DECLARE_MSGMAP();
public:
	using IControl::Create;
	using IControl::CreateEx;

	virtual void CreateEx(DWORD dwExStyle, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight, int id, HWND hWndParent) override;

private:
	typedef std::shared_ptr<IControl> (*CtrlCtor)();
	static std::vector<CtrlCtor> m_vtCtrlCtor;
	CtrlCtor m_NowCtrlCtor;

	RECT m_SelRect;

public:
	CDesignerCtrl();

	void InitToolList(CListBoxCtrl &ToolList);
	void OnToolListSelChange(int idx);

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags);
	void OnMouseMove(int x, int y, UINT keyFlags);
	void OnLButtonUp(int x, int y, UINT keyFlags);
	void ChangeSelRect(int x, int y);
	void OnDestroy();
};
