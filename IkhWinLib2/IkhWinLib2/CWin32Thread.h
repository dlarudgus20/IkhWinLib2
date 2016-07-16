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

#pragma once

#include "CObject.h"

#include "FnEvent.h"
#include "CSystemError.h"

BEGIN_IKHWINLIB2()

/**
 * @brief @ref CApplication::Poll 함수가 리턴하는 열거형입니다.
 * @see CApplication::Poll
 */
enum class PollReturn
{
	NoMessage,		/**< 메시지 큐에 메시지가 없습니다. */
	NormalMessage,	/**< 일반적인 메시지를 처리했습니다. */
	GlobalMessage,	/**< <c>PostMessage(NULL, </c>이나 <c>PostThreadMessage</c>로 스레드에 전달된 메시지입니다. */
};

/**
 * @brief Win32 스레드를 나타내는 클래스입니다.
 */
class CWin32Thread : public CObject, private boost::noncopyable
{
	friend class CApplication;
	friend class CWin32AttachThread;
private:
	HANDLE m_hThread;
	DWORD m_id;

	HACCEL m_hAccel = nullptr;

	explicit CWin32Thread() NOEXCEPT;
public:
	CWin32Thread(CWin32Thread &&other);
	CWin32Thread &operator =(CWin32Thread &&other);
	void swap(CWin32Thread &other) NOEXCEPT;

	explicit CWin32Thread(unsigned(__stdcall *fn)(void *param), void *lpParam = nullptr);

	template <typename F, typename ...Args>
	explicit CWin32Thread(F&& f, Args&&... args);

	template <typename F, typename ...Args>
	static CWin32Thread FromFunction(F f, Args&&... args)
	{
		return FromFunctionImpl(std::bind(f, std::forward<Args>(args)...));
	}

	virtual ~CWin32Thread() NOEXCEPT;

	CWin32Thread &Attach(HANDLE hThread) NOEXCEPT;
	void Deattach() NOEXCEPT;

	HANDLE GetHandle() const NOEXCEPT;
	DWORD GetId() const NOEXCEPT;

	/**
	 * @brief 메시지 큐를 검사하고 메시지가 있으면 처리합니다.
	 * @param[out] msg 메시지가 있다면 처리된 메시지가 리턴됩니다.
	 * @param[in] bCallOnIdle 이 값이 true이면 메시지 큐가 비었을 경우 @ref OnIdle 가상 함수를 호출합니다.
	 * @return 어떤 메시지를 처리했는지 알려주는 @ref PollReturn 열거형입니다.
	 * @see PollReturn
	 */
	PollReturn Poll(MSG &msg, bool bCallOnIdle = true) NOEXCEPT;

	/**
	* @brief 메시지 루프를 수행합니다.
	* @param[in] hAccel 액셀러레이터의 핸들입니다. 없을 경우 nullptr입니다.
	*/
	WPARAM Run(HACCEL hAccel = nullptr) NOEXCEPT;
	WPARAM Run(LPCTSTR lpAccel) NOEXCEPT;

	/**
	* @brief 모달리스 대화상자를 등록/해제합니다.
	* @param[in] hModeless nullptr가 아닐 경우 모달리스 대화상자를 등록합니다. nullptr일 경우 등록 해제합니다.
	*/
	void SetModeless(HWND hModeless) NOEXCEPT;

	template <typename F, typename ...Args>
	boost::future<typename std::result_of<F(Args...)>::type>
		Post(F f, Args&&... args)
	{
		typedef typename std::result_of<F(Args...)>::type result_type;
		return PostImpl<result_type>(
			std::function<result_type()>(std::bind(f, std::forward<Args>(args)...))
			);
	}

private:
	static CWin32Thread FromFunctionImpl(const std::function<void()> &f);

	template <typename Ret>
	boost::future<Ret> PostImpl(const std::function<Ret()> &f);

	struct PostImplStructBase
	{
		virtual void run() = 0;
	};
	template <typename Ret>
	struct PostImplStruct final : public PostImplStructBase
	{
		std::function<Ret()> func;
		boost::promise<Ret> promise;
		explicit PostImplStruct(std::function<Ret()> f)
			: func(std::move(f)) 
		{
		}
		virtual void run() override
		{
			try
			{
				promise.set_value(func());
			}
			catch (...)
			{
				promise.set_exception(std::current_exception());
			}
		}
	};
	template <>
	struct PostImplStruct<void> final : public PostImplStructBase
	{
		std::function<void()> func;
		boost::promise<void> promise;
		explicit PostImplStruct(std::function<void()> f)
			: func(std::move(f))
		{
		}
		virtual void run() override
		{
			try
			{
				func();
				promise.set_value();
			}
			catch (...)
			{
				promise.set_exception(std::current_exception());
			}
		}
	};

	static UINT ms_WM_POST;
	static void StaticCtor() NOEXCEPT;

