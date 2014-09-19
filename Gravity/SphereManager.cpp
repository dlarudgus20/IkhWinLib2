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
	// 만유인력 상수 (단위계.txt 참조)
	const double G = 0.49813678080;

	std::vector<Sphere> NewSpheres = m_Spheres;

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

			double length_2 = f[0] * f[0] + f[1] * f[1] + f[2] * f[2];
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

	m_Spheres = std::move(NewSpheres);
	m_day += LOGICAL_TIME_SPAN;
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
