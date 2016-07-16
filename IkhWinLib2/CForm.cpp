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
#include "IkhWinLib2/CForm.h"
#include "IkhWinLib2/CLogicError.h"

BEGIN_IKHWINLIB2()

BEGIN_MSGMAP(CForm, CWindow)
	MSGMAP_WM_SIZE(OnSize)
END_MSGMAP(CForm, CWindow)

void CForm::OnSize(UINT state, int cx, int cy)
{
	_updateLayout();
}

CForm::Child CForm::AddChild(CWindow *pChild)
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	if (_findChildInfo(pChild) != m_ChildList.end())
		throw CInvalidChildError(L"pChild는 이미 이 폼의 차일드입니다.");

	HWND hParent = GetParent(*pChild);
	if (hParent != *this)
	{
		SetParent(*pChild, *this);
	}

	m_ChildList.push_back(
		std::make_shared<ChildInfo>(pChild, m_ChildList.end())
		);

	if (m_ChildList.size() >= 2)
	{
		auto newone = std::prev(m_ChildList.end());
		auto oldone = std::prev(newone);
		(*newone)->ZOrderPrev = oldone;
		(*oldone)->ZOrderNext = newone;
	}

	auto it = std::prev(m_ChildList.end());
	pChild->evtDestroy += m_ChildList.back()->efpDestroy.set(
		[this, it](CWindow *) { m_ChildList.erase(it); }
	);

	pChild->evtPosSizeChanged += m_ChildList.back()->efpSizeChanged.set(
		[this, it](CWindow *) { if (!m_bLayouting) ApplyMovedChild(*it); }
	);

	_updateLayout();

	return Child(it);
}

void CForm::RemoveChild(CForm::Child child)
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	m_ChildList.erase(child.m_it);

	_updateLayout();
}

CForm::Child CForm::FindChild(CWindow *pChild)
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	auto it = _findChildInfo(pChild);
	if (it == m_ChildList.end())
		throw CInvalidChildError();

	return Child(it);
}

void CForm::DockChild(CForm::Child child, boost::optional<CForm::Child> target,
	CForm::Dock dock, int gap)
{
	assert(this != NULL);
	assert(GetHWND() != NULL);
	assert(0 <= dock && dock < 4);

	auto &d = (*child.m_it)->Dock[dock];
	d.Gap = gap;
	if (target)
	{
		d.TargetKind = TARGET_CHILD;
		d.TargetChild = *target->m_it;
		d.TargetChild_it = target->m_it;
	}
	else
	{
		d.TargetKind = TARGET_FORM;
	}

	_updateLayout();
}

void CForm::MoveChild(CForm::Child child, RECT pos)
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	(*child.m_it)->PosRect = pos;

	RECT ClientRt;
	GetClientRect(*this, &ClientRt);

	std::set<std::shared_ptr<ChildInfo> > ModifiedList;
	RecursiveLayout(*child.m_it, ClientRt, ModifiedList);

	m_bLayouting = true;
	HDWP hDefer = BeginDeferWindowPos((int)(unsigned)ModifiedList.size());
	DeferMoveChild(hDefer, *child.m_it);
	for (const auto &sp : ModifiedList)
	{
		DeferMoveChild(hDefer, sp);
		sp->fProcessed = false;
	}
	EndDeferWindowPos(hDefer);
	m_bLayouting = false;
}

void CForm::PerformLayout()
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	RECT ClientRt;
	GetClientRect(*this, &ClientRt);

	std::set<std::shared_ptr<ChildInfo> > unused;
	for (const auto &sp : m_ChildList)
	{
		if (!sp->fProcessed)
		{
			RecursiveLayout(sp, ClientRt, unused);
			unused.clear();
		}
	}

	m_bLayouting = true;
	HDWP hDefer = BeginDeferWindowPos((int)(unsigned)m_ChildList.size());
	for (const auto &sp : m_ChildList)
	{
		DeferMoveChild(hDefer, sp);
		sp->fProcessed = false;
	}
	EndDeferWindowPos(hDefer);
	m_bLayouting = false;

	m_bLayoutRequired = false;
}
void CForm::DeferMoveChild(HDWP hDefer, const std::shared_ptr<CForm::ChildInfo> &sp)
{
	HWND hWndInsertAfter;
	if (sp->ZOrderPrev != m_ChildList.end())
		hWndInsertAfter = (*sp->ZOrderPrev)->pWnd->GetHWND();
	else
		hWndInsertAfter = NULL;

	DeferWindowPos(hDefer, *sp->pWnd, hWndInsertAfter,
		sp->PosRect.left, sp->PosRect.top,
		sp->PosRect.right - sp->PosRect.left, sp->PosRect.bottom - sp->PosRect.top,
		SWP_NOACTIVATE | (hWndInsertAfter ? 0 : SWP_NOZORDER));
}

