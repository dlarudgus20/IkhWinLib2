// Copyright (c) 2014, ÀÓ°æÇö
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
#include "Projection.h"

const double Projection::m_OriOrthoRect[4] = { -4000, 4000, -4000, 4000 };
const double Projection::m_OriOrthoNear = -100000;
const double Projection::m_OriOrthoFar = 100000;

const double Projection::m_OriPersFovy = 43;
const double Projection::m_OriPersNear = 1;
const double Projection::m_OriPersFar = 300001;

Projection::Projection()
	: m_bOrtho(false)
{
	memcpy(m_OrthoRect, m_OriOrthoRect, sizeof(m_OrthoRect));
	m_OrthoNear = m_OriOrthoNear;
	m_OrthoFar = m_OriOrthoFar;

	m_PersFovy = m_OriPersFovy;
	m_PersAspect = 1.0f;
	m_PersNear = m_OriPersNear;
	m_PersFar = m_OriPersFar;
}

void Projection::SizeChanged(int cx, int cy)
{
	m_cx = cx;
	m_cy = cy;
}

void Projection::UseOrtho(bool bOrtho /* = true */)
{
	m_bOrtho = bOrtho;
}

void Projection::Apply() const
{
	static const double inch = 10.0;

	glMatrixMode(GL_PROJECTION);

	if (m_cx != 0 && m_cy != 0)
		glScaled(inch / m_cx, inch / m_cy, 1);

	if (m_bOrtho)
	{
		glOrtho(m_OrthoRect[0], m_OrthoRect[1], m_OrthoRect[2], m_OrthoRect[3],
			m_OrthoNear, m_OrthoFar);
	}
	else
	{
		gluPerspective(m_PersFovy, m_PersAspect, m_PersNear, m_PersFar);

		glMatrixMode(GL_MODELVIEW);
		glTranslated(0, 0, -10001);
	}
}
