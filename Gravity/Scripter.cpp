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

#include "stdafx.h"
#include "Scripter.h"
#include "SphereManager.h"

#include <IkhWinLib2/CFileDialog.h>
#include "CMyApp.h"

#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

#include "file_util.h"
#include "parse_utils.h"

namespace
{
	struct error_handler
	{
		const std::wstring &type;
		explicit error_handler(const std::wstring &t)
			: type(t) { }

		void operator ()(const std::wstring &str) const
		{
			throw ScripterException(L"invalid " + type + L": '" + str + L"'");
		}
	};

	inline std::array<double, 3> parse_vec3(const std::wstring &str)
	{
		return parse_utils::parse_vec3(str, error_handler(L"vec3"));
	}
	inline double parse_real(const std::wstring &str)
	{
		return parse_utils::parse_real(str, error_handler(L"real"));
	}
	inline std::array<float, 4> parse_rgba(const std::wstring &str)
	{
		return parse_utils::parse_rgba(str, error_handler(L"rgba"));
	}
	inline bool parse_bool(const std::wstring &str)
	{
		return parse_utils::parse_bool(str, error_handler(L"bool"));
	}
}

Scripter::Scripter(IScriptHost *pHost)
	: m_pHost(pHost)
	, m_separator(L"\\", L" \f\n\r\t\v", L"\"")
	, m_status(ScripterStatus { true, L"/Gravity/" })
{
#define CMD_ENTRY(name, fn, desc) { name, { std::bind(&Scripter::fn, this, _1, _2), desc } }
	m_cmd_map.insert(
	{
		CMD_ENTRY(L"create", CommandCreate, L"���ο� ���� ����ϴ�."),
		CMD_ENTRY(L"help", CommandHelp, L"������ ���ϴ�."),
		CMD_ENTRY(L"list", CommandList, L"���� ����� ���ϴ�."),
		CMD_ENTRY(L"sum-momentum", CommandSumMomentum, L"��ü ����� ���� ����մϴ�."),
		CMD_ENTRY(L"pause", CommandPause, L"�ùķ��̼��� �Ͻ������մϴ�."),
		CMD_ENTRY(L"resume", CommandResume, L"�ùķ��̼��� �簳�մϴ�."),
		CMD_ENTRY(L"save", CommandSave, L"���� ���¸� ��ũ��Ʈ�� �����մϴ�."),
		CMD_ENTRY(L"load", CommandLoad, L"��ũ��Ʈ�� �ҷ��� �����մϴ�."),
		CMD_ENTRY(L"setecho", CommandSetEcho, L"echo Ȱ��ȭ ���θ� �����մϴ�."),
		CMD_ENTRY(L"echo", CommandEcho, L"���ڿ��� ����մϴ�."),
		CMD_ENTRY(L"forcescroll", CommandForcescroll, L"���� ��ũ�� ���θ� �����մϴ�.")
	});
#undef CMD_ENTRY
}

void Scripter::ExecuteLine(const std::wstring &line, bool bThrow /* = false */)
{
	bool echo = m_status.bEcho;
	bool echo_forced = false;
	if (line.size() > 1)
	{
		if (line.front() == '$')
		{
			echo = true;
			echo_forced = true;
		}
		else if (line.front() == '@')
		{
			echo = false;
			echo_forced = true;
		}
	}

	if (echo)
		m_pHost->WriteLine(m_status.prompt + L" > " + line);

	try 
	{
		std::wstring buf = boost::algorithm::trim_copy(line); // tokenizer�� wstring�� ���۷����� �޽��ϴ�.
		if (echo_forced)
			buf.erase(buf.begin());

		try
		{
			tokenizer tok(buf, m_separator);
	
			if (tok.begin() == tok.end())
				return; // ������ �� �����
	
			if (tok.begin()->compare(0, 1, L"#") == 0)
				return;	// �ּ�
	
			std::vector<std::wstring> vttok(tok.begin(), tok.end());
	
			for (auto &str : vttok)
				boost::algorithm::trim(str);
	
			auto it = m_cmd_map.find(vttok[0]);
			if (it != m_cmd_map.end())
			{
				it->second.first(buf, vttok);
			}
			else
			{
				throw ScripterException(L"'" + vttok[0] + L"': �� �� ���� ��ɾ��Դϴ�.");
			}
		}
		catch (boost::escaped_list_error &)
		{
			throw ScripterException(L"'" + buf + L"' : syntax error.");
		}
	}
	catch (ScripterException &e)
	{
		m_pHost->WriteLine(m_status.prompt + L" <err>) " + e.Message());

		if (bThrow)
			throw;
	}
}

