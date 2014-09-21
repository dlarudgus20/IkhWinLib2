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
	namespace detail
	{
		wchar_t find_first_letter(const wchar_t **pptr);
	}

	template <typename F>
	std::array<double, 3> parse_vec3(const std::wstring &str, F error_handler)
	{
		std::array<double, 3> ret;
		parse_tuple(str, ret.data(), 3, error_handler);
		return ret;
	}

	template <typename F>
	double parse_real(const std::wstring &str, F error_handler)
	{
		const wchar_t *endptr;
		double ret = wcstod(str.c_str(), const_cast<wchar_t **>(&endptr));

		if (detail::find_first_letter(&endptr) != L'\0')
			error_handler(str);

		return ret;
	}

	template <typename F>
	std::array<float, 4> parse_rgba(const std::wstring &str, F error_handler)
	{
		std::array<float, 4> ret;
		parse_tuple(str, ret.data(), 4, error_handler);
		return ret;
	}

	namespace detail
	{
		template <typename T>
		struct strto_func;

		template <>
		struct strto_func < double >
		{
			static double f(const wchar_t *str, wchar_t **endptr)
			{
				return wcstod(str, endptr);
			}
		};
		template <>
		struct strto_func < float >
		{
			static float f(const wchar_t *str, wchar_t **endptr)
			{
				return wcstof(str, endptr);
			}
		};
	}
	template <typename T, typename F>
	void parse_tuple(const std::wstring &str, T *buf, size_t count, F error_handler)
	{
		const wchar_t *endptr = str.c_str();

		if (detail::find_first_letter(&endptr) != L'(')
			error_handler(str);
		endptr++;

		for (T *p = buf; p < buf + count; ++p)
		{
			*p = detail::strto_func<T>::f(endptr, const_cast<wchar_t **>(&endptr));
			wchar_t ch = detail::find_first_letter(&endptr);
			endptr++;

			if (ch == L')' && ++p == buf + count)
			{
				if (detail::find_first_letter(&endptr) != L'\0')
					error_handler(str);
				return;
			}
			else if (ch != L',')
			{
				error_handler(str);
			}
		}

		error_handler(str);
	}
}
