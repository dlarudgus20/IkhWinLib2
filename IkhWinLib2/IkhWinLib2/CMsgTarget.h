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

/**
 * @file CMsgTarget.h
 * @brief CMsgTarget Ŭ���� & �޽��� ��
 */

#pragma once

#include "CObject.h"

BEGIN_IKHWINLIB2()

/**
 * @brief �޽��� ���� ������ Ŭ������ �θ� �߻� Ŭ�����Դϴ�.
 */
class CMsgTarget : public CObject, private boost::noncopyable
{
	friend class CApplication;
private:
	DWORD m_MyThreadId;
	HANDLE m_MyThread;

public:
	/**
	 * @brief �������Դϴ�.
	 */
	CMsgTarget();

	/**
	 * @brief �� ��ü�� �޽����� ������ ����մϴ�. ���� ������ �ڽ� Ŭ�������� �޶��� �� �ֽ��ϴ�.
	 * @remark �� �Լ��� �����̳� ������ ���ܿ� ���� ���� ������ �ڽ� Ŭ�������� �޶��� �� �ֽ��ϴ�.<br/>
	 * ��Ȯ�� ������ ���Ѵٸ� @ref PostMsg �Լ��� ����Ͻʽÿ�.
	 * @return �޽����� ���� ���ϰ��Դϴ�.
	 * @exception CPostMsgError �޽����� ������ ���߽��ϴ�.
	 * @exception CSendMsgTargetThreadDied �� ��ü�� ������ �����尡 �׾����ϴ�.
	 * @exception CSendMsgProcException �޽��� ó�� �Լ����� ���ܰ� �߻��߽��ϴ�.
	 * @see PostMsg
	 */
	virtual LRESULT SendMsg(UINT iMessage, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief �� ��ü�� �޽����� �����ϴ�.
	 * @exception CPostMsgError �޽����� ������ ���߽��ϴ�.
	 * @see SendMsg
	 */
	virtual boost::future<LRESULT> PostMsgEx(UINT iMessage, WPARAM wParam, LPARAM lParam);

	/**
	 * @brief �޽��� ó�� �Լ��Դϴ�. �� ���� �߻� �Լ��� ���� �������ϴ� �ͺ��ٴ� �޽��� �� ��ũ�θ� ����ϴ� ���� �����մϴ�.
	 * @remarks �� ���� �߻� �Լ��� CMsgTarget�� ���������� �ƹ� �ϵ� ���� �ʰ� 0�� �����մϴ�.
	 * @param[in] iMessage ���� �޽����Դϴ�.
	 * @param[in] wParam �޽����� <c>WPARAM</c>�Դϴ�.
	 * @param[in] lParam �޽����� <c>LPARAM</c>�Դϴ�.
	 * @return �޽����� ���� ���ϰ��Դϴ�.
	 */
	virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) = 0;
};

/**
 * @brief �� Ŭ������ �޽��� ���� �����ٰ� �����մϴ�.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define DECLARE_MSGMAP() \
	public: \
		virtual LRESULT MessageProc(UINT iMessage, WPARAM wParam, LPARAM lParam, bool *pbProc = nullptr) override

/**
 * @brief �޽��� �� ������ �����Դϴ�.
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
 * @brief @ref MsgCracker.h���� �������� �ʴ� �޽��� �ڵ鷯�� �����մϴ�.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define MSGMAP_CUSTOM(msg, fn) \
		case (msg): return (fn)(wParam, lParam);

/**
 * @brief @ref MsgCracker.h���� �������� �ʴ� �޽����� ���� �θ��� �޽��� �Լ��� ȣ���մϴ�.
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
 * @brief �޽��� �� ������ ���Դϴ�.
 * @example CMsgTarget_MSGMAP.cpp
 */
#define END_MSGMAP(cls, base) \
		} \
		*pbProc = false; \
		return base::MessageProc(iMessage, wParam, lParam, pbProc); \
	}

END_IKHWINLIB2()
