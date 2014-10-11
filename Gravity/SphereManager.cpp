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
#include "SphereManager.h"

#define LOGICAL_TIME_SPAN (0.5)
#define UPDATE_TIME_SPAN (50)

#define EPSILON (1e-13)

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

#ifndef _DEBUG
		for (DWORD q = 0; q < count; q++)
#endif
		{
			Run();
		}
	}
}

void SphereManager::Run()
{
	std::vector<Sphere> NewSpheres = m_Spheres;

	RunForce(NewSpheres);
	RunCollision(NewSpheres);

	m_Spheres = std::move(NewSpheres);
	m_day += LOGICAL_TIME_SPAN;
}

void SphereManager::RunForce(std::vector<Sphere> &NewSpheres)
{
	// �����η� ��� (SystemOfUnits.md ����)
	const double G = 0.49813678080;

	#pragma omp parallel for
	for (int i = 0; static_cast<size_t>(i) < m_Spheres.size(); i++)
	{
		Sphere &newsp = NewSpheres[i];

		// �շ�
		std::array<double, 3> sum_f = { 0, 0, 0 };

		for (int j = 0; static_cast<size_t>(j) < m_Spheres.size(); j++)
		{
			if (i == j)
				continue;

			Sphere &sp2 = m_Spheres[j];

			std::array<double, 3> f = {
				sp2.coord[0] - newsp.coord[0],
				sp2.coord[1] - newsp.coord[1],
				sp2.coord[2] - newsp.coord[2],
			};

			double length_2 = dot_product(f, f);
			// �߷� ����� �շ¿� �Ѳ����� ������
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
		std::array<double, 3> dv = {
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

		// ���ӵ� ó��
		newsp.angle[0] += newsp.AngularVelocity[0] * LOGICAL_TIME_SPAN;
		newsp.angle[1] += newsp.AngularVelocity[1] * LOGICAL_TIME_SPAN;
		newsp.angle[2] += newsp.AngularVelocity[2] * LOGICAL_TIME_SPAN;
	}
}

void SphereManager::RunCollision(std::vector<Sphere> &NewSpheres)
{
	// �浹 ó��
	// ������ ��Ȳ�� ó������ ����; �ڵ��� �ʿ���

	std::vector<std::shared_ptr<CollisionInfo> > collisions;

	// pair.first -> �浹����
	// pair.second -> `true`�̸� negative - i�� j�� �ݴ��!
	typedef std::multimap<int, std::pair<std::shared_ptr<CollisionInfo>, bool> > col_map_t;
	col_map_t col_map;

	// �浹 ����

	//#pragma omp parallel for
	for (int i = 0; static_cast<size_t>(i) < m_Spheres.size() - 1; i++)
	{
		#pragma omp parallel for
		for (int j = i + 1; static_cast<size_t>(j) < m_Spheres.size(); j++)
		{
			auto ptr = DetectCollision(NewSpheres, i, j, LOGICAL_TIME_SPAN);
			if (!ptr)
				continue;

			#pragma omp critical
			{
				collisions.push_back(std::move(ptr));
			}
		}
	}

	std::unordered_map<int, double> map_time;

	// �� ���� �Ͼ �浹���� col_map�� ���
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
				if (abs(it->second.first->t - ptr->t) < EPSILON)
				{
					// �浹�� ���ÿ� �Ͼ
					col_map.emplace(key, std::make_pair(ptr, key_is_j));
				}
				else if (it->second.first->t > ptr->t)
				{
					// `ptr`�� �浹�� `it->second.first`���� ���� �Ͼ
					it->second.first = ptr;
				}
				else
				{
					goto cont;
				}
			}

			map_time[key] = ptr->t;

		cont:
			key_is_j = true;
		}
	}

	// ��ݷ�/����ݷ��� ������ ����� map�� ���

	std::unordered_map<int, std::array<double, 3> > map_sum_I, map_sum_A;

	std::pair<col_map_t::iterator, col_map_t::iterator> eq_rg;

	for (int idx = 0; ; )
	{
		eq_rg = col_map.equal_range(idx);
		if (eq_rg.first != eq_rg.second)
		{
			auto i_sum_I = map_sum_I.emplace(idx, std::array<double, 3>{ { 0.0, 0.0, 0.0 } }).first;
			auto i_sum_A = map_sum_A.emplace(idx, std::array<double, 3>{ { 0.0, 0.0, 0.0 } }).first;

			std::for_each(eq_rg.first, eq_rg.second,
				[&NewSpheres, &map_sum_I, &map_sum_A, i_sum_I, i_sum_A, idx] (col_map_t::value_type &pr) {
				auto &ptr = pr.second.first;
				if (!ptr->bProcessed)
				{
					if (pr.second.second) // j�� sphere �������� ó���ؾ� �Ѵٸ�
					{
						// j�� sphere�� ��ǥ��� �ٲ�
						std::swap(ptr->i, ptr->j);
						negativen(ptr->p);
						negativen(ptr->v);
					}

					// docs.md#��ݷ� ����

					// ���� ����� ��ݷ��� ������ �������� ���� ���
					// DetectCollision() �Լ����� �̹� ó���߱� ������ �־�� �ȉ�.
					assert(!(ptr->p_dot_v > EPSILON));

					auto j_sum_I = map_sum_I.emplace(ptr->j, std::array<double, 3> { { 0.0, 0.0, 0.0 } }).first;
					auto j_sum_A = map_sum_A.emplace(ptr->j, std::array<double, 3> { { 0.0, 0.0, 0.0 } }).first;

					double v_length = sqrt(ptr->v_length_2);

					double cos_theta_2 = square(ptr->p_dot_v) / ptr->p_length_2 / ptr->v_length_2;
					double sin_theta_2 = 1 - cos_theta_2;

					double sum_inverse_mass = (1 / NewSpheres[ptr->i].mass)
						+ (1 / NewSpheres[ptr->j].mass);

					double sum_inverse_moment = (1 / (NewSpheres[ptr->i].mass * 2 / 5))
						+ (1 / (NewSpheres[ptr->i].mass * 2 / 5));

					double J_length = (2 * v_length * cos_theta_2)
						/ (sum_inverse_mass * cos_theta_2 + sum_inverse_moment * sin_theta_2);

					std::array<double, 3> J = ptr->v;
					J[0] *= J_length / v_length;
					J[1] *= J_length / v_length;
					J[2] *= J_length / v_length;
	
					double I_length = J_length * sqrt(cos_theta_2);
	
					std::array<double, 3> i_I, j_I;
					std::array<double, 3> i_moment_arm;
					std::array<double, 3> j_moment_arm;

					double p_length = sqrt(ptr->p_length_2);
					i_moment_arm = ptr->p;
					i_moment_arm[0] /= p_length;
					i_moment_arm[1] /= p_length;
					i_moment_arm[2] /= p_length;

					j_moment_arm = i_moment_arm;
					i_I = i_moment_arm;
					j_I = i_moment_arm;
	
					i_I[0] *= I_length;
					i_I[1] *= I_length;
					i_I[2] *= I_length;
	
					j_I[0] *= -I_length;
					j_I[1] *= -I_length;
					j_I[2] *= -I_length;
	
					i_moment_arm[0] *= NewSpheres[ptr->i].radius;
					i_moment_arm[1] *= NewSpheres[ptr->i].radius;
					i_moment_arm[2] *= NewSpheres[ptr->i].radius;
	
					j_moment_arm[0] *= -NewSpheres[ptr->j].radius;
					j_moment_arm[1] *= -NewSpheres[ptr->j].radius;
					j_moment_arm[2] *= -NewSpheres[ptr->j].radius;
	
					std::array<double, 3> i_A, j_A;
					cross_product(i_A, i_moment_arm, J); // i_A = i_r x i_J
					cross_product(j_A, J, j_moment_arm); // j_A = j_r x j_J = j_r x (-i_J) = -(j_r x i_J) = i_J x j_r

					i_sum_I->second[0] += i_I[0];
					i_sum_I->second[1] += i_I[1];
					i_sum_I->second[2] += i_I[2];
					i_sum_A->second[0] += i_A[0];
					i_sum_A->second[1] += i_A[1];
					i_sum_A->second[2] += i_A[2];

					j_sum_I->second[0] += j_I[0];
					j_sum_I->second[1] += j_I[1];
					j_sum_I->second[2] += j_I[2];
					j_sum_A->second[0] += j_A[0];
					j_sum_A->second[1] += j_A[1];
					j_sum_A->second[2] += j_A[2];

					ptr->bProcessed = true;
				}
			});
		}

		if (eq_rg.second == col_map.end())
			break;

		idx = eq_rg.second->first;
	}

	// ��ݷ�/����ݷ��� ���� ���� ��ġ/�ӵ�/���ӵ� ���
	auto it_sum_I = map_sum_I.begin();
	auto it_sum_A = map_sum_A.begin();
	for (; it_sum_I != map_sum_I.end(); ++it_sum_I, ++it_sum_A)
	{
		int i = it_sum_I->first;

		const Sphere &oldsp = m_Spheres[i];
		Sphere &newsp = NewSpheres[i];

		const std::array<double, 3> &pr_I = it_sum_I->second;
		const std::array<double, 3> &pr_A = it_sum_A->second;

		std::array<double, 3> dv = {
			pr_I[0] / oldsp.mass,
			pr_I[1] / oldsp.mass,
			pr_I[2] / oldsp.mass
		};

		double moment_of_inerita = 2 * oldsp.mass * square(oldsp.radius) / 5;
		std::array<double, 3> dw = {
			pr_A[0] / moment_of_inerita,
			pr_A[1] / moment_of_inerita,
			pr_A[2] / moment_of_inerita
		};

		double before_time = map_time[i];
		double after_time = LOGICAL_TIME_SPAN - before_time;

		// �浹 �������� ���
		// v = (v0 + v1)/2
		// RunForce() ����
		std::array<double, 3> old_v = {
			(oldsp.velocity[0] + newsp.velocity[0]) / 2,
			(oldsp.velocity[1] + newsp.velocity[1]) / 2,
			(oldsp.velocity[2] + newsp.velocity[2]) / 2,
		};
		newsp.coord[0] = oldsp.coord[0] + old_v[0] * before_time;
		newsp.coord[1] = oldsp.coord[1] + old_v[1] * before_time;
		newsp.coord[2] = oldsp.coord[2] + old_v[2] * before_time;

		// �ӵ��� ��ȭ��Ų �� ���� �ð��� ���� �ٽ� ������
		newsp.velocity[0] += dv[0];
		newsp.velocity[1] += dv[1];
		newsp.velocity[2] += dv[2];

		newsp.coord[0] += newsp.velocity[0] * after_time;
		newsp.coord[1] += newsp.velocity[1] * after_time;
		newsp.coord[2] += newsp.velocity[2] * after_time;

		// ���ӵ��� ���ؼ��� ������ ó��
		std::array<double, 3> old_w = {
			(oldsp.AngularVelocity[0] + newsp.AngularVelocity[0]) / 2,
			(oldsp.AngularVelocity[1] + newsp.AngularVelocity[1]) / 2,
			(oldsp.AngularVelocity[2] + newsp.AngularVelocity[2]) / 2,
		};

		newsp.angle[0] = oldsp.angle[0] + old_w[0] * before_time;
		newsp.angle[1] = oldsp.angle[1] + old_w[1] * before_time;
		newsp.angle[2] = oldsp.angle[2] + old_w[2] * before_time;

		newsp.AngularVelocity[0] += dw[0];
		newsp.AngularVelocity[1] += dw[1];
		newsp.AngularVelocity[2] += dw[2];

		newsp.angle[0] += newsp.AngularVelocity[0] * after_time;
		newsp.angle[1] += newsp.AngularVelocity[1] * after_time;
		newsp.angle[2] += newsp.AngularVelocity[2] * after_time;
	}
}

