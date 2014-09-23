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
	std::wstring buf = boost::algorithm::trim_copy(line); // tokenizer�� wstring�� ���۷����� �޽��ϴ�.
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
		it->second.first(vttok);
	}
	else
	{
		throw ScripterException(L"'" + vttok[0] + L"': �� �� ���� ��ɾ��Դϴ�.");
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
				m_pHost->WriteLine(L"help: '" + vttok[1] + L"' ��ɾ�� �������� �ʽ��ϴ�.");
			else
				print_help(it);
		}
	}
	else
	{
		m_pHost->WriteLine(L"'help --detail'�� ġ�ø� �ڼ��� ������ ���� �� �ֽ��ϴ�.");

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
