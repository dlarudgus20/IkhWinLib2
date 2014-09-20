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

#include "SphereManager.h"

#define LOGICAL_TIME_SPAN (0.5)
#define UPDATE_TIME_SPAN (50)

// double 비교에 사용될 무한소값
#define EPSILON (1e-20)


SphereManager::SphereManager()
	: m_PrevUpdateTime(GetTickCount()), m_day(0)
{
	m_quadric = gluNewQuadric();
	gluQuadricDrawStyle(m_quadric, GLU_FILL);
}

SphereManager::~SphereManager()
{
	gluDeleteQuadric(m_quadric);
}

void SphereManager::AddSphere(const Sphere &s)
{
	m_Spheres.push_back(s);
	m_Spheres.back().CompileDisplayList(m_quadric);
}

void SphereManager::UpdateIfExpired()
{
	DWORD span = GetTickCount() - m_PrevUpdateTime;
	if (span >= UPDATE_TIME_SPAN)
	{
		DWORD count = span / UPDATE_TIME_SPAN;
		m_PrevUpdateTime += count * UPDATE_TIME_SPAN;

		for (DWORD q = 0; q < count; q++)
		{
			Run();
		}
	}
}

void SphereManager::Run()
{
	std::vector<Sphere> NewSpheres = m_Spheres;

	RunForce(NewSpheres);
	//RunCollision(NewSpheres);

	m_Spheres = std::move(NewSpheres);
	m_day += LOGICAL_TIME_SPAN;
}

void SphereManager::RunForce(std::vector<Sphere> &NewSpheres)
{
	// 만유인력 상수 (단위계.txt 참조)
	const double G = 0.49813678080;

	#pragma omp parallel for
	for (int i = 0; static_cast<size_t>(i) < m_Spheres.size(); i++)
	{
		Sphere &newsp = NewSpheres[i];

		// 합력
		double sum_f[3] = { 0, 0, 0 };

		for (int j = 0; static_cast<size_t>(j) < m_Spheres.size(); j++)
		{
			if (i == j)
				continue;

			Sphere &sp2 = m_Spheres[j];

			double f[3] = {
				sp2.coord[0] - newsp.coord[0],
				sp2.coord[1] - newsp.coord[1],
				sp2.coord[2] - newsp.coord[2],
			};

			double length_2 = dot_product(f, f);
			// 중력 상수는 합력에 한꺼번에 곱해줌
			// G*F1 + G*F2 + ... = G*(F1 + F2 + ...)
			double SizeOfForce = /*G * */ newsp.mass * sp2.mass / length_2;

			double length = sqrt(length_2);
			f[0] *= SizeOfForce / length;
			f[1] *= SizeOfForce / length;
			f[2] *= SizeOfForce / length;

			sum_f[0] += f[0];
			sum_f[1] += f[1];
			sum_f[2] += f[2];
		}

		sum_f[0] *= G;
		sum_f[1] *= G;
		sum_f[2] *= G;

		// F = ma
		// F/m = a = dv/dt
		// dt * F/m = dv
		double dv[3] = {
			LOGICAL_TIME_SPAN * sum_f[0] / newsp.mass,
			LOGICAL_TIME_SPAN * sum_f[1] / newsp.mass,
			LOGICAL_TIME_SPAN * sum_f[2] / newsp.mass
		};

		// v = ds/dt
		// ds = v * dt = ((v1 + v2)/2) * dt
		newsp.coord[0] += (newsp.velocity[0] + dv[0] / 2) * LOGICAL_TIME_SPAN;
		newsp.coord[1] += (newsp.velocity[1] + dv[1] / 2) * LOGICAL_TIME_SPAN;
		newsp.coord[2] += (newsp.velocity[2] + dv[2] / 2) * LOGICAL_TIME_SPAN;

		newsp.velocity[0] += dv[0];
		newsp.velocity[1] += dv[1];
		newsp.velocity[2] += dv[2];
	}
}