std::shared_ptr<SphereManager::CollisionInfo> SphereManager::DetectCollision(
	const std::vector<Sphere> &NewSpheres, int i, int j, double RemainTime) const
{
	const Sphere &oldsp_1 = m_Spheres[i];
	const Sphere &newsp_1 = NewSpheres[i];
	const Sphere &oldsp_2 = m_Spheres[j];
	const Sphere &newsp_2 = NewSpheres[j];

	// ����: p + tv (p=ó�� ��ġ, v=�ӵ�)
	// ���� �߽� �� �Ÿ�: |(p1 - p2) + t(v1 - v2)|
	// �浹�ÿ� r1+r2 =: k �� ������
	// p:=p1-p2, v:=v1-v2
	// |p + tv|^2 = k^2
	// |p|^2 + |v|^2 t^2 + 2t (p.v) = k^2
	// (|v|^2)t^2 + 2(p.v) t + |p|^2 - k^2 = 0
	// t = ( -(p.v) +- sqrt( (p.v)^2 - (|v||p|)^2 + k^2 |v|^2 ) ) / ( |v|^2 )

	CollisionInfo info;
	info.i = i;
	info.j = j;

	info.p[0] = oldsp_1.coord[0] - oldsp_2.coord[0];
	info.p[1] = oldsp_1.coord[1] - oldsp_2.coord[1];
	info.p[2] = oldsp_1.coord[2] - oldsp_2.coord[2];

	// v = (v0 + v1)/2
	// RunForce() ����
	info.v[0] = ((oldsp_1.velocity[0] + newsp_1.velocity[0]) - (oldsp_2.velocity[0] + newsp_2.velocity[0])) / 2;
	info.v[1] = ((oldsp_1.velocity[1] + newsp_1.velocity[1]) - (oldsp_2.velocity[1] + newsp_2.velocity[1])) / 2;
	info.v[2] = ((oldsp_1.velocity[2] + newsp_1.velocity[2]) - (oldsp_2.velocity[2] + newsp_2.velocity[2])) / 2;

	info.p_length_2 = get_length_2(info.p);
	info.v_length_2 = get_length_2(info.v);
	info.p_dot_v = dot_product(info.p, info.v);

	// ���� ����� ��ݷ��� ������ �������� ���� ���
	if (info.p_dot_v > EPSILON)	// �浹�� �Ͼ�� ���� (docs.md#��ݷ�#2 ����)
		return std::shared_ptr<CollisionInfo>();

	// �� ���� ������ ��
	double k = oldsp_1.radius + oldsp_2.radius;

	// �Ǻ��� < 0 �̸� �浹 ����
	// �Ǻ��� < epsilon (������ ��) �� �浹 ���� (docs.md#��ݷ�#2 ����)
	double D = info.p_dot_v * info.p_dot_v - info.p_length_2 * info.v_length_2 + square(k) * info.v_length_2;
	if (D < EPSILON)
		return std::shared_ptr<CollisionInfo>();

	double sqrt_D = sqrt(D);
	double t1 = (-info.p_dot_v + sqrt_D) / info.v_length_2;
	double t2 = (-info.p_dot_v - sqrt_D) / info.v_length_2;

	// ���� ������ �浹 ����
	if (t1 < 0)
		return std::shared_ptr<CollisionInfo>();

	// ó�� �浹�� �ð�

	// (|v|^2)t^2 + 2(p.v) t + |p|^2 - k^2 < 0 �� ���(Sphere �� ���� ��ģ ���)�� �浹�� ����
	// 2������ ��� |v|^2�� ��� ==> t2 < RemainTime < t1 �̿��� ��
	// t2�� ����̰� RemainTime���� �۴ٸ� t2���� ���� (��ġ�� ����)
	// t2�� ������� ��ĥ ���ɼ��� ����.
	bool possibly_overlapped = false;

	if (t2 < 0)
	{
		info.t = t1;
		possibly_overlapped = true;
	}
	else
	{
		info.t = t2;
	}
	//info.t = min(t1, t2);

	// ...�� RemainTime���� �۾ƾ� ��
	if (info.t > RemainTime)
	{
		if (possibly_overlapped)
		{
			// ������ -> 0�ʿ��� �浹�� ������ ó��
			info.t = 0;
		}
		else
		{
			// ��ġ�� �ʾ���
			return std::shared_ptr<CollisionInfo>();
		}
	}

	return std::make_shared<CollisionInfo>(info);
}

