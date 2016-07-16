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
 * @file FnEvent.h
 * @brief FnEvent 클래스
 */

#pragma once

#include "CLogicError.h"

BEGIN_IKHWINLIB2()

/**
 * @brief 이벤트를 나타내는 클래스입니다. 이 클래스는 @ref CObject 를 상속받지 않습니다.
 * @tparam Owner 이 이벤트를 가지는 객체의 타입입니다.
 * @tparam F 이벤트 함수의 타입입니다. 반환형이 void여야 합니다.
 */
template <typename Owner, typename F> class FnEvent;

template <typename F> class EventFnPtr;


/**
 * @brief <c>FnEvent</c>의 실제 구현입니다.
 */
template <typename Owner, typename ...Args>
class FnEvent<Owner, void(Args...)> final : private boost::noncopyable
{
	friend Owner;
public:
	typedef void FnType(Args...);
	typedef std::function<FnType> FnObjType;

private:
	std::vector<FnObjType> _m_lstFunc;
	std::vector<std::weak_ptr<FnObjType> > _m_lstFuncPtr;
	std::shared_ptr<std::recursive_mutex> _m_pmu;

public:
	FnEvent() : _m_pmu(std::make_shared<std::recursive_mutex>()) { }

	// thread unsafe
	FnEvent(FnEvent &&other)
		: _m_lstFunc(std::move(other._m_lstFunc))
		, _m_lstFuncPtr(std::move(other._m_lstFuncPtr))
		, _m_pmu(std::move(other._m_pmu))
	{
	}
	template <typename O>
	FnEvent(FnEvent<O, void(Args...)> &&other)
		: _m_lstFunc(std::move(other._m_lstFunc))
		, _m_lstFuncPtr(std::move(other._m_lstFuncPtr))
		, _m_pmu(std::move(other._m_pmu))
	{
	}
	FnEvent &operator =(FnEvent &&other)
	{
		_m_lstFunc = std::move(other._m_lstFunc);
		_m_lstFuncPtr = std::move(other._m_lstFuncPtr);
		_m_pmu = std::move(other._m_pmu);
		return *this;
	}
	template <typename O>
	FnEvent &operator =(FnEvent<O, void(Args...)> &&other)
	{
		_m_lstFunc = std::move(other._m_lstFunc);
		_m_lstFuncPtr = std::move(other._m_lstFuncPtr);
		_m_pmu = std::move(other._m_pmu);
		return *this;
	}
	void swap(FnEvent &other) NOEXCEPT
	{
		_m_lstFunc.swap(other._m_lstFunc);
		_m_lstFuncPtr.swap(other._m_lstFuncPtr);
		_m_pmu.swap(other._m_pmu);
	}
	template <typename O>
	void swap(FnEvent<O, void(Args...)> &other) NOEXCEPT
	{
		_m_lstFunc.swap(other._m_lstFunc);
		_m_lstFuncPtr.swap(other._m_lstFuncPtr);
		_m_pmu.swap(other._m_pmu);
	}

	/**
	 * @brief 이벤트에 핸들러를 추가합니다.
	 * @tparam F 핸들러의 타입입니다.
	 * @param[in] fn 추가할 핸들러입니다.
	 */
	template <typename F, typename = std::enable_if<
			std::is_convertible<std::remove_reference<F>::type, std::weak_ptr<FnObjType> >
			>::type
		>
	FnEvent &operator +=(F fn)
	{
		SYN_LOCK_GUARD(*_m_pmu);
		_m_lstFunc.push_back(
			FnObjType(std::move(fn))
			);
		return *this;
	}
	FnEvent &operator +=(std::weak_ptr<FnObjType> fn);

	/**
	 * @brief 이벤트에서 함수 포인터형 핸들러를 제거합니다.
	 * @param[in] 제거할 함수 포인터형 핸들러입니다.
	 * @exception CInvalidArgError 제거할 핸들러가 없습니다.
	 */
	FnEvent &operator -=(FnObjType *fn);

	FnEvent &operator +=(EventFnPtr<void(Args...)> &efp);
	FnEvent &operator -=(EventFnPtr<void(Args...)> &efp);

	/**
	 * @brief 이벤트에서 핸들러를 모두 제거합니다.
	 */
	void Clear()
	{
		SYN_LOCK_GUARD(*_m_pmu);
		_m_lstFunc.clear();
		_m_lstFuncPtr.clear();
	}