	friend class CMsgTarget;
	static void OnUnexpectedException(const std::exception_ptr &eptr) NOEXCEPT;

public:
	/**
	* @brief 아이들 타임에 호출되는 이벤트입니다.
	* @see OnIdle
	*/
	FnEvent<CWin32Thread, void()> evtIdle;
	/**
	* @brief <c>PostMessage(NULL, </c>이나 <c>PostThreadMessage</c>로 스레드에 전달된 메시지를 받으면 호출되는 이벤트입니다.
	* @param[in] msg 받은 메시지입니다.
	* @see OnGlobalMsg
	*/
	FnEvent<CWin32Thread, void(const MSG &msg)> evtGlobalMsg;

protected:
	/**
	* @brief 아이들 타임에 호출되는 가상 함수입니다.
	* @remarks 디폴트 동작은 @ref Idle 이벤트의 핸들러가 없을 경우 생성자에 전달된 <c>bNoCollectOnIdle</c>이 <c>false</c>라면 쓰레기 수집을 조금 수행합니다. 있을 경우 Idle 이벤트를 호출합니다. 그 후에 <c>WaitMessage</c> 함수를 호출해 대기합니다.
	* @see evtIdle
	*/
	virtual void OnIdle();
	/**
	* @brief <c>PostMessage(NULL, </c>이나 <c>PostThreadMessage</c>로 스레드에 전달된 메시지를 받으면 호출되는 가상 함수입니다.
	* @param[in] msg 받은 메시지입니다.
	* @remarks 디폴트 동작은 @ref GlobalMsg 이벤트를 호출합니다.
	* @see evtGlobalMsg
	*/
	virtual void OnGlobalMsg(const MSG &msg);
};

inline CWin32Thread::CWin32Thread() NOEXCEPT
	: m_hThread(GetCurrentThread()), m_id(GetCurrentThreadId())
{

}

inline CWin32Thread::CWin32Thread(CWin32Thread &&other)
	: evtIdle { std::move(other.evtIdle) }
	, evtGlobalMsg { std::move(other.evtGlobalMsg) }
{
	m_hThread = other.m_hThread;
	other.m_hThread = nullptr;
	m_id = other.m_id;
	other.m_id = 0xffffffff;
	m_hAccel = other.m_hAccel;
	other.m_hAccel = nullptr;
}
inline CWin32Thread &CWin32Thread::operator =(CWin32Thread &&other)
{
	m_hThread = other.m_hThread;
	other.m_hThread = nullptr;
	m_id = other.m_id;
	other.m_id = 0xffffffff;
	m_hAccel = other.m_hAccel;
	other.m_hAccel = nullptr;
	evtIdle = std::move(other.evtIdle);
	evtGlobalMsg = std::move(other.evtGlobalMsg);
}
inline void CWin32Thread::swap(CWin32Thread &other) NOEXCEPT
{
	std::swap(m_hThread, other.m_hThread);
	std::swap(m_id, other.m_id);
	std::swap(m_hAccel, other.m_hAccel);
	evtIdle.swap(other.evtIdle);
	evtGlobalMsg.swap(other.evtGlobalMsg);
}

inline CWin32Thread::CWin32Thread(unsigned (__stdcall *fn)(void *param), void *lpParam /* = nullptr*/)
{
	unsigned id;
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, fn, lpParam, 0, &id);
	if (m_hThread == nullptr)
		throw CThreadCreationError();

	m_id = (DWORD)id;
}

template <typename F, typename ...Args>
inline CWin32Thread::CWin32Thread(F&& f, Args&&... args)
{
	try
	{
		std::thread thrd(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		m_hThread = static_cast<HANDLE>(thrd.native_handle());
		m_id = GetThreadId(m_hThread);
		thrd.detach();
	}
	catch (std::system_error &)
	{
		throw CThreadCreationError();
	}
}

inline CWin32Thread &CWin32Thread::Attach(HANDLE hThread) NOEXCEPT
{
	m_hThread = hThread;
	return *this;
}
inline void CWin32Thread::Deattach() NOEXCEPT
{
	m_hThread = nullptr;
}

inline HANDLE CWin32Thread::GetHandle() const NOEXCEPT
{
	return m_hThread;
}
inline DWORD CWin32Thread::GetId() const NOEXCEPT
{
	return m_id;
}

inline WPARAM CWin32Thread::Run(LPCTSTR lpAccel) NOEXCEPT
{
	return Run(LoadAccelerators(GetModuleHandle(NULL), lpAccel));
}

template <typename Ret>
boost::future<Ret> CWin32Thread::PostImpl(const std::function<Ret()> &f)
{
	auto *pf = new PostImplStruct<Ret>(f);
	if (!PostThreadMessage(m_id, ms_WM_POST, 0, (LPARAM)pf))
	{
		delete pf;
		throw CPostMsgError();
	}
	else
	{
		return pf->promise.get_future();
	}
}

END_IKHWINLIB2()