void SphereManager::Render()
{
	for (const Sphere s : m_Spheres)
	{
		glPushMatrix();
		glTranslated(s.coord[0], s.coord[1], s.coord[2]);
		glRotated(get_length(s.angle), s.angle[0], s.angle[1], s.angle[2]);
		glCallList(s.DisplayList);

		/* ȸ���� �� �Ǵ��� ���� ���� ������ �ڵ�
		glBegin(GL_TRIANGLES);
		{
			glColor3fv(s.color.data());
			glVertex3d(0, s.radius, 0);
			glColor3fv(s.color.data());
			glVertex3d(-(s.radius * 0.866), -(s.radius / 2), 0);
			glColor3fv(s.color.data());
			glVertex3d(s.radius * 0.866, -(s.radius / 2), 0);
		}
		*/

		glEnd();
		glPopMatrix();
	}
}

void Sphere::CompileDisplayList(GLUquadric *quadric)
{
	DisplayList = glGenLists(1);
	glNewList(DisplayList, GL_COMPILE);
	//glColor4fv(color.data());
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color.data());
	glMaterialfv(GL_FRONT, GL_SPECULAR, color.data());
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	gluSphere(quadric, radius, GLint(radius * 100), GLint(radius * 100));
	glEndList();
}

void Sphere::DeleteDisplayList()
{
	glDeleteLists(DisplayList, 1);
}


#ifdef _DEBUG

// ����� ����� �Լ�
std::pair<std::array<double, 3>, double> sum_momentum(const std::vector<Sphere> &spheres)
{
	std::array<double, 3> p = { 0, 0, 0 }, ap = { 0, 0, 0 };

	for (int i = 0; static_cast<size_t>(i) < spheres.size(); ++i)
	{
		const Sphere &sp = spheres[i];

		p[0] += sp.mass * sp.velocity[0];
		p[1] += sp.mass * sp.velocity[1];
		p[2] += sp.mass * sp.velocity[2];

		double moment_of_inertia = 2 * sp.mass * square(sp.radius) / 5;
		ap[0] += moment_of_inertia * sp.AngularVelocity[0];
		ap[1] += moment_of_inertia * sp.AngularVelocity[1];
		ap[2] += moment_of_inertia * sp.AngularVelocity[2];
	}

	return { p, get_length(p) };
}

#endif