void SphereManager::RunCollision(std::vector<Sphere> &NewSpheres)
{
	std::vector<std::shared_ptr<CollisionInfo> > collisions;

	// pair.first -> 충돌정보
	// pair.second -> `true`이면 negative - i와 j를 반대로!
	std::multimap<int, std::pair<std::shared_ptr<CollisionInfo>, bool> > col_map;

	double RemainTime = LOGICAL_TIME_SPAN;

	bool bNoCollision;
	while (1)
	{
		bNoCollision = true;

		// 충돌 감지
		for (int i = 0; static_cast<size_t>(i) < m_Spheres.size() - 1; i++)
		{
			#pragma omp parallel for
			for (int j = 0; static_cast<size_t>(j) < m_Spheres.size(); j++)
			{
				auto ptr = DetectCollision(NewSpheres, i, j, RemainTime);
				if (!ptr)
					continue;

				#pragma omp critical
				{
					bNoCollision = false;
					collisions.push_back(std::move(ptr));
				}
			}
		}

		if (bNoCollision)
			break;

		// 맨 먼저 일어난 충돌들을 col_map에 기록
		for (auto &ptr : collisions)
		{
			int arkey[2] = { ptr->i, ptr->j };
			bool key_is_j = false;

			for (int key : arkey)
			{
				auto it = col_map.find(key);
				if (it == col_map.end())
				{
					col_map.emplace(key, std::make_pair(ptr, key_is_j));
				}
				else
				{
					if (it->second.first->t - ptr->t < EPSILON)
					{
						// 충돌이 동시에 일어남
						col_map.emplace(key, std::make_pair(ptr, key_is_j));
					}
					else if (it->second.first->t > ptr->t)
					{
						// `ptr`의 충돌이 `it->second.first`보다 먼저 일어남
						it->second.first = ptr;
					}
				}

				key_is_j = true;
			}
		}
	}
}

std::shared_ptr<SphereManager::CollisionInfo> SphereManager::DetectCollision(
	const std::vector<Sphere> &NewSpheres, int i, int j, double RemainTime) const
{
	const Sphere &oldsp_1 = m_Spheres[i];
	const Sphere &newsp_1 = NewSpheres[i];
	const Sphere &oldsp_2 = m_Spheres[j];
	const Sphere &newsp_2 = NewSpheres[j];

	// 궤적: p + tv (p=처음 위치, v=속도)
	// 원점 간 거리: |(p1 - p2) + t(v1 - v2)| = r1+r2 =: k
	// p:=p1-p2, v:=v1-v2
	// |p + tv|^2 = k^2
	// |p|^2 + |v|^2 t^2 + 2t (p.v) = k^2
	// (|v|^2)t^2 + 2(p.v) t + |p|^2 - k^2 = 0
	// t = -(p.v) +- sqrt( (p.v)^2 - (|v||p|)^2 + k^2 |v|^2

	CollisionInfo info;

	info.p[0] = oldsp_1.coord[0] - oldsp_2.coord[0];
	info.p[1] = oldsp_1.coord[1] - oldsp_2.coord[1];
	info.p[2] = oldsp_1.coord[2] - oldsp_2.coord[2];

	info.v[0] = oldsp_1.velocity[0] - oldsp_2.velocity[0];
	info.v[0] = oldsp_1.velocity[1] - oldsp_2.velocity[1];
	info.v[0] = oldsp_1.velocity[2] - oldsp_2.velocity[2];

	info.p_dot_v = dot_product(info.p, info.v);
	info.p_length_2 = dot_product(info.p, info.p);
	info.v_length_2 = dot_product(info.v, info.v);

	// 두 원의 반지름 합
	double k = oldsp_1.radius + oldsp_2.radius;

	// 판별식 < 0 이면 충돌 ㄴㄴ
	double D = info.p_dot_v * info.p_dot_v - info.p_length_2 * info.v_length_2 + k * k * info.v_length_2;
	if (D < 0)
		return std::shared_ptr<CollisionInfo>();

	double sqrt_D = sqrt(D);
	double t1 = -info.p_dot_v + sqrt_D;
	double t2 = -info.p_dot_v - sqrt_D;

	// 근이 음수면 충돌 ㄴㄴ
	if (t1 < 0)
		return std::shared_ptr<CollisionInfo>();

	// 처음 충돌의 시각
	info.t = min(t1, t2);
	// ...이 RemainTime보다 작아야 함
	if (info.t > RemainTime)
		return std::shared_ptr<CollisionInfo>();

	return std::make_shared<CollisionInfo>(info);
}

void SphereManager::Render()
{
	for (const Sphere s : m_Spheres)
	{
		glPushMatrix();
		glTranslated(s.coord[0], s.coord[1], s.coord[2]);
		glCallList(s.DisplayList);
		glPopMatrix();
	}
}

void Sphere::CompileDisplayList(GLUquadric *quadric)
{
	DisplayList = glGenLists(1);
	glNewList(DisplayList, GL_COMPILE);
	glColor4fv(color);
	gluSphere(quadric, radius, GLint(radius * 100), GLint(radius * 100));
	glEndList();
}

void Sphere::DeleteDisplayList()
{
	glDeleteLists(DisplayList, 1);
}
