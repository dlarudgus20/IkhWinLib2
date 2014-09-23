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
#include "Scripter.h"
#include "SphereManager.h"
#include "parse_utils.h"
using namespace parse_utils;

#include <boost/algorithm/string.hpp>

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
}

Scripter::Scripter(IScriptHost *pHost)
	: m_pHost(pHost)
	, m_separator(L"\\", L" \f\n\r\t\v", L"\"")
	, m_PunctSeparator(LR"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)")
{
	m_cmd_map.insert(
	{
		{ L"create", { std::bind(&Scripter::CommandCreate, this, _1), L"create new sphere" } },
		{ L"help", { std::bind(&Scripter::CommandHelp, this, _1), L"view help" } },
		{ L"ls", { std::bind(&Scripter::CommandLs, this, _1), L"view list of spheres" } },
		{ L"sum-momentum", { std::bind(&Scripter::CommandSumMomentum, this, _1), L"view sum of momentum & angular momentum" } }
	});
}

void Scripter::ExecuteLine(const std::wstring &line)
{
	std::wstring buf = boost::algorithm::trim_copy(line); // tokenizer는 wstring의 레퍼런스를 받습니다.
	tokenizer tok(buf, m_separator);

	if (tok.begin() == tok.end())
		return; // 라인이 텅 비었음

	if (tok.begin()->compare(0, 1, L"#") == 0)
		return;	// 주석

	std::vector<std::wstring> vttok(tok.begin(), tok.end());

	for (auto &str : vttok)
		boost::algorithm::trim(str);

	auto it = m_cmd_map.find(vttok[0]);
	if (it != m_cmd_map.end())
	{
		it->second.first(vttok);
	}
	else
	{
		throw ScripterException(L"'" + vttok[0] + L"': 알 수 없는 명령어입니다.");
	}
}

void Scripter::CommandHelp(const std::vector<std::wstring> &vttok)
{
	auto print_help = [this](cmd_map_t::const_iterator it) {
		m_pHost->WriteLine(it->first + L" : " + it->second.second); };

	if (vttok.size() == 2)
	{
		if (vttok[1] == L"--detail")
		{
			m_pHost->WriteMultiLine(
				L"Gravity - 만유인력 시뮬레이션 프로그램 made by 임경현\n"
				L"이 프로그램은 IkhWinLib2 (https://github.com/dlarudgus20/IkhWinLib2)의 예제 프로그램의 일부입니다.\n"
				L"화면 하단의 명령 창을 통해 프로그램을 제어할 수 있습니다.\n"
				L"'help' 명령을 내리면 명령어들의 목록과 도움말을 볼 수 있습니다."
				L"'help [명령어]' 혹은 '[명령어] --help'를 치면 특정 명령어에 대한 자세한 도움말을 볼 수 있습니다.\n"
				L"명령어 도움말은 다음과 같은 형태입니다.\n"
				L"    명령어 [필수인수1:타입] [필수인수2:타입] ... (선택인수1:타입) (선택인수2:타입) ...\n"
				L"필수인수는 반드시 입력해야 하는 인수를 말합니다. 선택인수는 반드시 입력하지 않아도 되는 인수이고, 입력하지 않을 시 디폴트값이 사용됩니다."
				L"인수 목록이 <<<no param>>> 인 명령어들은 인를 받지 않습니다.\n"
				L"타입은 인수의 형태를 말합니다. 이 프로그램엔 다음과 같은 타입이 있습니다.\n"
				L" vec3: 3차원 벡터입니다. 예: (1.1,2.5,3.99)\n"
				L" real: 실수입니다. 예: 2.3\n"
				L" rgba: RGBA 색상입니다. 각 색상의 범위는 0.0 ~ 1.0입니다. 예: (1,1,0,1) 불투명한 노랑색\n"
				);
		}
		else if (vttok[1] == L"--help")
		{
			return; // 무시
		}
		else
		{
			auto it = m_cmd_map.find(vttok[1]);
			if (it == m_cmd_map.end())
				m_pHost->WriteLine(L"help: '" + vttok[1] + L"' 명령어는 존재하지 않습니다.");
			else
				print_help(it);
		}
	}
	else
	{
		m_pHost->WriteLine(L"'help --detail'을 치시면 자세한 정보를 얻을 수 있습니다.");

		for (auto it = m_cmd_map.begin(); it != m_cmd_map.end(); ++it)
		{
			print_help(it);
			it->second.first({ std::wstring(), L"--help" });
		}
	}
}

