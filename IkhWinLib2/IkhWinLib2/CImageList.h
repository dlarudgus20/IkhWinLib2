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

#pragma once

#include "CObject.h"
#include "NonCopyable.h"
#include "CSystemError.h"

BEGIN_IKHWINLIB2()

class CImageList : public CObject, private NonCopyable
{
private:
	HIMAGELIST m_hList;
	void destroy() NOEXCEPT;

private:
	CImageList(int) { /* do nothing */ }
public:
	CImageList();

	static CImageList Load(LPCTSTR lpImage, int cx, COLORREF crMask = CLR_NONE,
		int cGrow = 1, UINT uType = IMAGE_BITMAP, UINT uFlags = 0);

	static CImageList Create(int cx, int cy, UINT flags, int cInitial = 5, int cGrow = 5);

	virtual ~CImageList();

	CImageList(CImageList &&rhs) NOEXCEPT;
	CImageList &operator =(CImageList &&rhs) NOEXCEPT;

	operator HIMAGELIST() const NOEXCEPT;
};

inline CImageList::CImageList()
{
	m_hList = NULL;
}

inline CImageList CImageList::Create(int cx, int cy, UINT flags, int cInitial, int cGrow)
{
	CImageList ret(0);

	ret.m_hList = ImageList_Create(cx, cy, flags, cInitial, cGrow);

	if (ret.m_hList == NULL)
		throw CImageListCreationError();

	return ret;
}

inline CImageList CImageList::Load(LPCTSTR lpImage, int cx, COLORREF crMask,
	int cGrow, UINT uType, UINT uFlags)
{
	CImageList ret(0);

	ret.m_hList = ImageList_LoadImage(GetModuleHandle(NULL),
		lpImage, cx, cGrow, crMask, uType, uFlags);

	if (ret.m_hList == NULL)
		throw CImageListCreationError();

	return ret;
}

inline CImageList::CImageList(CImageList &&rhs) NOEXCEPT
{
	m_hList = rhs.m_hList;
	rhs.m_hList = NULL;
}
inline CImageList &CImageList::operator =(CImageList &&rhs) NOEXCEPT
{
	destroy();
	m_hList = rhs.m_hList;
	rhs.m_hList = NULL;
	return *this;
}

inline CImageList::operator HIMAGELIST() const NOEXCEPT
{
	return m_hList;
}

inline void CImageList::destroy() NOEXCEPT
{
	if (m_hList != NULL)
		ImageList_Destroy(m_hList);
}

END_IKHWINLIB2()
