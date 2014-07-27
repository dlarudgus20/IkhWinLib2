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
 * @file IkhWinLib2.h
 * @brief IkhWinLib2의 모든 헤더를 include하고 네임스페이스를 using 해 줍니다.
 */

#pragma once

#include "Defines.h"

#include "CApplication.h"
#include "CObject.h"

#include "CException.h"
#include "CSystemError.h"
#include "CLogicError.h"

#include "CMsgTarget.h"

#include "CWindow.h"
#include "COpenGLWnd.h"
#include "CIdleOpenGLWnd.h"
#include "CThreadOpenGLWnd.h"

#include "CControlWnd.h"
#include "CButtonCtrl.h"
#include "CComboBoxCtrl.h"
#include "CEditCtrl.h"
#include "CListBoxCtrl.h"
#include "CScrollbarCtrl.h"
#include "CStaticCtrl.h"

#include "CDateTimePickCtrl.h"
#include "CHotkeyCtrl.h"
#include "CProgressCtrl.h"
#include "CSpinbtnCtrl.h"
#include "CTrackbarCtrl.h"

#include "CForm.h"

#include "CWndClass.h"

#include "FnEvent.h"
#include "NonCopyable.h"

#include "EnableVisualStyle.h"

/**
* @macro IKHWINLIB2_NO_USING
* @brief 네임스페이스를 using하지 않습니다.
*/
#ifndef IKHWINLIB2_NO_USING
using namespace IkhProgram::IkhWinLib2;
#endif

#ifndef IKHWINLIB2_NO_AUTOMATICALLY_LINK
# pragma comment(lib, "IkhWinLib2.lib")
# pragma comment(lib, "gcmt-lib.lib")
# pragma comment(lib, "comctl32.lib")
#endif
