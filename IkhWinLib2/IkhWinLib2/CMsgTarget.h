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
 * @file CMsgTarget.h
 * @brief CMsgTarget 클래스 & 메시지 맵
 */

#pragma once

#include "CObject.h"

BEGIN_IKHWINLIB2()

/**
 * @brief 메시지 맵을 가지는 클래스의 부모 추상 클래스입니다.
 */
class CMsgTarget : public CObject, private boost::noncopyable
{
	friend class CApplication;
private:
	DWORD m_MyThreadId;
	HANDLE m_MyThread;

public:
	/**
	 * @brief 생성자입니다.
	 */
	CMsgTarget();

	/**
	 * @brief 이 객체에 메시지를 보내고 대기합니다. 세부 구현은 자식 클래스마다 달라질 수 있습니다.
	 * @remark 이 함수의 동작이나 던지는 예외에 대한 세부 구현은 자식 클래스마다 달라질 수 있습니다.<br/>
	 * 정확한 동작을 원한다면 @ref PostMsg 함수를 사용하십시오.
	 * @return 메시지에 대한 리턴값입니다.
	 * @exception CPostMsgError 메시지를 붙히지 못했습니다.
	 * @exception CSendMsgTargetThreadDied 이 객체를 가지는 스레드가 죽었습니다.
	 * @exception CSendMsgProcException 메시지 처리 함수에서 예외가 발생했습니다.
	 * @see PostMsg
	 */
	virtual LRESULT SendMsg(UINT iMessage, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 이 객체에 메시지를 붙힙니다.
	 * @exception CPostMsgError 메시지를 붙히지 못했습니다.
	 * @see SendMsg
	 */
	virtual boost::future<LRESULT> PostMsgEx(UINT iMessage, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief 메시지 처리 함수입니다. 이 순수 추상 함수를 직접 재정의하는 것보다는 메시지 맵 매크로를 사용하는 것을 권장합니다.
	 * @remarks 이 순수 추상 함수의 CMsgTarget의 구현에서는 아무 일도 하지 않고 0을 리턴합니다.
	 * @param[in] iMessage 받은 메시지입니다.
	 * @param[in] wParam 메시지의 <c>WPARAM</c>입니다.
	 * @param[in] lParam 메시지의 <c>LPARAM</c>입니다.
	 * @return 메시지에 대한 리턴값입니다.
	 */
	virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) = 0;
};

/**
 * @brief 이 클래스가 메시지 맵을 가진다고 선언합니다.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define DECLARE_MSGMAP() \
	public: \
		virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) override

/**
 * @brief 메시지 맵 구현의 시작입니다.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define BEGIN_MSGMAP(cls, base) \
	LRESULT cls::MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc) \
	{ \
		bool _proc; if (pbProc == nullptr) pbProc = &_proc; \
		*pbProc = true; \
		switch (iMessage) \
		{

#include "MsgCracker.h"

#define BEGIN_CMDMAP() \
		case WM_COMMAND:

/* void OnCommand(int id, HWND hCtl, UINT codeNotify) */
#define CMDMAP_ID(id, fn) \
			if ((int)LOWORD(wParam) == (id)) \
				return ((fn)(LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam)), 0);
#define CMDMAP_CTRL(code, id, fn) \
			if ((int)LOWORD(wParam) == (id) && HIWORD(wParam) == (code)) \
				return ((fn)(LOWORD(wParam), (HWND)lParam, (UINT)HIWORD(wParam)), 0);

#define END_CMDMAP() \
			break;

#define BEGIN_NOTIFYMAP() \
		case WM_NOTIFY: { \
			NMHDR *phdr = (NMHDR *)lParam; \

/* LRESULT OnNotify(NMHDR *phdr); */
#define NOTIFYMAP_ID(code, id, fn) \
			if ((int)wParam == (id) && phdr->code == (code)) { \
				return (fn)(phdr); }
#define NOTIFYMAP_WND(code, wnd, fn) \
			if (phdr->hwndFrom == static_cast<HWND>(wnd) && phdr->code == (code)) { \
				return (fn)(phdr); }

#define END_NOTIFYMAP() \
			break; }

/**
 * @brief @ref MsgCracker.h에서 지원하지 않는 메시지 핸들러를 선언합니다.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define MSGMAP_CUSTOM(msg, fn) \
		case (msg): return (fn)(wParam, lParam);

/**
 * @brief @ref MsgCracker.h에서 지원하지 않는 메시지에 대해 부모의 메시지 함수를 호출합니다.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define MSG_FORWARD_CUSTOM(base, msg, wParam, lParam) \
	(base::MessageProc)(msg, wParam, lParam)

#define BEGIN_CHAIN() \
		default:

#define MSG_CHAIN(target) \
			if (target) { \
				LRESULT r = static_cast<CMsgTarget *>(target)->MessageProc( \
					iMessage, wParam, lParam, pbProc); \
				if (*pbProc) return r; \
			}

#define END_CHAIN() \
			break;

/**
 * @brief 메시지 맵 구현의 끝입니다.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define END_MSGMAP(cls, base) \
		} \
		*pbProc = false; \
		return base::MessageProc(iMessage, wParam, lParam, pbProc); \
	}

END_IKHWINLIB2()
