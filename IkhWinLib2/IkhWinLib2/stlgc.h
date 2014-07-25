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

/**
 * @file stlgc.h
 * @brief boehm-gc의 할당자를 사용하는 표준 STL 컨테이너/컨테이너 어댑터들과 stringstream의 typedef 모음입니다.
 */

#pragma once

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <queue>
#include <stack>

#include <string>

/**
 * @brief boehm-gc의 할당자를 사용하는 표준 STL 컨테이너/컨테이너 어댑터들과 stringstream의 typedef 모음입니다.
 */
namespace stlgc
{
	template <typename T> using deque				= std::deque<T, gc_allocator<T> >;
	template <typename T> using forward_list		= std::forward_list<T, gc_allocator<T> >;
	template <typename T> using list				= std::list<T, gc_allocator<T> >;
	template <typename T> using map					= std::map<T, gc_allocator<T> >;
	template <typename T> using set					= std::set<T, gc_allocator<T> >;
	template <typename T> using multimap			= std::multimap<T, gc_allocator<T> >;
	template <typename T> using multiset			= std::multiset<T, gc_allocator<T> >;
	template <typename T> using unordered_map		= std::unordered_map<T, gc_allocator<T> >;
	template <typename T> using unordered_set		= std::unordered_set<T, gc_allocator<T> >;
	template <typename T> using unordered_multimap	= std::unordered_multimap<T, gc_allocator<T> >;
	template <typename T> using unordered_multiset	= std::unordered_multiset<T, gc_allocator<T> >;
	template <typename T> using vector				= std::vector<T, gc_allocator<T> >;

	template <typename T> using queue				= std::queue<T, stlgc::deque<T> >;
	template <typename T> using stack				= std::stack<T, stlgc::deque<T> >;

	template <typename Elem, typename Traits = std::char_traits<Elem> >
	using basic_string = std::basic_string<Elem, Traits, gc_allocator<Elem> >;
	using string = stlgc::basic_string<char>;
	using wstring = stlgc::basic_string<wchar_t>;
#ifndef _MSC_VER
	using u16string = stlgc::basic_string<char16_t>;
	using u32string = stlgc::basic_string<char32_t>;
#endif

	template <typename Elem, typename Traits = std::char_traits<Elem> >
	using basic_stringstream = std::basic_stringstream<Elem, Traits, gc_allocator<Elem> >;
	using stringstream = stlgc::basic_stringstream<char>;
	using wstringstream = stlgc::basic_stringstream<wchar_t>;
}