void Scripter::CommandHelp(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	auto print_help = [this](cmd_map_t::const_iterator it) {
		m_pHost->WriteLine(it->first + L" : " + it->second.second); };

	if (vttok.size() == 2)
	{
		if (vttok[1] == L"--detail")
		{
			m_pHost->WriteMultiLine(
				L"Gravity - �����η� �ùķ��̼� ���α׷� made by �Ӱ���\n"
				L"�� ���α׷��� IkhWinLib2 (https://github.com/dlarudgus20/IkhWinLib2)�� ���� ���α׷��� �Ϻ��Դϴ�.\n"
				L"ȭ�� �ϴ��� ��� â�� ���� ���α׷��� ������ �� �ֽ��ϴ�.\n"
				L"'help' ����� ������ ��ɾ���� ��ϰ� ������ �� �� �ֽ��ϴ�."
				L"'help [��ɾ�]' Ȥ�� '[��ɾ�] --help'�� ġ�� Ư�� ��ɾ ���� �ڼ��� ������ �� �� �ֽ��ϴ�.\n"
				L"��ɾ� ������ ������ ���� �����Դϴ�.\n"
				L"    ��ɾ� [�ʼ��μ�1:Ÿ��] [�ʼ��μ�2:Ÿ��] ... (�����μ�1:Ÿ��) (�����μ�2:Ÿ��) ...\n"
				L"�ʼ��μ��� �ݵ�� �Է��ؾ� �ϴ� �μ��� ���մϴ�. �����μ��� �ݵ�� �Է����� �ʾƵ� �Ǵ� �μ��̰�, �Է����� ���� �� ����Ʈ���� ���˴ϴ�."
				L"�μ� ����� <<<no param>>> �� ��ɾ���� �θ� ���� �ʽ��ϴ�.\n"
				L"Ÿ���� �μ��� ���¸� ���մϴ�. �� ���α׷��� ������ ���� Ÿ���� �ֽ��ϴ�.\n"
				L" vec3: 3���� �����Դϴ�. ��: (1.1,2.5,3.99)\n"
				L" real: �Ǽ��Դϴ�. ��: 2.3\n"
				L" rgba: RGBA �����Դϴ�. �� ������ ������ 0.0 ~ 1.0�Դϴ�. ��: (1,1,0,1) �������� �����\n"
				L" string: ���ڿ��Դϴ�. ��: \"����������\"\n"
				L" bool: ��/�����Դϴ�. 1 Ȥ�� true�� ���̰�, 0 Ȥ�� false�� �����Դϴ�.\n"
				);
		}
		else if (vttok[1] == L"--help")
		{
			return; // ����
		}
		else
		{
			auto it = m_cmd_map.find(vttok[1]);
			if (it == m_cmd_map.end())
			{
				m_pHost->WriteLine(L"help: '" + vttok[1] + L"' ��ɾ�� �������� �ʽ��ϴ�.");
			}
			else
			{
				print_help(it);
				it->second.first(L"", { L"--help" });
			}
		}
	}
	else
	{
		m_pHost->WriteLine(L"'help --detail'�� ġ�ø� �ڼ��� ������ ���� �� �ֽ��ϴ�.");

		for (auto it = m_cmd_map.begin(); it != m_cmd_map.end(); ++it)
		{
			print_help(it);
		}
	}
}

void Scripter::CommandCreate(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"create [��ǥ:vec3] [������:real] [����:real] [��:rgba] (����:real) (�ӵ�:vec3) (���ӵ�:vec3)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (!(5 <= vttok.size() && vttok.size() <= 8))
		throw ScripterException(usage);

	Sphere sp { /* zero initialization */ };

	sp.coord = parse_vec3(vttok[1]);
	sp.radius = parse_real(vttok[2]);
	sp.mass = parse_real(vttok[3]);
	sp.color = parse_rgba(vttok[4]);

	if (vttok.size() >= 6)
		sp.shininess = static_cast<float>(parse_real(vttok[5]));
	else
		sp.shininess = 64.0f;

	if (vttok.size() >= 7)
		sp.velocity = parse_vec3(vttok[6]);

	if (vttok.size() >= 8)
		sp.AngularVelocity = parse_vec3(vttok[7]);

	m_pHost->GetSphereManager()->AddSphere(sp);
}

