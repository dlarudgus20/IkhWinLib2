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

#include "Defines.h"

BEGIN_IKHWINLIB2()

class GCThreadInitor final
{
private:
	GC_stack_base m_sb;
public:
	GCThreadInitor()
	{
		GC_get_stack_base(&m_sb);
		GC_register_my_thread(&m_sb);
	}
	~GCThreadInitor()
	{
		GC_unregister_my_thread();
	}
};

template <typename F> class _wrapGCThreadobj
{
private:
	F m_f;
public:
	explicit _wrapGCThreadobj(F f) : m_f(f) { }

	template <typename ...Args>
	auto operator ()(Args&&... args)
		-> decltype(m_f(std::forward<Args>(args)...))
	{
		GCThreadInitor _gcthi;
		return m_f(std::forward<Args>(args)...);
	}
};
template <typename F> _wrapGCThreadobj<F> WrapGCThread(F f)
{
	return _wrapGCThreadobj<F>(f);
}

// GCPtr<std::string> *ptr = GCNew<std::string>("asdf"); // only class type; not primitive
//  or
// std::string *ptr = GCNew<std::string>("asdf");
//
//  If you need destroy object immediately,
// GCPtr<std::string> *ptr = ...; // important: type of ptr must be GCPtr<>
// delete ptr;
//
//  (In Visual C++) warning C4520 can be ignored.
template <class T> class GCPtr final
	: public T, public virtual gc_cleanup
{
private:
	template <typename ...Args>
	explicit GCPtr(Args&&... args) : T(std::forward<Args>(args)...) { }

#ifdef _MSC_VER
	GCPtr() { }	// without this, compiler crashes
#endif

public:
	template <bool primitive, bool is_gcclean, typename U, typename ...Args> friend struct _gcNew_t;
};

template <bool primitive, bool is_gcclean, typename T, typename ...Args> struct _gcNew_t;
template <typename T, typename ...Args> struct _gcNew_t<false, false, T, Args...>
{
	static GCPtr<T> *_f(Args&&... args)
	{
		return new (GC)GCPtr<T>(std::forward<Args>(args)...);
	}
};
template <typename T, typename ...Args> struct _gcNew_t<false, true, T, Args...>
{
	static T *_f(Args&&... args)
	{
		return new (GC)T(std::forward<Args>(args)...);
	}
};
template <typename T, typename ...Args> struct _gcNew_t<true, false, T, Args...>
{
	static T *_f(Args&&... args)
	{
		return new (GC)T(std::forward<Args>(args)...);
	}
};
#define IKHWINLIB2_GC_NEW_HELPER() \
	_gcNew_t<std::is_fundamental<T>::value, \
		std::is_assignable<const gc_cleanup *, std::remove_reference<T> *>::value, \
		T, Args...>::_f(std::forward<Args>(args)...)
template <typename T, typename ...Args> auto GCNew(Args&&... args)
	-> decltype(IKHWINLIB2_GC_NEW_HELPER())
{
	return IKHWINLIB2_GC_NEW_HELPER();
}

// std::string str;
// void func1(C *ptr_with_gc);
// void func2(GCPtr<C> *ptr_with_gc);
// func1(GCBox(str));
// func2(GCBox(str));
template <typename T> auto GCBox(T &&t)
	-> decltype(GCNew<std::remove_reference<T>::type>(std::forward<T>(t)))
{
	return GCNew<std::remove_reference<T>::type>(std::forward<T>(t));
}

END_IKHWINLIB2()
