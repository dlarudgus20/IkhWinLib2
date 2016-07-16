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

#pragma once

namespace parse_utils
{
	template <typename F>
	std::array<double, 3> parse_vec3(const std::wstring &str, F error_handler);

	template <typename F>
	double parse_real(const std::wstring &str, F error_handler);

	template <typename F>
	std::array<float, 4> parse_rgba(const std::wstring &str, F error_handler);

	template <typename F>
	bool parse_bool(const std::wstring &str, F error_handler);

	template <typename T, typename F>
	void parse_tuple(const std::wstring &str, T *buf, size_t count, F error_handler);

	inline std::wstring tostr_vec3(const std::array<double, 3> &vec3)
	{
		return L'('
			+ std::to_wstring(vec3[0])
			+ L',' + std::to_wstring(vec3[1])
			+ L',' + std::to_wstring(vec3[2])
			+ L')';
	}

	inline std::wstring tostr_real(double real)
	{
		return std::to_wstring(real);
	}

	inline std::wstring tostr_rgba(const std::array<float, 4> rgba)
	{
		return L'('
			+ std::to_wstring(rgba[0])
			+ L',' + std::to_wstring(rgba[1])
			+ L',' + std::to_wstring(rgba[2])
			+ L',' + std::to_wstring(rgba[3])
			+ L')';
	}

	inline std::wstring tostr_bool(bool b)
	{
		return b ? L"true" : L"false";
	}
}

#include "parse_utils_detail.h"