void Scripter::CommandList(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"list <<<no param>>>";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() != 1)
		throw ScripterException(usage);

	auto spheres = m_pHost->GetSphereManager()->GetSpheres();

	m_pHost->WriteLine(L"[��ȣ] / [��ǥ] / [������] / [����] / [��] / [�ӵ�] / [���ӵ�]");
	for (int i = 0; static_cast<size_t>(i) < spheres.size(); ++i)
	{
		Sphere &sp = spheres[i];

		m_pHost->WriteLine((
			boost::wformat(L"%d / (%.3f,%.3f,%.3f) / %.3f / %.3f / (%.3f,%.3f,%.3f,%.3f)"
				L" / (%.3f,%.3f,%.3f) [%.3f] / (%.3f,%.3f,%.3f) [%.3f]"
				)
			% i % sp.coord[0] % sp.coord[1] % sp.coord[2]
			% sp.radius % sp.mass
			% sp.color[0] % sp.color[1] % sp.color[2] % sp.color[3]
			% sp.velocity[0] % sp.velocity[1] % sp.velocity[2] % get_length(sp.velocity)
			% sp.AngularVelocity[0] % sp.AngularVelocity[1] % sp.AngularVelocity[2] % get_length(sp.AngularVelocity)
			).str());
	}
}

void Scripter::CommandSumMomentum(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"sum-momentum <<<no param>>>";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() != 1)
		throw ScripterException(usage);

	auto spheres = m_pHost->GetSphereManager()->GetSpheres();

	std::array<double, 3> p = { 0, 0, 0 };

	#pragma omp parallel for
	for (int i = 0; static_cast<size_t>(i) < spheres.size(); ++i)
	{
		Sphere &sp = spheres[i];

		p[0] += sp.mass * sp.velocity[0];
		p[1] += sp.mass * sp.velocity[1];
		p[2] += sp.mass * sp.velocity[2];
	}

	m_pHost->WriteLine((
		boost::wformat(L"��ü ����� ��: (%.3f,%.3f,%.3f) [%.3f]")
		% p[0] % p[1] % p[2] % get_length(p)
		).str());
}

void Scripter::CommandPause(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"pause (quiet:bool)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() > 2)
		throw ScripterException(usage);

	bool quiet = false;
	if (vttok.size() == 2)
		quiet = parse_bool(vttok[1]);

	if (!m_pHost->GetSphereManager()->IsPaused())
	{
		m_pHost->GetSphereManager()->PauseRunning();
		m_pHost->UpdateTitle();
	}
	else if (!quiet)
	{
		throw ScripterException(L"�ùķ��̼��� �̹� �Ͻ����� �����Դϴ�.");
	}
}

void Scripter::CommandResume(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"resume (quiet:bool)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() > 2)
		throw ScripterException(usage);

	bool quiet = false;
	if (vttok.size() == 2)
		quiet = parse_bool(vttok[1]);

	if (m_pHost->GetSphereManager()->IsPaused())
	{
		m_pHost->GetSphereManager()->ResumeRunning();
		m_pHost->UpdateTitle();
	}
	else if (!quiet)
	{
		throw ScripterException(L"�ùķ��̼��� �Ͻ����� ���°� �ƴմϴ�.");
	}
}

