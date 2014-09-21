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

#include <IkhWinLib2/Defines.h>
using namespace IkhProgram::IkhWinLib2;

#include <gl/gl.h>
#include <gl/glu.h>

inline double square(double d)
{
	return d * d;
}
inline void cross_product(double dest[3], const double (&lhs)[3], const double (&rhs)[3])
{
	dest[0] = rhs[1] * lhs[2] - lhs[1] * rhs[2];
	dest[1] = rhs[2] * lhs[0] - lhs[2] * rhs[0];
	dest[2] = rhs[0] * lhs[1] - lhs[0] * rhs[1];
}
inline double dot_product(const double (&lhs)[3], const double (&rhs)[3])
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}
inline double get_length_2(const double (&lhs)[3])
{
	return dot_product(lhs, lhs);
}
inline double get_length(const double (&lhs)[3])
{
	return sqrt(get_length_2(lhs));
}

template <typename T>
inline void negativen(T &data)
{
	data = -data;
}

inline void negativen(double (&data)[3])
{
	data[0] = -data[0];
	data[1] = -data[1];
	data[2] = -data[2];
}

struct Sphere
{
	double coord[3];
	double radius, mass;
	float color[4];

	double velocity[3];

	double angle[3];
	double AngularVelocity[3];

	GLuint DisplayList;

	void CompileDisplayList(GLUquadric *quadric);
	void DeleteDisplayList();
};

class SphereManager final
{
private:
	DWORD m_PrevUpdateTime;

	std::vector<Sphere> m_Spheres;
	double m_day;

	GLUquadric *m_quadric;

	void Run();
	void RunForce(std::vector<Sphere> &NewSpheres);
	void RunCollision(std::vector<Sphere> &NewSpheres);

public:
	struct CollisionInfo
	{
		// Sphere 번호
		int i, j;
		// 충돌 발생 시각
		double t;
		// i번 Sphere에서 본 j번 Sphere의 위치와 속도
		double p[3], v[3];
		// pre-calculated values
		double p_dot_v, p_length_2, v_length_2;

		bool bProcessed = false;
	};
private:
	std::shared_ptr<CollisionInfo> DetectCollision(
		const std::vector<Sphere> &NewSpheres, int i, int j, double RemainTime) const;

public:
	SphereManager();
	~SphereManager();

	double GetDay() const { return m_day; }

	void AddSphere(const Sphere &s);

	void UpdateIfExpired();
	void Render();
};
