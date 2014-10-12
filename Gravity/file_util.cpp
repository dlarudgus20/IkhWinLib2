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
#include "file_util.h"

namespace file_util
{
	std::wifstream open_input_file(const std::wstring &fname, bool *is_empty /* = nullptr*/)
	{
		return open_input_file(std::wifstream { fname, std::ios::in | std::ios::binary }, is_empty);
	}
	std::wifstream open_input_file(const std::string &fname, bool *is_empty /* = nullptr*/)
	{
		return open_input_file(std::wifstream { fname, std::ios::in | std::ios::binary }, is_empty);
	}
	std::wifstream open_input_file(std::wifstream f, bool *is_empty /* = nullptr*/)
	{
		bool tmp;
		if (is_empty == nullptr)
			is_empty = &tmp;	// trick for no-checking null

		f.exceptions(std::ios::failbit | std::ios::badbit);

		if (f.peek() == std::wifstream::traits_type::eof())
		{
			*is_empty = true;
			f.clear();
			goto AssumeFormat;
		}
		else
		{
			*is_empty = false;
		}

		wchar_t ch = f.get();
		if (ch == L'\xef')
		{
			ch = f.get();
			wchar_t ch2 = f.get();
			if (ch != L'\xbb' || ch2 != L'\xbf')
				throw CFileUtilException(L"encoding error");

			f.imbue(
				std::locale(f.getloc(), new std::codecvt_utf8<wchar_t>())
				);
		}
		else
		{
			if (ch == L'\xfe' || ch == L'\xff')
				throw CFileUtilException(L"utf-16/32 is not supported");

		AssumeFormat:
			f.seekg(0, std::ios::beg);

			//if (assume_default_is_ansi)
			//{
			//	// assume Ansi
			//	// it's not implemented yet, so ignore option and run UTF-8
			//}
			//else
			{
				// assume UTF-8
				f.imbue(
					std::locale(f.getloc(), new std::codecvt_utf8<wchar_t>())
					);
			}
		}
		return f;
	}

	std::wofstream open_output_file(const std::wstring &fname)
	{
		return open_output_file(std::wofstream { fname,
			std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary });
	}
	std::wofstream open_output_file(const std::string &fname)
	{
		return open_output_file(std::wofstream { fname,
			std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary });
	}
	std::wofstream open_output_file(std::wofstream f)
	{
		f.exceptions(std::ios::failbit | std::ios::badbit);
		//f.write(L"\xef\xbb\xbf", 3); // utf8 BOM

		f.imbue(
			std::locale(f.getloc(), new std::codecvt_utf8<wchar_t>())
			);
		return f;
	}
}