void Scripter::CommandSave(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"save (�ڵ�����:bool) (���ϸ�:string)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() > 3)
		throw ScripterException(usage);

	bool bAutoRun = false;
	if (vttok.size() >= 2)
		bAutoRun = parse_bool(vttok[1]);

	std::wstring fname;
	if (vttok.size() >= 3)
	{
		fname = vttok[2];
	}
	else
	{
		CFileDialog dialog(false, L"Gravity ��ũ��Ʈ ����\0*.grvs\0��� ����\0*.*\0", L"grvs", L"��ũ��Ʈ�� ����");
		if (dialog.DoModal(*GetMyApp()->GetMainWnd()) != IDOK)
		{
			throw ScripterException(L"����ڰ� ��ũ��Ʈ�� ������ ����Ͽ����ϴ�.");
		}
		fname = dialog.GetFileName();
	}

	try
	{
		std::wofstream file = file_util::open_output_file(fname);

		const auto &spheres = m_pHost->GetSphereManager()->GetSpheres();
		for (const Sphere &sp : spheres)
		{
			file << L"create "
				<< parse_utils::tostr_vec3(sp.coord) << ' '
				<< parse_utils::tostr_real(sp.radius) << ' '
				<< parse_utils::tostr_real(sp.mass) << ' '
				<< parse_utils::tostr_rgba(sp.color) << ' '
				<< parse_utils::tostr_real(sp.shininess) << ' '
				<< parse_utils::tostr_vec3(sp.velocity) << ' '
				<< parse_utils::tostr_vec3(sp.AngularVelocity) << L'\n';
		}

		if (bAutoRun)
		{
			file << L"resume true";
		}
	}
	catch (file_util::CFileUtilException &e)
	{
		throw ScripterException(fname + L": " + e.Message());
	}
	catch (std::ios_base::failure &)
	{
		throw ScripterException(fname + L": ���� ���忡 �����Ͽ����ϴ�.");
	}

	m_pHost->WriteLine(fname + L": ��ũ��Ʈ ���� �Ϸ�");
}

void Scripter::CommandLoad(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"load (���ϸ�:string)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() > 2)
		throw ScripterException(usage);

	std::wstring fname;
	if (vttok.size() == 2)
	{
		fname = vttok[1];
	}
	else
	{
		CFileDialog dialog(true, L"Gravity ��ũ��Ʈ ����\0*.grvs\0��� ����\0*.*\0", L"grvs", L"��ũ��Ʈ �ҷ�����");
		if (dialog.DoModal(*GetMyApp()->GetMainWnd()) != IDOK)
		{
			throw ScripterException(L"����ڰ� ��ũ��Ʈ �ҷ����⸦ ����Ͽ����ϴ�.");
		}
		fname = dialog.GetFileName();
	}

	try
	{
		std::wifstream file = file_util::open_input_file(fname);
		std::wstring strLine;
		unsigned line = 0;

		ScripterStatus old_status = m_status;
		m_status.prompt = fname;

		BOOST_SCOPE_EXIT(old_status, this_)
		{
			this_->m_status = old_status;
		}
		BOOST_SCOPE_EXIT_END

		file.exceptions(std::ios::badbit);

		while (1)
		{
			if (file.eof())
				break;
			else if (file.fail())
				throw std::ios_base::failure("");

			std::getline(file, strLine);
			line++;

			try
			{
				ExecuteLine(strLine, false);
			}
			catch (ScripterException &e)
			{
				throw ScripterException(fname + L'(' + std::to_wstring(line) + L"): " + e.Message());
			}
		}
	}
	catch (file_util::CFileUtilException &e)
	{
		throw ScripterException(fname + L": " + e.Message());
	}
	catch (std::ios_base::failure &)
	{
		throw ScripterException(fname + L": ������ ���� �� �����ϴ�.");
	}

	m_pHost->WriteLine(fname + L": ��ũ��Ʈ ���� �Ϸ�");
}

void Scripter::CommandSetEcho(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"setecho [echo�ѱ�:bool]";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() != 2)
		throw ScripterException(usage);

	m_status.bEcho = parse_bool(vttok[1]);
}

void Scripter::CommandEcho(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"echo ��¹��ڿ�";

	if (line.empty() && vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() > 2)
		throw ScripterException(usage);

	if (line.size() == vttok[0].size())
	{
		m_pHost->WriteLine(L"");
	}
	else
	{
		size_t idx;
		for (idx = vttok[0].size(); isspace(line[idx]); idx++) { }

		m_pHost->WriteLine(line.substr(idx));
	}
}

void Scripter::CommandForcescroll(const std::wstring &line, const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"forcescroll [��ũ�ѿ���:bool]";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() != 2)
		throw ScripterException(usage);

	m_pHost->ForceScroll(parse_bool(vttok[1]));
}
