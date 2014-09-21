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

#include <IkhWinLib2/CLogicError.h>
using namespace IkhProgram::IkhWinLib2;

#include <boost/tokenizer.hpp>
#include "SphereManager.h"

IKHWINLIB2_MAKE_EXCEPTION(ScripterException, CLogicError, L"스크립트 처리에 오류가 발생했습니다.")

class IScriptHost
{
public:
	virtual void WriteLine(const std::wstring &str) = 0;
	virtual void WriteMultiLine(const std::wstring &str) = 0;

	virtual void CreateSphere(const Sphere &sp) = 0;
};

class Scripter final
{
private:
	typedef boost::escaped_list_separator<wchar_t> separator;
	typedef boost::tokenizer<separator, std::wstring::const_iterator, std::wstring> tokenizer;
	separator m_separator;

	typedef std::function<void(const std::vector<std::wstring> &)> cmd_handler_t;
	typedef std::unordered_map<std::wstring, std::pair<cmd_handler_t, std::wstring> > cmd_map_t;
	cmd_map_t m_cmd_map;

	typedef boost::char_separator<wchar_t> punct_separator;
	typedef boost::tokenizer<punct_separator, std::wstring::const_iterator, std::wstring> punct_tokenizer;
	punct_separator m_PunctSeparator;

	IScriptHost *m_pHost;

public:
	explicit Scripter(IScriptHost *pHost);

	void ExecuteLine(const std::wstring &line);

private:
	void CommandHelp(const std::vector<std::wstring> &vttok);
	void CommandCreate(const std::vector<std::wstring> &vttok);
};
