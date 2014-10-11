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
 * @file Defines.h
 * @brief IkhWinLib2의 기본 헤더 파일입니다.
 */

#pragma once

// Include Headers
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <process.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
#include <assert.h>

#include <iostream>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>
#include <fstream>

#include <string>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <stack>
#include <queue>

#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <memory>
#include <utility>
#include <future>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <new>
#include <codecvt>
using std::swap;

#ifndef IKHWINLIB2_USE_BOOST_PLACEHOLDER
using namespace std::placeholders;
#define BOOST_BIND_NO_PLACEHOLDERS
#endif

#define BOOST_THREAD_VERSION 4
#define BOOST_RESULT_OF_USE_DECLTYPE
#include <boost/thread/future.hpp>
#include <boost/scope_exit.hpp>
#include <boost/optional.hpp>
#include <boost/format.hpp>
#include <boost/scope_exit.hpp>
#include <boost/bind.hpp>

#ifdef _MSC_VER
# define NOEXCEPT throw()
#else
# define NOEXCEPT noexcept
#endif

// IkhWinLib2 Namespace Macro
#define BEGIN_IKHWINLIB2() \
	namespace IkhProgram { \
		namespace IkhWinLib2 {

#define END_IKHWINLIB2() \
		} \
	}

/**
 * @namespace IkhProgram::IkhWinLib2
 * @brief IkhWinLib2의 기본 네임스페이스입니다.
 */

#include "Utility.h"
