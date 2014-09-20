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

#include "CException.h"

BEGIN_IKHWINLIB2()

IKHWINLIB2_MAKE_EXCEPTION(CSystemError, CException, L"IkhProgram::IkhWinLib2::CSystemError")

IKHWINLIB2_MAKE_EXCEPTION(CCreationError, CSystemError, L"IkhProgram::IkhWinLib2::CCreationError")

IKHWINLIB2_MAKE_EXCEPTION(CRegisterClassError, CCreationError, L"윈도우 클래스를 등록하지 못했습니다.")
IKHWINLIB2_MAKE_EXCEPTION(CWindowCreationError, CCreationError, L"창을 생성하지 못했습니다.")
IKHWINLIB2_MAKE_EXCEPTION(CEventCreationError, CCreationError, L"이벤트 동기화 객체를 생성하지 못했습니다.")
IKHWINLIB2_MAKE_EXCEPTION(CThreadCreationError, CCreationError, L"스레드를 생성하지 못했습니다.")
IKHWINLIB2_MAKE_EXCEPTION(CImageListCreationError, CCreationError, L"이미지 리스트를 생성하지 못했습니다.")

IKHWINLIB2_MAKE_EXCEPTION(CValidWin32ThreadDestroyError, CSystemError, L"스레드에 연결된 CWin32Thread 객체가 파괴되고 있습니다.")

IKHWINLIB2_MAKE_EXCEPTION(CPostMsgError, CSystemError, L"메시지를 붙힐 수 없습니다.")
IKHWINLIB2_MAKE_EXCEPTION(CMsgTargetThreadDied, CSystemError, L"메시지를 보낼 대상 스레드가 죽었습니다.")

class CChangeDisplaySettingsError : public CSystemError
{
private:
	LONG m_code;
public:
	explicit CChangeDisplaySettingsError(LONG c)
		: CSystemError(L"ChangeDisplaySettings()가 실패하였습니다.")
		, m_code(c)
	{

	}
	LONG GetErrCode() const NOEXCEPT { return m_code; }
};

END_IKHWINLIB2()
