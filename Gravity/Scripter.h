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

class SphereManager;

IKHWINLIB2_MAKE_EXCEPTION(ScripterException, CLogicError, L"스크립트 처리에 오류가 발생했습니다.")

class IScriptHost
{
public:
	virtual void WriteLine(const std::wstring &str) = 0;
	virtual void WriteMultiLine(const std::wstring &str) = 0;

	virtual void UpdateTitle() = 0;
	virtual void ForceScroll(bool bScroll) = 0;

	virtual SphereManager *GetSphereManager() = 0;
};

class Scripter final
{
private:
	typedef boost::escaped_list_separator<wchar_t> separator;
	typedef boost::tokenizer<separator, std::wstring::const_iterator, std::wstring> tokenizer;
	separator m_separator;

	typedef std::function<void(const std::wstring &, const std::vector<std::wstring> &)> cmd_handler_t;
	typedef std::unordered_map<std::wstring, std::pair<cmd_handler_t, std::wstring> > cmd_map_t;
	cmd_map_t m_cmd_map;

	IScriptHost *m_pHost;

	struct ScripterStatus
	{
		bool bEcho;
		std::wstring prompt;
	};
	ScripterStatus m_status;

public:
	explicit Scripter(IScriptHost *pHost);

	void ExecuteLine(const std::wstring &line, bool bThrow = false);

private:
	void CommandHelp(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandCreate(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandList(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandSumMomentum(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandPause(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandResume(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandSave(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandLoad(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandSetEcho(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandEcho(const std::wstring &line, const std::vector<std::wstring> &vttok);
	void CommandForcescroll(const std::wstring &line, const std::vector<std::wstring> &vttok);
};
