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
 * @file NonCopyable.h
 * @brief NonCopyable 클래스
 */

#pragma once

#include "Defines.h"

BEGIN_IKHWINLIB2()

/**
 * @brief 이 클래스를 <c>private</c> 상속받으면 복사 불가능해 집니다. 이 클래스는 @ref CObject 를 상속받지 않습니다.
 * @example NonCopyable_Example.cpp
 */
class NonCopyable
{
private:
	NonCopyable(const NonCopyable &) = delete;
	NonCopyable &operator =(const NonCopyable &) = delete;
protected:
	NonCopyable() = default;
	~NonCopyable() = default;
};

/**
 * @brief 이 클래스를 <c>private</c> 상속받으면 복사 및 이동 불가능해 집니다. 이 클래스는 @ref CObject 를 상속받지 않습니다.
 */
class NonCopyableMovable
{
private:
	NonCopyableMovable(const NonCopyableMovable &) = delete;
	NonCopyableMovable &operator =(const NonCopyableMovable &) = delete;
	NonCopyableMovable(NonCopyableMovable &&) = delete;
	NonCopyableMovable &operator =(NonCopyableMovable &&) = delete;
protected:
	NonCopyableMovable() = default;
	~NonCopyableMovable() = default;
};

END_IKHWINLIB2()