	/**
	* @brief 이벤트 핸들러가 없는지 조사합니다.
	* @return 이벤트 핸들러가 없을 경우 true입니다.
	*/
	bool IsEmpty()
	{
		SYN_LOCK_GUARD(*_m_pmu);
		return _m_lstFunc.empty() && _m_lstFuncPtr.empty();
	}

private:
	/**
	 * @brief 이벤트의 핸들러를 호출합니다.
	 */
	template <typename ...Args2>
	void operator() (Args2&&... args)
	{
		SYN_LOCK_GUARD(*_m_pmu);

		_m_lstFuncPtr.erase(
			std::remove_if(_m_lstFuncPtr.begin(), _m_lstFuncPtr.end(),
				[](const std::weak_ptr<FnObjType> &fn) { return fn.expired(); }
				),
			_m_lstFuncPtr.end()
			);

		for (const std::function<FnType> &fn : _m_lstFunc)
		{
			fn(std::forward<Args2>(args)...);
		}
		for (const std::weak_ptr<FnObjType> &fn : _m_lstFuncPtr)
		{
			auto sptr = fn.lock();
			if (sptr)
				(*sptr.get())(std::forward<Args2>(args)...);
		}
	}
};

template <typename ...Args>
class EventFnPtr<void(Args...)>
{
public:
	typedef void FnType(Args...);
	typedef std::function<FnType> FnObjType;

private:
	std::shared_ptr<FnObjType> m_pfn;
	std::weak_ptr<std::recursive_mutex> _m_pmu;
public:
	EventFnPtr() = default;
	template <typename F> EventFnPtr(F &&f)
		: m_pfn(std::make_shared<FnObjType>(std::forward<F>(f)))
	{

	}
	template <typename F> EventFnPtr &set(F &&f)
	{
		m_pfn = std::make_shared<FnObjType>(std::forward<F>(f));
		return *this;
	}

	~EventFnPtr() NOEXCEPT
	{
		auto sp = _m_pmu.lock();
		if (sp)
		{
			SYN_LOCK_GUARD(*sp);
			m_pfn.reset();
		}
	}
	const std::shared_ptr<FnObjType> &get() const
	{
		return m_pfn;
	}

	void _mutex_set(const std::shared_ptr<std::recursive_mutex> &_mu)
	{
		_m_pmu = _mu;
	}
};

template <typename Owner, typename ...Args>
FnEvent<Owner, void(Args...)> &FnEvent<Owner, void(Args...)>::operator +=(
	std::weak_ptr<typename FnEvent<Owner, void(Args...)>::FnObjType> fn)
{
	SYN_LOCK_GUARD(*_m_pmu);
	_m_lstFuncPtr.push_back(std::move(fn));
	return *this;
}

template <typename Owner, typename ...Args>
FnEvent<Owner, void(Args...)> &FnEvent<Owner, void(Args...)>::operator -=(
	typename FnEvent<Owner, void(Args...)>::FnObjType *fn)
{
	SYN_LOCK_GUARD(*_m_pmu);

	bool succeeded = false;
	auto it = std::remove_if(_m_lstFuncPtr.begin(), _m_lstFuncPtr.end(),
		[fn, succeeded](const std::weak_ptr<FnObjType> &fn) {
			auto sptr = fn.lock();

			if (sptr.get() == fn)	// found
				return (succeeded = true);
			else if (!sptr)			// null
				return true;
			else					// otherwise
				return false;
		});
	_m_lstFuncPtr.erase(it, _m_lstFuncPtr.end());

	if (!succeeded)
		throw CInvalidArgError();

	return *this;
}

template <typename Owner, typename ...Args>
FnEvent<Owner, void(Args...)> &FnEvent<Owner, void(Args...)>::operator +=(
	EventFnPtr<typename FnEvent<Owner, void(Args...)>::FnType> &fn)
{
	SYN_LOCK_GUARD(*_m_pmu);
	fn._mutex_set(_m_pmu);
	*this += fn.get();
	return *this;
}

template <typename Owner, typename ...Args>
FnEvent<Owner, void(Args...)> &FnEvent<Owner, void(Args...)>::operator -=(
	EventFnPtr<typename FnEvent<Owner, void(Args...)>::FnType> &fn)
{
	SYN_LOCK_GUARD(*_m_pmu);
	// fn.get() <- std::shared_ptr<FnObjType>
	// fn.get().get() <- FnObjType *
	*this -= fn.get().get();
	return *this;
}

END_IKHWINLIB2()
