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

#include <boost/locale.hpp>
#include <locale>

#include "CProject.h"

BEGIN_MSGMAP(CProject, CMsgTarget)
END_MSGMAP(CProject, CMsgTarget)

CProject::CProject(CTreeViewCtrl &tvctl, const std::wstring &fname)
	: m_TreeView(tvctl), m_fname(fname)
{

}

void CProject::Load(const std::wstring &fname)
{
	if (!fname.empty())
	{
		m_fname = fname;
	}

	read_xml(
		boost::locale::conv::from_utf(m_fname, std::locale()),
		m_pt
		);
}

void CProject::Save(const std::wstring &fname)
{
	if (!fname.empty())
	{
		m_fname = fname;
	}

	write_xml(
		boost::locale::conv::from_utf(m_fname, std::locale()),
		m_pt
		);
}
