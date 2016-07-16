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

/**
 * @file CForm.h
 * @brief CForm 클래스
 */

#pragma once

#include "CWindow.h"

BEGIN_IKHWINLIB2()

class CForm : public CWindow
{
	DECLARE_MSGMAP();
public:
	enum TargetKind_t { TARGET_NONE, TARGET_CHILD, TARGET_FORM };
private:
	struct ChildInfo;

	typedef std::list<std::shared_ptr<ChildInfo> > ChildList;
	typedef ChildList::iterator ChildIterator;

	struct ChildInfo
	{
		struct DockInfo
		{
			TargetKind_t TargetKind = TARGET_NONE;
			std::weak_ptr<ChildInfo> TargetChild;
			ChildIterator TargetChild_it;
			int Gap;
		};

		CWindow *pWnd;

		DockInfo Dock[4];

		EventFnPtr<void(CWindow *)> efpDestroy;
		EventFnPtr<void(CWindow *)> efpSizeChanged;
		explicit ChildInfo(CWindow *wnd, ChildIterator end)
			: pWnd(wnd), fProcessed(false)
			, it_end(end), ZOrderPrev(end), ZOrderNext(end)
		{
		}
		~ChildInfo()
		{
			if (ZOrderPrev != it_end)
				(*ZOrderPrev)->ZOrderNext = ZOrderNext;
			if (ZOrderNext != it_end)
				(*ZOrderNext)->ZOrderPrev = ZOrderPrev;
		}

		bool fProcessed;

		RECT PosRect;

		ChildIterator it_end;
		ChildIterator ZOrderPrev, ZOrderNext;
	};

	ChildList m_ChildList;

	bool m_bLayoutIsSuspended = false;
	bool m_bLayoutRequired = false;
	bool m_bLayouting = false;

public:
	enum Dock
	{
		DockLeft = 0, DockTop = 1, DockRight = 2, DockBottom = 3
	};

	class Child
	{
		friend CForm;
	private:
		ChildIterator m_it;
		Child(ChildIterator it) : m_it(it) { }
	public:
		CWindow *GetWnd() const { return (*m_it)->pWnd; }

		TargetKind_t GetDockKind(Dock d) const { return (*m_it)->Dock[d].TargetKind; }
		Child GetDockTarget(Dock d) const;
		Child GetZOrderPrev() const { return Child((*m_it)->ZOrderPrev); }
		Child GetZOrderNext() const { return Child((*m_it)->ZOrderNext); }
	};

	Child AddChild(CWindow *pWnd);
	void RemoveChild(Child child);
	Child FindChild(CWindow *pWnd);
	typedef boost::optional<Child> NoTargetOpt;
	void DockChild(Child child, boost::optional<Child> target, Dock dock, int gap);
	void MoveChild(Child child, RECT pos);

	void SuspendLayout();
	void ResumeLayout();
	void PerformLayout();
private:
	void _updateLayout();
	void DeferMoveChild(HDWP hDefer, const std::shared_ptr<ChildInfo> &sp);
	void ApplyMovedChild(const std::shared_ptr<ChildInfo> &pci);

	template <typename T>
	struct value_less
	{
		bool operator ()(const std::shared_ptr<T> &lhs, const std::shared_ptr<T> &rhs) const
		{
			return lhs->get() < rhs->get();
		}
	};
	typedef std::set<std::shared_ptr<ChildInfo>, value_less<ChildInfo> > ModifiedList_t;

	void RecursiveLayout(const std::shared_ptr<ChildInfo> &pci, const RECT &ClientRt,
		std::set<std::shared_ptr<ChildInfo> > &ModifiedList);

	ChildIterator _findChildInfo(CWindow *pChild);

protected:
	void OnSize(UINT state, int cx, int cy);
};

inline void CForm::SuspendLayout()
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	m_bLayoutIsSuspended = true;
}
inline void CForm::ResumeLayout()
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	m_bLayoutIsSuspended = false;
	if (m_bLayoutRequired)
	{
		PerformLayout();
	}
}

inline void CForm::_updateLayout()
{
	assert(this != NULL);
	assert(GetHWND() != NULL);

	if (!m_bLayoutIsSuspended)
		PerformLayout();
	else
		m_bLayoutRequired = true;
}

inline CForm::ChildIterator CForm::_findChildInfo(CWindow *pChild)
{
	auto f = [pChild](const std::shared_ptr<ChildInfo> &sp) { return sp->pWnd == pChild; };
	return std::find_if(m_ChildList.begin(), m_ChildList.end(), f);
}

END_IKHWINLIB2()
