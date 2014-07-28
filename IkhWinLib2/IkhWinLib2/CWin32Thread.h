// Copyright (c) 2014, �Ӱ���
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
#include "NonCopyable.h"

#include "FnEvent.h"
#include "CSystemError.h"

BEGIN_IKHWINLIB2()

/**
 * @brief @ref CApplication::Poll �Լ��� �����ϴ� �������Դϴ�.
 * @see CApplication::Poll
 */
enum class PollReturn
{
	NoMessage,		/**< �޽��� ť�� �޽����� �����ϴ�. */
	NormalMessage,	/**< �Ϲ����� �޽����� ó���߽��ϴ�. */
	GlobalMessage,	/**< <c>PostMessage(NULL, </c>�̳� <c>PostThreadMessage</c>�� �����忡 ���޵� �޽����Դϴ�. */
};

/**
 * @brief Win32 �����带 ��Ÿ���� Ŭ�����Դϴ�.
 */
class CWin32Thread : public CObject, private NonCopyable
{
	friend class CApplication;
	friend class CWin32AttachThread;
private:
	HANDLE m_hThread;
	DWORD m_id;
	bool m_bCollectOnIdle;

	HACCEL m_hAccel = nullptr;

	explicit CWin32Thread(bool bCollectOnIdle = false) NOEXCEPT;
public:
	explicit CWin32Thread(unsigned(__stdcall *fn)(void *param), void *lpParam = nullptr,
		bool bCollectOnIdle = false);

	template <typename F, typename ...Args>
	static CWin32Thread FromFunction(F f, Args&&... args)
	{
		return FromFunctionImpl(std::bind(f, std::forward<Args>(args)...));
	}

	virtual ~CWin32Thread() NOEXCEPT;

	void Attach(HANDLE hThread) NOEXCEPT;
	void Deattach() NOEXCEPT;

	HANDLE GetHandle() const NOEXCEPT;
	DWORD GetId() const NOEXCEPT;

	/**
	 * @brief �޽��� ť�� �˻��ϰ� �޽����� ������ ó���մϴ�.
	 * @param[out] msg �޽����� �ִٸ� ó���� �޽����� ���ϵ˴ϴ�.
	 * @param[in] bCallOnIdle �� ���� true�̸� �޽��� ť�� ����� ��� @ref OnIdle ���� �Լ��� ȣ���մϴ�.
	 * @return � �޽����� ó���ߴ��� �˷��ִ� @ref PollReturn �������Դϴ�.
	 * @see PollReturn
	 */
	PollReturn Poll(MSG &msg, bool bCallOnIdle = true) NOEXCEPT;

	/**
	* @brief �޽��� ������ �����մϴ�.
	* @param[in] hAccel �׼����������� �ڵ��Դϴ�. ���� ��� nullptr�Դϴ�.
	*/
	WPARAM Run(HACCEL hAccel = nullptr) NOEXCEPT;
	WPARAM Run(LPCTSTR lpAccel) NOEXCEPT;

	/**
	* @brief ��޸��� ��ȭ���ڸ� ���/�����մϴ�.
	* @param[in] hModeless nullptr�� �ƴ� ��� ��޸��� ��ȭ���ڸ� ����մϴ�. nullptr�� ��� ��� �����մϴ�.
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
	* @brief ���̵� Ÿ�ӿ� ȣ��Ǵ� �̺�Ʈ�Դϴ�.
	* @see OnIdle
	*/
	FnEvent<CWin32Thread, void()> evtIdle;
	/**
	* @brief <c>PostMessage(NULL, </c>�̳� <c>PostThreadMessage</c>�� �����忡 ���޵� �޽����� ������ ȣ��Ǵ� �̺�Ʈ�Դϴ�.
	* @param[in] msg ���� �޽����Դϴ�.
	* @see OnGlobalMsg
	*/
	FnEvent<CWin32Thread, void(const MSG &msg)> evtGlobalMsg;

protected:
	/**
	* @brief ���̵� Ÿ�ӿ� ȣ��Ǵ� ���� �Լ��Դϴ�.
	* @remarks ����Ʈ ������ @ref Idle �̺�Ʈ�� �ڵ鷯�� ���� ��� �����ڿ� ���޵� <c>bNoCollectOnIdle</c>�� <c>false</c>��� ������ ������ ���� �����մϴ�. ���� ��� Idle �̺�Ʈ�� ȣ���մϴ�. �� �Ŀ� <c>WaitMessage</c> �Լ��� ȣ���� ����մϴ�.
	* @see evtIdle
	*/
	virtual void OnIdle();
	/**
	* @brief <c>PostMessage(NULL, </c>�̳� <c>PostThreadMessage</c>�� �����忡 ���޵� �޽����� ������ ȣ��Ǵ� ���� �Լ��Դϴ�.
	* @param[in] msg ���� �޽����Դϴ�.
	* @remarks ����Ʈ ������ @ref GlobalMsg �̺�Ʈ�� ȣ���մϴ�.
	* @see evtGlobalMsg
	*/
	virtual void OnGlobalMsg(const MSG &msg);
};

class CWin32AttachThread : public CWin32Thread
{
public:
	explicit CWin32AttachThread(HANDLE hThread = nullptr, bool bCollectOnIdle = false) NOEXCEPT;
	virtual ~CWin32AttachThread() NOEXCEPT;
};
inline CWin32AttachThread::CWin32AttachThread(HANDLE hThread /* = nullptr*/,
	bool bCollectOnIdle /* = false*/) NOEXCEPT
	: CWin32Thread(bCollectOnIdle)
{
	Attach(hThread);
}

inline CWin32Thread::CWin32Thread(bool bCollectOnIdle /* = false*/) NOEXCEPT
	: m_bCollectOnIdle(bCollectOnIdle)
	, m_hThread(GetCurrentThread()), m_id(GetCurrentThreadId())
{

}
inline CWin32Thread::CWin32Thread(unsigned (__stdcall *fn)(void *param),
	void *lpParam /* = nullptr*/, bool bCollectOnIdle /* = false*/)
	: m_bCollectOnIdle(bCollectOnIdle)
{
	unsigned id;
	m_hThread = (HANDLE)GC_beginthreadex(nullptr, 0, fn, lpParam, 0, &id);
	if (m_hThread == nullptr)
		throw CThreadCreationError();

	m_id = (DWORD)id;
}

inline void CWin32Thread::Attach(HANDLE hThread) NOEXCEPT
{
	m_hThread = hThread;
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
