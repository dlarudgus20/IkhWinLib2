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
#include "Shader.h"
#include "resource.h"

Shader::Shader()
{
	m_vertex = glCreateShader(GL_VERTEX_SHADER);
	auto vs = ReadFromResource(IDR_VERT_SHADER);
	glShaderSource(m_vertex, 1, &vs.first, &vs.second);
	glCompileShader(m_vertex);
	ShaderCompileErrorCheck(m_vertex, "GL_VERTEX_SHADER");

	m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	auto fs = ReadFromResource(IDR_FRAG_SHADER);
	glShaderSource(m_fragment, 1, &fs.first, &fs.second);
	glCompileShader(m_fragment);
	ShaderCompileErrorCheck(m_fragment, "GL_FRAGMENT_SHADER");

	m_program = glCreateProgram();

	glAttachShader(m_program, m_vertex);
	glAttachShader(m_program, m_fragment);

	glLinkProgram(m_program);
	ProgramErrorCheck(m_program);

	glUseProgram(m_program);
}

Shader::~Shader()
{
	glUseProgram(0);
	glDetachShader(m_program, m_fragment);
	glDetachShader(m_program, m_vertex);

	glDeleteShader(m_fragment);
	glDeleteShader(m_vertex);
	glDeleteProgram(m_program);
}

std::pair<const char *, GLint> Shader::ReadFromResource(UINT id)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	HRSRC hRSrc = FindResource(hInst, MAKEINTRESOURCE(id), L"SHADER");
	DWORD size = SizeofResource(hInst, hRSrc);
	HGLOBAL hMem = LoadResource(hInst, hRSrc);
	const char *ptr = reinterpret_cast<const char *>(LockResource(hMem));
	return { ptr, size };
}

void Shader::ShaderCompileErrorCheck(GLuint shader, const std::string &name)
{
	GLint bCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bCompiled);

	GLint len;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

	std::string str;

	if (len > 1) // '\0' character
	{
		str.resize(len);
		GLsizei cWritten;
		glGetShaderInfoLog(shader, len, &cWritten, &str[0]);
	}
	if (bCompiled == GL_FALSE)
	{
		OutputDebugStringA(name + ": error\n");
		if (len > 1)
		{
			OutputDebugStringA("========\n");
			OutputDebugStringA(str);
			OutputDebugStringA("========\n");
		}
		throw std::runtime_error("shader compilation is failed in '" + name + "'");
	}
	else if (len > 1)
	{
		OutputDebugStringA(name + ": info\n");
		OutputDebugStringA("========\n");
		OutputDebugStringA(str);
		OutputDebugStringA("========\n");
	}
}

void Shader::ProgramErrorCheck(GLuint prog)
{
	GLint bLinked;
	glGetProgramiv(prog, GL_LINK_STATUS, &bLinked);

	GLint len;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

	std::string str;

	if (len > 1) // '\0' character
	{
		str.resize(len);
		GLsizei cWritten;
		glGetProgramInfoLog(prog, len, &cWritten, &str[0]);
	}
	if (bLinked == GL_FALSE)
	{
		OutputDebugStringA("shader program linking error\n");
		if (len > 1)
		{
			OutputDebugStringA("========\n");
			OutputDebugStringA(str);
			OutputDebugStringA("========\n");
		}
		throw std::runtime_error("shader program linking is failed");
	}
	else if (len > 1)
	{
		OutputDebugStringA("shader program info\n");
		OutputDebugStringA("========\n");
		OutputDebugStringA(str);
		OutputDebugStringA("========\n");
	}
}