void CForm::RecursiveLayout(const std::shared_ptr<CForm::ChildInfo> &pci, const RECT &ClientRt,
	std::set<std::shared_ptr<CForm::ChildInfo> > &ModifiedList)
{
	if (pci->fProcessed)
		return;
	pci->fProcessed = true;

	GetWindowRect(*pci->pWnd, &pci->PosRect);
	ScreenToClient(*this, (LPPOINT)&pci->PosRect.left);
	ScreenToClient(*this, (LPPOINT)&pci->PosRect.right);

	RECT &rt = pci->PosRect;

	bool left_modified = false;
	bool top_modified = false;
	bool right_modified = false;
	bool bottom_modified = false;

	// TARGET_CHILD
	if (pci->Dock[DockLeft].TargetKind == TARGET_CHILD)
	{
		auto sp = pci->Dock[DockLeft].TargetChild.lock();
		if (sp)
		{
			ModifiedList.insert(sp);
			RecursiveLayout(sp, ClientRt, ModifiedList);
			LONG delta = (sp->PosRect.right + pci->Dock[DockLeft].Gap) - rt.left;
			rt.left += delta;
			rt.right += delta;
			left_modified = true;
			right_modified = true;
		}
		else
		{
			pci->Dock[DockLeft].TargetKind = TARGET_NONE;
		}
	}
	if (pci->Dock[DockTop].TargetKind == TARGET_CHILD)
	{
		auto sp = pci->Dock[DockTop].TargetChild.lock();
		if (sp)
		{
			ModifiedList.insert(sp);
			RecursiveLayout(sp, ClientRt, ModifiedList);
			LONG delta = (sp->PosRect.bottom + pci->Dock[DockLeft].Gap) - rt.top;
			rt.top += delta;
			rt.bottom += delta;
			top_modified = true;
			bottom_modified = true;
		}
		else
		{
			pci->Dock[DockTop].TargetKind = TARGET_NONE;
		}
	}
	if (pci->Dock[DockRight].TargetKind == TARGET_CHILD)
	{
		auto sp = pci->Dock[DockRight].TargetChild.lock();
		if (sp)
		{
			ModifiedList.insert(sp);
			RecursiveLayout(sp, ClientRt, ModifiedList);
			LONG delta = (sp->PosRect.left - pci->Dock[DockLeft].Gap) - rt.right;
			rt.right += delta;
			if (!left_modified)
				rt.left += delta;
			right_modified = true;
		}
		else
		{
			pci->Dock[DockRight].TargetKind = TARGET_NONE;
		}
	}
	if (pci->Dock[DockBottom].TargetKind == TARGET_CHILD)
	{
		auto sp = pci->Dock[DockBottom].TargetChild.lock();
		if (sp)
		{
			ModifiedList.insert(sp);
			RecursiveLayout(sp, ClientRt, ModifiedList);
			LONG delta = (sp->PosRect.top - pci->Dock[DockBottom].Gap) - rt.bottom;
			rt.bottom += delta;
			if (!top_modified)
				rt.top += delta;
			bottom_modified = true;
		}
		else
		{
			pci->Dock[DockBottom].TargetKind = TARGET_NONE;
		}
	}

	// TARGET_FORM
	if (pci->Dock[DockLeft].TargetKind == TARGET_FORM)
	{
		LONG delta = pci->Dock[DockLeft].Gap - rt.left;
		rt.left = pci->Dock[DockLeft].Gap;
		if (!right_modified)
			rt.right += delta;
		left_modified = true;
	}
	if (pci->Dock[DockTop].TargetKind == TARGET_FORM)
	{
		LONG delta = pci->Dock[DockTop].Gap - rt.top;
		rt.top = pci->Dock[DockTop].Gap;
		if (!bottom_modified)
			rt.bottom += delta;
		top_modified = true;
	}
	if (pci->Dock[DockRight].TargetKind == TARGET_FORM)
	{
		LONG delta = (ClientRt.right - pci->Dock[DockRight].Gap) - rt.right;
		rt.right += delta;
		if (!left_modified)
			rt.left += delta;
	}
	if (pci->Dock[DockBottom].TargetKind == TARGET_FORM)
	{
		LONG delta = (ClientRt.bottom - pci->Dock[DockBottom].Gap) - rt.bottom;
		rt.bottom += delta;
		if (!top_modified)
			rt.top += delta;
	}
}

void CForm::ApplyMovedChild(const std::shared_ptr<CForm::ChildInfo> &pci)
{
	RECT ClientRt;
	GetClientRect(*this, &ClientRt);

	std::set<std::shared_ptr<ChildInfo> > ModifiedList;
	RecursiveLayout(pci, ClientRt, ModifiedList);

	m_bLayouting = true;
	HDWP hDefer = BeginDeferWindowPos((int)(unsigned)ModifiedList.size());
	for (const auto &sp : ModifiedList)
	{
		DeferMoveChild(hDefer, sp);
		sp->fProcessed = false;
	}
	EndDeferWindowPos(hDefer);
	m_bLayouting = false;
}


// CForm::Child
CForm::Child CForm::Child::GetDockTarget(Dock d) const
{
	if (GetDockKind(d) == TARGET_NONE)
		throw CInvalidArgError(L"도킹되어 있지 않습니다.");
	if (GetDockKind(d) == TARGET_FORM)
		throw CInvalidArgError(L"폼에 도킹되어 있습니다.");

	auto sp = (*m_it)->Dock[d].TargetChild.lock();
	if (!sp)
		throw CInvalidArgError(L"도킹한 다른 차일드가 이미 파괴되어 있습니다.");

	return Child(sp->Dock[d].TargetChild_it);
}

END_IKHWINLIB2()