void Scripter::CommandCreate(const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"create [coord:vec3] [radius:real] [mass:real] [color:rgba] (velocity:vec3) (AngularVelocity:vec3)";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (!(5 <= vttok.size() && vttok.size() <= 7))
		throw ScripterException(usage);

	Sphere sp { /* zero initialization */ };

	sp.coord = parse_vec3(vttok[1], error_handler(L"vec3"));
	sp.radius = parse_real(vttok[2], error_handler(L"real"));
	sp.mass = parse_real(vttok[3], error_handler(L"real"));
	sp.color = parse_rgba(vttok[4], error_handler(L"rgba"));

	if (vttok.size() >= 6)
		sp.velocity = parse_vec3(vttok[5], error_handler(L"vec3"));

	if (vttok.size() >= 7)
		sp.AngularVelocity = parse_vec3(vttok[6], error_handler(L"vec3"));

	m_pHost->GetSphereManager()->AddSphere(sp);
}

void Scripter::CommandLs(const std::vector<std::wstring> &vttok)
{
	const std::wstring usage = L"ls <<<no param>>>";

	if (vttok.size() == 2 && vttok[1] == L"--help")
	{
		m_pHost->WriteLine(usage);
		return;
	}

	if (vttok.size() != 1)
		throw ScripterException(usage);

	auto spheres = m_pHost->GetSphereManager()->GetSpheres();

	m_pHost->WriteLine(L"[num] / [coord] / [radius] / [mass] / [color] / [velocity] / [AngularVelocity]");
	for (int i = 0; i < spheres.size(); ++i)
	{
		Sphere &sp = spheres[i];

		m_pHost->WriteLine((
			boost::wformat(L"%d / (%.3f,%.3f,%.3f) / %.3f / %.3f / (%.3f,%.3f,%.3f,%.3f)"
				L" / (%.3f,%.3f,%.3f) [%.3f] / (%.3f,%.3f,%.3f) [%.3f]"
				)
			% i % sp.coord[0] % sp.coord[1] % sp.coord[2]
			% sp.radius % sp.mass
			% sp.color[0] % sp.color[1] % sp.color[2] % sp.color[3]
			% sp.velocity[0] % sp.velocity[1] % sp.velocity[2] % get_length(sp.velocity.data())
			% sp.AngularVelocity[0] % sp.AngularVelocity[1] % sp.AngularVelocity[2] % get_length(sp.AngularVelocity.data())
			).str());
	}
}

void Scripter::CommandSumMomentum(const std::vector<std::wstring> &vttok)
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

	double p[3] = { }, ap[3] = { };

	#pragma omp parallel for
	for (int i = 0; i < spheres.size(); ++i)
	{
		Sphere &sp = spheres[i];

		p[0] += sp.mass * sp.velocity[0];
		p[1] += sp.mass * sp.velocity[1];
		p[2] += sp.mass * sp.velocity[2];

		double moment_of_inertia = 2 * sp.mass * square(sp.radius) / 5;
		ap[0] += moment_of_inertia * sp.AngularVelocity[0];
		ap[1] += moment_of_inertia * sp.AngularVelocity[1];
		ap[2] += moment_of_inertia * sp.AngularVelocity[2];
	}

	m_pHost->WriteLine((
		boost::wformat(L"momentum: (%.3f,%.3f,%.3f) [%.3f]")
		% p[0] % p[1] % p[2] % get_length(p)
		).str());
	m_pHost->WriteLine((
		boost::wformat(L"angular-momentum: (%.3f,%.3f,%.3f) [%.3f]")
		% ap[0] % ap[1] % ap[2] % get_length(ap)
		).str());
}
