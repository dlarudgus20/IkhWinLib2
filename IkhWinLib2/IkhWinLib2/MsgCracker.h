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
 * @file MsgCracker.h
 * @brief 메시지 핸들러 매크로들이 정의된 헤더입니다.
 * @remarks
 * MSGMAP_WM_XXX(fn) 매크로들은 메시지 맵 안에서 메시지 핸들러를 정의합니다.<br/>
 * MSG_FORWARD_WM_XXX(base, ...) 매크로들은 메시지 핸들러에서 부모의 메시지 핸들러를 호출합니다.<br/>
 * 더 자세한 건 다음 예제를 참고하십시오.
 * @example CMsgTarget_MSGMAP.cpp
 */

#pragma once

/* void OnCompacting(UINT compactRatio) */
#define MSGMAP_WM_COMPACTING(fn) \
	case WM_COMPACTING: return ((fn)((UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_COMPACTING(base, compactRatio) \
	(void)(base::MessageProc)(WM_COMPACTING, (WPARAM)(UINT)(compactRatio), 0L)

/* void OnWinIniChange(LPCTSTR lpszSectionName) */
#define MSGMAP_WM_WININICHANGE(fn) \
	case WM_WININICHANGE: return ((fn)((LPCTSTR)(lParam)), 0L);
#define MSG_FORWARD_WM_WININICHANGE(base, lpszSectionName) \
	(void)(base::MessageProc)(WM_WININICHANGE, 0L, (LPARAM)(LPCTSTR)(lpszSectionName))

/* void OnSysColorChange() */
#define MSGMAP_WM_SYSCOLORCHANGE(fn) \
	case WM_SYSCOLORCHANGE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_SYSCOLORCHANGE(base) \
	(void)(base::MessageProc)(WM_SYSCOLORCHANGE, 0L, 0L)

/* BOOL OnQueryNewPalette() */
#define MSGMAP_WM_QUERYNEWPALETTE(fn) \
	case WM_QUERYNEWPALETTE: return MAKELRESULT((BOOL)(fn)(), 0L);
#define MSG_FORWARD_WM_QUERYNEWPALETTE(base) \
	(BOOL)(DWORD)(base::MessageProc)(WM_QUERYNEWPALETTE, 0L, 0L)

/* void OnPaletteIsChanging(PaletteChange) */
#define MSGMAP_WM_PALETTEISCHANGING(fn) \
	case WM_PALETTEISCHANGING: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_PALETTEISCHANGING(base, basePaletteChange) \
	(void)(base::MessageProc)(WM_PALETTEISCHANGING, (WPARAM)(HWND)(hwndPaletteChange), 0L)

/* void OnPaletteChanged(PaletteChange) */
#define MSGMAP_WM_PALETTECHANGED(fn) \
	case WM_PALETTECHANGED: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_PALETTECHANGED(base, basePaletteChange) \
	(void)(base::MessageProc)(WM_PALETTECHANGED, (WPARAM)(HWND)(hwndPaletteChange), 0L)

/* void OnFontChange() */
#define MSGMAP_WM_FONTCHANGE(fn) \
	case WM_FONTCHANGE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_FONTCHANGE(base) \
	(void)(base::MessageProc)(WM_FONTCHANGE, 0L, 0L)

/* void OnSpoolerStatus(UINT status, int cJobInQueue) */
#define MSGMAP_WM_SPOOLERSTATUS(fn) \
	case WM_SPOOLERSTATUS: return ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SPOOLERSTATUS(base, status, cJobInQueue) \
	(void)(base::MessageProc)(WM_SPOOLERSTATUS, (WPARAM)(status), MAKELPARAM((cJobInQueue), 0))

/* void OnDevModeChange(LPCTSTR lpszDeviceName) */
#define MSGMAP_WM_DEVMODECHANGE(fn) \
	case WM_DEVMODECHANGE: return ((fn)((LPCTSTR)(lParam)), 0L);
#define MSG_FORWARD_WM_DEVMODECHANGE(base, lpszDeviceName) \
	(void)(base::MessageProc)(WM_DEVMODECHANGE, 0L, (LPARAM)(LPCTSTR)(lpszDeviceName))

/* void OnTimeChange() */
#define MSGMAP_WM_TIMECHANGE(fn) \
	case WM_TIMECHANGE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_TIMECHANGE(base) \
	(void)(base::MessageProc)(WM_TIMECHANGE, 0L, 0L)

/* void OnPower(int code) */
#define MSGMAP_WM_POWER(fn) \
	case WM_POWER: return ((fn)((int)(wParam)), 0L);
#define MSG_FORWARD_WM_POWER(base, code) \
	(void)(base::MessageProc)(WM_POWER, (WPARAM)(int)(code), 0L)

/* BOOL OnQueryEndSession() */
#define MSGMAP_WM_QUERYENDSESSION(fn) \
	case WM_QUERYENDSESSION: return MAKELRESULT((BOOL)(fn)(), 0L);
#define MSG_FORWARD_WM_QUERYENDSESSION(base) \
	(BOOL)(DWORD)(base::MessageProc)(WM_QUERYENDSESSION, 0L, 0L)

/* void OnEndSession(BOOL fEnding) */
#define MSGMAP_WM_ENDSESSION(fn) \
	case WM_ENDSESSION: return ((fn)((BOOL)(wParam)), 0L);
#define MSG_FORWARD_WM_ENDSESSION(base, fEnding) \
	(void)(base::MessageProc)(WM_ENDSESSION, (WPARAM)(BOOL)(fEnding), 0L)

/* void OnQuit(int exitCode) */
#define MSGMAP_WM_QUIT(fn) \
	case WM_QUIT: return ((fn)((int)(wParam)), 0L);
#define MSG_FORWARD_WM_QUIT(base, exitCode) \
	(void)(base::MessageProc)(WM_QUIT, (WPARAM)(exitCode), 0L)

/* BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) */
#define MSGMAP_WM_CREATE(fn) \
	case WM_CREATE: return ((fn)((LPCREATESTRUCT)(lParam)) ? 0L : (LRESULT)-1L);
#define MSG_FORWARD_WM_CREATE(base, lpCreateStruct) \
	(!(BOOL)(DWORD)(base::MessageProc)(WM_CREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct)))

/* BOOL OnNCCreate(LPCREATESTRUCT lpCreateStruct) */
#define MSGMAP_WM_NCCREATE(fn) \
	case WM_NCCREATE: return (LRESULT)(DWORD)(BOOL)(fn)((LPCREATESTRUCT)(lParam));
#define MSG_FORWARD_WM_NCCREATE(base, lpCreateStruct) \
	(BOOL)(DWORD)(base::MessageProc)(WM_NCCREATE, 0L, (LPARAM)(LPCREATESTRUCT)(lpCreateStruct))

/* void OnDestroy() */
#define MSGMAP_WM_DESTROY(fn) \
	case WM_DESTROY: return ((fn)(), 0L);
#define MSG_FORWARD_WM_DESTROY(base) \
	(void)(base::MessageProc)(WM_DESTROY, 0L, 0L)

/* void OnNCDestroy() */
#define MSGMAP_WM_NCDESTROY(fn) \
	case WM_NCDESTROY: return ((fn)(), 0L);
#define MSG_FORWARD_WM_NCDESTROY(base) \
	(void)(base::MessageProc)(WM_NCDESTROY, 0L, 0L)

/* void OnShowWindow(BOOL fShow, UINT status) */
#define MSGMAP_WM_SHOWWINDOW(fn) \
	case WM_SHOWWINDOW: return ((fn)((BOOL)(wParam), (UINT)(lParam)), 0L);
#define MSG_FORWARD_WM_SHOWWINDOW(base, fShow, status) \
	(void)(base::MessageProc)(WM_SHOWWINDOW, (WPARAM)(BOOL)(fShow), (LPARAM)(UINT)(status))

/* void OnSetRedraw(BOOL fRedraw) */
#define MSGMAP_WM_SETREDRAW(fn) \
	case WM_SETREDRAW: return ((fn)((BOOL)(wParam)), 0L);
#define MSG_FORWARD_WM_SETREDRAW(base, fRedraw) \
	(void)(base::MessageProc)(WM_SETREDRAW, (WPARAM)(BOOL)(fRedraw), 0L)

/* void OnEnable(BOOL fEnable) */
#define MSGMAP_WM_ENABLE(fn) \
	case WM_ENABLE: return ((fn)((BOOL)(wParam)), 0L);
#define MSG_FORWARD_WM_ENABLE(base, fEnable) \
	(void)(base::MessageProc)(WM_ENABLE, (WPARAM)(BOOL)(fEnable), 0L)

/* void OnSetText(LPCTSTR lpszText) */
#define MSGMAP_WM_SETTEXT(fn) \
	case WM_SETTEXT: return ((fn)((LPCTSTR)(lParam)), 0L);
#define MSG_FORWARD_WM_SETTEXT(base, lpszText) \
	(void)(base::MessageProc)(WM_SETTEXT, 0L, (LPARAM)(LPCTSTR)(lpszText))

/* INT OnGetText(int cchTextMax, LPTSTR lpszText) */
#define MSGMAP_WM_GETTEXT(fn) \
	case WM_GETTEXT: return (LRESULT)(DWORD)(int)(fn)((int)(wParam), (LPTSTR)(lParam));
#define MSG_FORWARD_WM_GETTEXT(base, cchTextMax, lpszText) \
	(int)(DWORD)(base::MessageProc)(WM_GETTEXT, (WPARAM)(int)(cchTextMax), (LPARAM)(LPTSTR)(lpszText))

/* INT OnGetTextLength() */
#define MSGMAP_WM_GETTEXTLENGTH(fn) \
	case WM_GETTEXTLENGTH: return (LRESULT)(DWORD)(int)(fn)();
#define MSG_FORWARD_WM_GETTEXTLENGTH(base) \
	(int)(DWORD)(base::MessageProc)(WM_GETTEXTLENGTH, 0L, 0L)

/* BOOL OnWindowPosChanging(LPWINDOWPOS lpwpos) */
#define MSGMAP_WM_WINDOWPOSCHANGING(fn) \
	case WM_WINDOWPOSCHANGING: return (LRESULT)(DWORD)(BOOL)(fn)((LPWINDOWPOS)(lParam));
#define MSG_FORWARD_WM_WINDOWPOSCHANGING(base, lpwpos) \
	(BOOL)(DWORD)(base::MessageProc)(WM_WINDOWPOSCHANGING, 0L, (LPARAM)(LPWINDOWPOS)(lpwpos))

/* void OnWindowPosChanged(const LPWINDOWPOS lpwpos) */
#define MSGMAP_WM_WINDOWPOSCHANGED(fn) \
	case WM_WINDOWPOSCHANGED: return ((fn)((const LPWINDOWPOS)(lParam)), 0L);
#define MSG_FORWARD_WM_WINDOWPOSCHANGED(base, lpwpos) \
	(void)(base::MessageProc)(WM_WINDOWPOSCHANGED, 0L, (LPARAM)(const LPWINDOWPOS)(lpwpos))

/* void OnMove(int x, int y) */
#define MSGMAP_WM_MOVE(fn) \
	case WM_MOVE: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_MOVE(base, x, y) \
	(void)(base::MessageProc)(WM_MOVE, 0L, MAKELPARAM((x), (y)))

/* void OnSize(UINT state, int cx, int cy) */
#define MSGMAP_WM_SIZE(fn) \
	case WM_SIZE: return ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SIZE(base, state, cx, cy) \
	(void)(base::MessageProc)(WM_SIZE, (WPARAM)(UINT)(state), MAKELPARAM((cx), (cy)))

/* void OnSizing(UINT flags, LPRECT lpRect) */
#define MSGMAP_WM_SIZING(fn) \
	case WM_SIZING: return ((fn)((UINT)(wParam), (LPRECT)(lParam)), 0);
#define MSG_FORWARD_WM_SIZING(base, flags, lpRect) \
	(void)(base::MessageProc)(WM_SIZING, (WPARAM)(UINT)(flags), (LPARAM)(LPRECT)(lParam))

/* void OnClose() */
#define MSGMAP_WM_CLOSE(fn) \
	case WM_CLOSE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_CLOSE(base) \
	(void)(base::MessageProc)(WM_CLOSE, 0L, 0L)

/* BOOL OnQueryOpen() */
#define MSGMAP_WM_QUERYOPEN(fn) \
	case WM_QUERYOPEN: return MAKELRESULT((BOOL)(fn)(), 0L);
#define MSG_FORWARD_WM_QUERYOPEN(base) \
	(BOOL)(DWORD)(base::MessageProc)(WM_QUERYOPEN, 0L, 0L)

/* void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo) */
#define MSGMAP_WM_GETMINMAXINFO(fn) \
	case WM_GETMINMAXINFO: return ((fn)((LPMINMAXINFO)(lParam)), 0L);
#define MSG_FORWARD_WM_GETMINMAXINFO(base, lpMinMaxInfo) \
	(void)(base::MessageProc)(WM_GETMINMAXINFO, 0L, (LPARAM)(LPMINMAXINFO)(lpMinMaxInfo))

/* void OnPaint() */
#define MSGMAP_WM_PAINT(fn) \
	case WM_PAINT: return ((fn)(), 0L);
#define MSG_FORWARD_WM_PAINT(base) \
	(void)(base::MessageProc)(WM_PAINT, 0L, 0L)

/* BOOL OnEraseBkgnd(HDC hdc) */
#define MSGMAP_WM_ERASEBKGND(fn) \
	case WM_ERASEBKGND: return (LRESULT)(DWORD)(BOOL)(fn)((HDC)(wParam));
#define MSG_FORWARD_WM_ERASEBKGND(base, hdc) \
	(BOOL)(DWORD)(base::MessageProc)(WM_ERASEBKGND, (WPARAM)(HDC)(hdc), 0L)

/* BOOL OnIconEraseBkgnd(HDC hdc) */
#define MSGMAP_WM_ICONERASEBKGND(fn) \
	case WM_ICONERASEBKGND: return (LRESULT)(DWORD)(BOOL)(fn)((HDC)(wParam));
#define MSG_FORWARD_WM_ICONERASEBKGND(base, hdc) \
	(BOOL)(DWORD)(base::MessageProc)(WM_ICONERASEBKGND, (WPARAM)(HDC)(hdc), 0L)

/* void OnNCPaint(HRGN hrgn) */
#define MSGMAP_WM_NCPAINT(fn) \
	case WM_NCPAINT: return ((fn)((HRGN)(wParam)), 0L);
#define MSG_FORWARD_WM_NCPAINT(base, hrgn) \
	(void)(base::MessageProc)(WM_NCPAINT, (WPARAM)(HRGN)(hrgn), 0L)

/* UINT OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp) */
#define MSGMAP_WM_NCCALCSIZE(fn) \
	case WM_NCCALCSIZE: return (LRESULT)(DWORD)(UINT)(fn)((BOOL)(wParam), (NCCALCSIZE_PARAMS *)(lParam));
#define MSG_FORWARD_WM_NCCALCSIZE(base, fCalcValidRects, lpcsp) \
	(UINT)(DWORD)(base::MessageProc)(WM_NCCALCSIZE, (WPARAM)(fCalcValidRects), (LPARAM)(NCCALCSIZE_PARAMS *)(lpcsp))

/* UINT OnNCHitTest(int x, int y) */
#define MSGMAP_WM_NCHITTEST(fn) \
	case WM_NCHITTEST: return (LRESULT)(DWORD)(UINT)(fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
#define MSG_FORWARD_WM_NCHITTEST(base, x, y) \
	(UINT)(DWORD)(base::MessageProc)(WM_NCHITTEST, 0L, MAKELPARAM((x), (y)))

/* HICON OnQueryDragIcon() */
#define MSGMAP_WM_QUERYDRAGICON(fn) \
	case WM_QUERYDRAGICON: return (LRESULT)(DWORD)(UINT)(fn)();
#define MSG_FORWARD_WM_QUERYDRAGICON(base) \
	(HICON)(UINT)(DWORD)(base::MessageProc)(WM_QUERYDRAGICON, 0L, 0L)

#ifdef _INC_SHELLAPI
/* void OnDropFiles(HDROP hdrop) */
#define MSGMAP_WM_DROPFILES(fn) \
	case WM_DROPFILES: return ((fn)((HDROP)(wParam)), 0L);
#define MSG_FORWARD_WM_DROPFILES(base, hdrop) \
	(void)(base::MessageProc)(WM_DROPFILES, (WPARAM)(HDROP)(hdrop), 0L)
#endif  /* _INC_SHELLAPI */

/* void OnActivate(UINT state, HWND hActDeact, BOOL fMinimized) */
#define MSGMAP_WM_ACTIVATE(fn) \
	case WM_ACTIVATE: return ((fn)((UINT)LOWORD(wParam), (HWND)(lParam), (BOOL)HIWORD(wParam)), 0L);
#define MSG_FORWARD_WM_ACTIVATE(base, state, baseActDeact, fMinimized) \
	(void)(base::MessageProc)(WM_ACTIVATE, MAKEWPARAM((state), (fMinimized)), (LPARAM)(HWND)(hwndActDeact))

/* void OnActivateApp(BOOL fActivate, DWORD dwThreadId) */
#define MSGMAP_WM_ACTIVATEAPP(fn) \
	case WM_ACTIVATEAPP: return ((fn)((BOOL)(wParam), (DWORD)(lParam)), 0L);
#define MSG_FORWARD_WM_ACTIVATEAPP(base, fActivate, dwThreadId) \
	(void)(base::MessageProc)(WM_ACTIVATEAPP, (WPARAM)(BOOL)(fActivate), (LPARAM)(dwThreadId))

/* BOOL OnNCActivate(BOOL fActive, HWND hActDeact, BOOL fMinimized) */
#define MSGMAP_WM_NCACTIVATE(fn) \
	case WM_NCACTIVATE: return (LRESULT)(DWORD)(BOOL)(fn)((BOOL)(wParam), 0L, 0L);
#define MSG_FORWARD_WM_NCACTIVATE(base, fActive, baseActDeact, fMinimized) \
	(BOOL)(DWORD)(base::MessageProc)(WM_NCACTIVATE, (WPARAM)(BOOL)(fActive), 0L)

/* void OnSetFocus(HWND hOldFocus) */
#define MSGMAP_WM_SETFOCUS(fn) \
	case WM_SETFOCUS: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_SETFOCUS(base, baseOldFocus) \
	(void)(base::MessageProc)(WM_SETFOCUS, (WPARAM)(HWND)(hwndOldFocus), 0L)

/* void OnKillFocus(HWND hNewFocus) */
#define MSGMAP_WM_KILLFOCUS(fn) \
	case WM_KILLFOCUS: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_KILLFOCUS(base, baseNewFocus) \
	(void)(base::MessageProc)(WM_KILLFOCUS, (WPARAM)(HWND)(hwndNewFocus), 0L)

/* void OnKey(UINT vk, BOOL fDown, int cRepeat, UINT flags) */
#define MSGMAP_WM_KEYDOWN(fn) \
	case WM_KEYDOWN: return ((fn)((UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_KEYDOWN(base, vk, cRepeat, flags) \
	(void)(base::MessageProc)(WM_KEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)))

/* void OnKey(UINT vk, BOOL fDown, int cRepeat, UINT flags) */
#define MSGMAP_WM_KEYUP(fn) \
	case WM_KEYUP: return ((fn)((UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_KEYUP(base, vk, cRepeat, flags) \
	(void)(base::MessageProc)(WM_KEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)))

/* void OnChar(TCHAR ch, int cRepeat) */
#define MSGMAP_WM_CHAR(fn) \
	case WM_CHAR: return ((fn)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_CHAR(base, ch, cRepeat) \
	(void)(base::MessageProc)(WM_CHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat), 0))

/* void OnDeadChar(TCHAR ch, int cRepeat) */
#define MSGMAP_WM_DEADCHAR(fn) \
	case WM_DEADCHAR: return ((fn)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_DEADCHAR(base, ch, cRepeat) \
	(void)(base::MessageProc)(WM_DEADCHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat), 0))

/* void OnSysKey(UINT vk, BOOL fDown, int cRepeat, UINT flags) */
#define MSGMAP_WM_SYSKEYDOWN(fn) \
	case WM_SYSKEYDOWN: return ((fn)((UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SYSKEYDOWN(base, vk, cRepeat, flags) \
	(void)(base::MessageProc)(WM_SYSKEYDOWN, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)))

/* void OnSysKey(UINT vk, BOOL fDown, int cRepeat, UINT flags) */
#define MSGMAP_WM_SYSKEYUP(fn) \
	case WM_SYSKEYUP: return ((fn)((UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SYSKEYUP(base, vk, cRepeat, flags) \
	(void)(base::MessageProc)(WM_SYSKEYUP, (WPARAM)(UINT)(vk), MAKELPARAM((cRepeat), (flags)))

/* void OnSysChar(TCHAR ch, int cRepeat) */
#define MSGMAP_WM_SYSCHAR(fn) \
	case WM_SYSCHAR: return ((fn)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SYSCHAR(base, ch, cRepeat) \
	(void)(base::MessageProc)(WM_SYSCHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat), 0))

/* void OnSysDeadChar(TCHAR ch, int cRepeat) */
#define MSGMAP_WM_SYSDEADCHAR(fn) \
	case WM_SYSDEADCHAR: return ((fn)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SYSDEADCHAR(base, ch, cRepeat) \
	(void)(base::MessageProc)(WM_SYSDEADCHAR, (WPARAM)(TCHAR)(ch), MAKELPARAM((cRepeat), 0))

/* void OnMouseMove(int x, int y, UINT keyFlags) */
#define MSGMAP_WM_MOUSEMOVE(fn) \
	case WM_MOUSEMOVE: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_MOUSEMOVE(base, x, y, keyFlags) \
	(void)(base::MessageProc)(WM_MOUSEMOVE, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_LBUTTONDOWN(fn) \
	case WM_LBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_LBUTTONDOWN(base, fDoubleClick, x, y, keyFlags) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_LBUTTONDBLCLK : WM_LBUTTONDOWN, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnLButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_LBUTTONDBLCLK(fn) \
	case WM_LBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnLButtonUp(int x, int y, UINT keyFlags) */
#define MSGMAP_WM_LBUTTONUP(fn) \
	case WM_LBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_LBUTTONUP(base, x, y, keyFlags) \
	(void)(base::MessageProc)(WM_LBUTTONUP, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnRButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_RBUTTONDOWN(fn) \
	case WM_RBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_RBUTTONDOWN(base, fDoubleClick, x, y, keyFlags) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_RBUTTONDBLCLK : WM_RBUTTONDOWN, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnRButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_RBUTTONDBLCLK(fn) \
	case WM_RBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnRButtonUp(int x, int y, UINT flags) */
#define MSGMAP_WM_RBUTTONUP(fn) \
	case WM_RBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_RBUTTONUP(base, x, y, keyFlags) \
	(void)(base::MessageProc)(WM_RBUTTONUP, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnMButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_MBUTTONDOWN(fn) \
	case WM_MBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_MBUTTONDOWN(base, fDoubleClick, x, y, keyFlags) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_MBUTTONDBLCLK : WM_MBUTTONDOWN, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnMButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define MSGMAP_WM_MBUTTONDBLCLK(fn) \
	case WM_MBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnMButtonUp(int x, int y, UINT flags) */
#define MSGMAP_WM_MBUTTONUP(fn) \
	case WM_MBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_MBUTTONUP(base, x, y, keyFlags) \
	(void)(base::MessageProc)(WM_MBUTTONUP, (WPARAM)(UINT)(keyFlags), MAKELPARAM((x), (y)))

/* void OnMouseWheel(int xPos, int yPos, int zDelta, UINT fwKeys) */
#define MSGMAP_WM_MOUSEWHEEL(fn) \
	case WM_MOUSEWHEEL: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L);
#define MSG_FORWARD_WM_MOUSEWHEEL(base, xPos, yPos, zDelta, fwKeys) \
	(void)(base::MessageProc)(WM_MOUSEWHEEL, MAKEWPARAM((fwKeys), (zDelta)), MAKELPARAM((xPos), (yPos)))

/* void OnNCMouseMove(int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCMOUSEMOVE(fn) \
	case WM_NCMOUSEMOVE: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCMOUSEMOVE(base, x, y, codeHitTest) \
	(void)(base::MessageProc)(WM_NCMOUSEMOVE, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCLBUTTONDOWN(fn) \
	case WM_NCLBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCLBUTTONDOWN(base, fDoubleClick, x, y, codeHitTest) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_NCLBUTTONDBLCLK : WM_NCLBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCLButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCLBUTTONDBLCLK(fn) \
	case WM_NCLBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnNCLButtonUp(int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCLBUTTONUP(fn) \
	case WM_NCLBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCLBUTTONUP(base, x, y, codeHitTest) \
	(void)(base::MessageProc)(WM_NCLBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCRBUTTONDOWN(fn) \
	case WM_NCRBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCRBUTTONDOWN(base, fDoubleClick, x, y, codeHitTest) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_NCRBUTTONDBLCLK : WM_NCRBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCRButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCRBUTTONDBLCLK(fn) \
	case WM_NCRBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnNCRButtonUp(int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCRBUTTONUP(fn) \
	case WM_NCRBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCRBUTTONUP(base, x, y, codeHitTest) \
	(void)(base::MessageProc)(WM_NCRBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCMBUTTONDOWN(fn) \
	case WM_NCMBUTTONDOWN: return ((fn)(FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCMBUTTONDOWN(base, fDoubleClick, x, y, codeHitTest) \
	(void)(base::MessageProc)((fDoubleClick) ? WM_NCMBUTTONDBLCLK : WM_NCMBUTTONDOWN, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* void OnNCMButtonDown(BOOL fDoubleClick, int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCMBUTTONDBLCLK(fn) \
	case WM_NCMBUTTONDBLCLK: return ((fn)(TRUE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);

/* void OnNCMButtonUp(int x, int y, UINT codeHitTest) */
#define MSGMAP_WM_NCMBUTTONUP(fn) \
	case WM_NCMBUTTONUP: return ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_NCMBUTTONUP(base, x, y, codeHitTest) \
	(void)(base::MessageProc)(WM_NCMBUTTONUP, (WPARAM)(UINT)(codeHitTest), MAKELPARAM((x), (y)))

/* int OnMouseActivate(HWND hTopLevel, UINT codeHitTest, UINT msg) */
#define MSGMAP_WM_MOUSEACTIVATE(fn) \
	case WM_MOUSEACTIVATE: return (LRESULT)(DWORD)(int)(fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
#define MSG_FORWARD_WM_MOUSEACTIVATE(base, baseTopLevel, codeHitTest, msg) \
	(int)(DWORD)(base::MessageProc)(WM_MOUSEACTIVATE, (WPARAM)(HWND)(hwndTopLevel), MAKELPARAM((codeHitTest), (msg)))

/* void OnCancelMode() */
#define MSGMAP_WM_CANCELMODE(fn) \
	case WM_CANCELMODE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_CANCELMODE(base) \
	(void)(base::MessageProc)(WM_CANCELMODE, 0L, 0L)

/* void OnTimer(UINT id) */
#define MSGMAP_WM_TIMER(fn) \
	case WM_TIMER: return ((fn)((UINT)(wParam)), 0L);
#define MSG_FORWARD_WM_TIMER(base, id) \
	(void)(base::MessageProc)(WM_TIMER, (WPARAM)(UINT)(id), 0L)

/* void OnInitMenu(HMENU hMenu) */
#define MSGMAP_WM_INITMENU(fn) \
	case WM_INITMENU: return ((fn)((HMENU)(wParam)), 0L);
#define MSG_FORWARD_WM_INITMENU(base, hMenu) \
	(void)(base::MessageProc)(WM_INITMENU, (WPARAM)(HMENU)(hMenu), 0L)

/* void OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) */
#define MSGMAP_WM_INITMENUPOPUP(fn) \
	case WM_INITMENUPOPUP: return ((fn)((HMENU)(wParam), (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_INITMENUPOPUP(base, hMenu, item, fSystemMenu) \
	(void)(base::MessageProc)(WM_INITMENUPOPUP, (WPARAM)(HMENU)(hMenu), MAKELPARAM((item), (fSystemMenu)))

/* void OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags) */
#define MSGMAP_WM_MENUSELECT(fn)                  \
	case WM_MENUSELECT: return ((fn)((HMENU)(lParam), \
	(HIWORD(wParam) & MF_POPUP) ? 0L : (int)(LOWORD(wParam)), \
	(HIWORD(wParam) & MF_POPUP) ? GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L, \
	(UINT)(((short)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam))), 0L);
#define MSG_FORWARD_WM_MENUSELECT(base, hmenu, item, hmenuPopup, flags) \
	(void)(base::MessageProc)(WM_MENUSELECT, MAKEWPARAM((item), (flags)), (LPARAM)(HMENU)((hmenu) ? (hmenu) : (hmenuPopup)))

/* DWORD OnMenuChar(UINT ch, UINT flags, HMENU hmenu) */
#define MSGMAP_WM_MENUCHAR(fn) \
	case WM_MENUCHAR: return (LRESULT)(DWORD)(fn)((UINT)(LOWORD(wParam)), (UINT)HIWORD(wParam), (HMENU)(lParam));
#define MSG_FORWARD_WM_MENUCHAR(base, ch, flags, hmenu) \
	(DWORD)(base::MessageProc)(WM_MENUCHAR, MAKEWPARAM(flags, (WORD)(ch)), (LPARAM)(HMENU)(hmenu))

/* void OnCommand(int id, HWND hCtl, UINT codeNotify) */
#define MSGMAP_WM_COMMAND(fn) \
	case WM_COMMAND: return ((fn)((int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L);
#define MSG_FORWARD_WM_COMMAND(base, id, baseCtl, codeNotify) \
	(void)(base::MessageProc)(WM_COMMAND, MAKEWPARAM((UINT)(id), (UINT)(codeNotify)), (LPARAM)(HWND)(hwndCtl))

/* void OnHScroll(HWND hCtl, UINT code, int pos) */
#define MSGMAP_WM_HSCROLL(fn) \
	case WM_HSCROLL: return ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L);
#define MSG_FORWARD_WM_HSCROLL(base, baseCtl, code, pos) \
	(void)(base::MessageProc)(WM_HSCROLL, MAKEWPARAM((UINT)(int)(code), (UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl))

/* void OnVScroll(HWND hCtl, UINT code, int pos) */
#define MSGMAP_WM_VSCROLL(fn) \
	case WM_VSCROLL: return ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L);
#define MSG_FORWARD_WM_VSCROLL(base, baseCtl, code, pos) \
	(void)(base::MessageProc)(WM_VSCROLL, MAKEWPARAM((UINT)(int)(code), (UINT)(int)(pos)), (LPARAM)(HWND)(hwndCtl))

/* void OnCut() */
#define MSGMAP_WM_CUT(fn) \
	case WM_CUT: return ((fn)(), 0L);
#define MSG_FORWARD_WM_CUT(base) \
	(void)(base::MessageProc)(WM_CUT, 0L, 0L)

/* void OnCopy() */
#define MSGMAP_WM_COPY(fn) \
	case WM_COPY: return ((fn)(), 0L);
#define MSG_FORWARD_WM_COPY(base) \
	(void)(base::MessageProc)(WM_COPY, 0L, 0L)

/* void OnPaste() */
#define MSGMAP_WM_PASTE(fn) \
	case WM_PASTE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_PASTE(base) \
	(void)(base::MessageProc)(WM_PASTE, 0L, 0L)

/* void OnClear() */
#define MSGMAP_WM_CLEAR(fn) \
	case WM_CLEAR: return ((fn)(), 0L);
#define MSG_FORWARD_WM_CLEAR(base) \
	(void)(base::MessageProc)(WM_CLEAR, 0L, 0L)

/* void OnUndo() */
#define MSGMAP_WM_UNDO(fn) \
	case WM_UNDO: return ((fn)(), 0L);
#define MSG_FORWARD_WM_UNDO(base) \
	(void)(base::MessageProc)(WM_UNDO, 0L, 0L)

/* HANDLE OnRenderFormat(UINT fmt) */
#define MSGMAP_WM_RENDERFORMAT(fn) \
	case WM_RENDERFORMAT: return (LRESULT)(UINT_PTR)(HANDLE)(fn)((UINT)(wParam));
#define MSG_FORWARD_WM_RENDERFORMAT(base, fmt) \
	(HANDLE)(UINT_PTR)(base::MessageProc)(WM_RENDERFORMAT, (WPARAM)(UINT)(fmt), 0L)

/* void OnRenderAllFormats() */
#define MSGMAP_WM_RENDERALLFORMATS(fn) \
	case WM_RENDERALLFORMATS: return ((fn)(), 0L);
#define MSG_FORWARD_WM_RENDERALLFORMATS(base) \
	(void)(base::MessageProc)(WM_RENDERALLFORMATS, 0L, 0L)

/* void OnDestroyClipboard() */
#define MSGMAP_WM_DESTROYCLIPBOARD(fn) \
	case WM_DESTROYCLIPBOARD: return ((fn)(), 0L);
#define MSG_FORWARD_WM_DESTROYCLIPBOARD(base) \
	(void)(base::MessageProc)(WM_DESTROYCLIPBOARD, 0L, 0L)

/* void OnDrawClipboard() */
#define MSGMAP_WM_DRAWCLIPBOARD(fn) \
	case WM_DRAWCLIPBOARD: return ((fn)(), 0L);
#define MSG_FORWARD_WM_DRAWCLIPBOARD(base) \
	(void)(base::MessageProc)(WM_DRAWCLIPBOARD, 0L, 0L)

/* void OnPaintClipboard(HWND hCBViewer, const LPPAINTSTRUCT lpPaintStruct) */
#define MSGMAP_WM_PAINTCLIPBOARD(fn) \
	case WM_PAINTCLIPBOARD: return ((fn)((HWND)(wParam), (const LPPAINTSTRUCT)GlobalLock((HGLOBAL)(lParam))), GlobalUnlock((HGLOBAL)(lParam)), 0L);
#define MSG_FORWARD_WM_PAINTCLIPBOARD(base, baseCBViewer, lpPaintStruct) \
	(void)(base::MessageProc)(WM_PAINTCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPPAINTSTRUCT)(lpPaintStruct))

/* void OnSizeClipboard(HWND hCBViewer, const LPRECT lprc) */
#define MSGMAP_WM_SIZECLIPBOARD(fn) \
	case WM_SIZECLIPBOARD: return ((fn)((HWND)(wParam), (const LPRECT)GlobalLock((HGLOBAL)(lParam))), GlobalUnlock((HGLOBAL)(lParam)), 0L);
#define MSG_FORWARD_WM_SIZECLIPBOARD(base, baseCBViewer, lprc) \
	(void)(base::MessageProc)(WM_SIZECLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), (LPARAM)(LPRECT)(lprc))

/* void OnVScrollClipboard(HWND hCBViewer, UINT code, int pos) */
#define MSGMAP_WM_VSCROLLCLIPBOARD(fn) \
	case WM_VSCROLLCLIPBOARD: return ((fn)((HWND)(wParam), (UINT)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_VSCROLLCLIPBOARD(base, baseCBViewer, code, pos) \
	(void)(base::MessageProc)(WM_VSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos)))

/* void OnHScrollClipboard(HWND hCBViewer, UINT code, int pos) */
#define MSGMAP_WM_HSCROLLCLIPBOARD(fn) \
	case WM_HSCROLLCLIPBOARD: return ((fn)((HWND)(wParam), (UINT)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_HSCROLLCLIPBOARD(base, baseCBViewer, code, pos) \
	(void)(base::MessageProc)(WM_HSCROLLCLIPBOARD, (WPARAM)(HWND)(hwndCBViewer), MAKELPARAM((code), (pos)))

/* void OnAskCBFormatName(int cchMax, LPTSTR rgchName) */
#define MSGMAP_WM_ASKCBFORMATNAME(fn) \
	case WM_ASKCBFORMATNAME: return ((fn)((int)(wParam), (LPTSTR)(lParam)), 0L);
#define MSG_FORWARD_WM_ASKCBFORMATNAME(base, cchMax, rgchName) \
	(void)(base::MessageProc)(WM_ASKCBFORMATNAME, (WPARAM)(int)(cchMax), (LPARAM)(rgchName))

/* void OnChangeCBChain(HWND hRemove, HWND hNext) */
#define MSGMAP_WM_CHANGECBCHAIN(fn) \
	case WM_CHANGECBCHAIN: return ((fn)((HWND)(wParam), (HWND)(lParam)), 0L);
#define MSG_FORWARD_WM_CHANGECBCHAIN(base, baseRemove, baseNext) \
	(void)(base::MessageProc)(WM_CHANGECBCHAIN, (WPARAM)(HWND)(hwndRemove), (LPARAM)(HWND)(hwndNext))

/* BOOL OnSetCursor(HWND hCursor, UINT codeHitTest, UINT msg) */
#define MSGMAP_WM_SETCURSOR(fn) \
	case WM_SETCURSOR: return (LRESULT)(DWORD)(BOOL)(fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
#define MSG_FORWARD_WM_SETCURSOR(base, baseCursor, codeHitTest, msg) \
	(BOOL)(DWORD)(base::MessageProc)(WM_SETCURSOR, (WPARAM)(HWND)(hwndCursor), MAKELPARAM((codeHitTest), (msg)))

/* void OnSysCommand(UINT cmd, int x, int y) */
#define MSGMAP_WM_SYSCOMMAND(fn) \
	case WM_SYSCOMMAND: return ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_SYSCOMMAND(base, cmd, x, y) \
	(void)(base::MessageProc)(WM_SYSCOMMAND, (WPARAM)(UINT)(cmd), MAKELPARAM((x), (y)))

/* HWND OnMDICreate(const LPMDICREATESTRUCT lpmcs) */
#define MSGMAP_WM_MDICREATE(fn) \
	case WM_MDICREATE: return (LRESULT)(DWORD)(UINT)(fn)((LPMDICREATESTRUCT)(lParam));
#define MSG_FORWARD_WM_MDICREATE(base, lpmcs) \
	(HWND)(UINT)(DWORD)(base::MessageProc)(WM_MDICREATE, 0L, (LPARAM)(LPMDICREATESTRUCT)(lpmcs))

/* void OnMDIDestroy(HWND hDestroy) */
#define MSGMAP_WM_MDIDESTROY(fn) \
	case WM_MDIDESTROY: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_MDIDESTROY(base, baseDestroy) \
	(void)(base::MessageProc)(WM_MDIDESTROY, (WPARAM)(hwndDestroy), 0L)

/* NOTE: Usable only by MDI client windows */
/* void OnMDIActivate(BOOL fActive, HWND hActivate, HWND hDeactivate) */
#define MSGMAP_WM_MDIACTIVATE(fn) \
	case WM_MDIACTIVATE: return ((fn)((BOOL)(lParam == (LPARAM)hwnd), (HWND)(lParam), (HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_MDIACTIVATE(base, fActive, baseActivate, baseDeactivate) \
	(void)(base::MessageProc)(hwnd, WM_MDIACTIVATE, (WPARAM)(hwndDeactivate), (LPARAM)(hwndActivate))

/* void OnMDIRestore(HWND hRestore) */
#define MSGMAP_WM_MDIRESTORE(fn) \
	case WM_MDIRESTORE: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_MDIRESTORE(base, baseRestore) \
	(void)(base::MessageProc)(WM_MDIRESTORE, (WPARAM)(hwndRestore), 0L)

/* HWND OnMDINext(HWND hCur, BOOL fPrev) */
#define MSGMAP_WM_MDINEXT(fn) \
	case WM_MDINEXT: return (LRESULT)(HWND)(fn)((HWND)(wParam), (BOOL)lParam);
#define MSG_FORWARD_WM_MDINEXT(base, baseCur, fPrev) \
	(HWND)(UINT_PTR)(base::MessageProc)(WM_MDINEXT, (WPARAM)(hwndCur), (LPARAM)(fPrev))

/* void OnMDIMaximize(HWND hMaximize) */
#define MSGMAP_WM_MDIMAXIMIZE(fn) \
	case WM_MDIMAXIMIZE: return ((fn)((HWND)(wParam)), 0L);
#define MSG_FORWARD_WM_MDIMAXIMIZE(base, baseMaximize) \
	(void)(base::MessageProc)(WM_MDIMAXIMIZE, (WPARAM)(hwndMaximize), 0L)

/* BOOL OnMDITile(UINT cmd) */
#define MSGMAP_WM_MDITILE(fn) \
	case WM_MDITILE: return (LRESULT)(DWORD)(fn)((UINT)(wParam));
#define MSG_FORWARD_WM_MDITILE(base, cmd) \
	(BOOL)(DWORD)(base::MessageProc)(WM_MDITILE, (WPARAM)(cmd), 0L)

/* BOOL OnMDICascade(UINT cmd) */
#define MSGMAP_WM_MDICASCADE(fn) \
	case WM_MDICASCADE: return (LRESULT)(DWORD)(fn)((UINT)(wParam));
#define MSG_FORWARD_WM_MDICASCADE(base, cmd) \
	(BOOL)(DWORD)(base::MessageProc)(WM_MDICASCADE, (WPARAM)(cmd), 0L)

/* void OnMDIIconArrange() */
#define MSGMAP_WM_MDIICONARRANGE(fn) \
	case WM_MDIICONARRANGE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_MDIICONARRANGE(base) \
	(void)(base::MessageProc)(WM_MDIICONARRANGE, 0L, 0L)

/* HWND OnMDIGetActive() */
#define MSGMAP_WM_MDIGETACTIVE(fn) \
	case WM_MDIGETACTIVE: return (LRESULT)(UINT_PTR)(fn)();
#define MSG_FORWARD_WM_MDIGETACTIVE(base) \
	(HWND)(UINT_PTR)(base::MessageProc)(WM_MDIGETACTIVE, 0L, 0L)

/* HMENU OnMDISetMenu(BOOL fRefresh, HMENU hmenuFrame, HMENU hmenuWindow) */
#define MSGMAP_WM_MDISETMENU(fn) \
	case WM_MDISETMENU: return (LRESULT)(UINT_PTR)(fn)((BOOL)(wParam), (HMENU)(wParam), (HMENU)(lParam));
#define MSG_FORWARD_WM_MDISETMENU(base, fRefresh, hmenuFrame, hmenuWindow) \
	(HMENU)(UINT_PTR)(base::MessageProc)(WM_MDISETMENU, (WPARAM)((fRefresh) ? (hmenuFrame) : 0), (LPARAM)(hmenuWindow))

/* void OnChildActivate() */
#define MSGMAP_WM_CHILDACTIVATE(fn) \
	case WM_CHILDACTIVATE: return ((fn)(), 0L);
#define MSG_FORWARD_WM_CHILDACTIVATE(base) \
	(void)(base::MessageProc)(WM_CHILDACTIVATE, 0L, 0L)

/* BOOL OnInitDialog(Focus, LPARAM lParam) */
#define MSGMAP_WM_INITDIALOG(fn) \
	case WM_INITDIALOG: return (LRESULT)(DWORD)(UINT)(BOOL)(fn)((HWND)(wParam), lParam);
#define MSG_FORWARD_WM_INITDIALOG(base, baseFocus, lParam) \
	(BOOL)(DWORD)(base::MessageProc)(WM_INITDIALOG, (WPARAM)(HWND)(hwndFocus), (lParam))

/* HWND OnNextDlgCtl(HWND hSetFocus, BOOL fNext) */
#define MSGMAP_WM_NEXTDLGCTL(fn) \
	case WM_NEXTDLGCTL: return (LRESULT)(UINT_PTR)(HWND)(fn)((HWND)(wParam), (BOOL)(lParam));
#define MSG_FORWARD_WM_NEXTDLGCTL(base, baseSetFocus, fNext) \
	(HWND)(UINT_PTR)(base::MessageProc)(WM_NEXTDLGCTL, (WPARAM)(HWND)(hwndSetFocus), (LPARAM)(fNext))

/* void OnParentNotify(UINT msg, HWND hChild, int idChild) */
#define MSGMAP_WM_PARENTNOTIFY(fn) \
	case WM_PARENTNOTIFY: return ((fn)((UINT)LOWORD(wParam), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L);
#define MSG_FORWARD_WM_PARENTNOTIFY(base, msg, baseChild, idChild) \
	(void)(base::MessageProc)(WM_PARENTNOTIFY, MAKEWPARAM(msg, idChild), (LPARAM)(hwndChild))

/* void OnEnterIdle(UINT source, HWND hSource) */
#define MSGMAP_WM_ENTERIDLE(fn) \
	case WM_ENTERIDLE: return ((fn)((UINT)(wParam), (HWND)(lParam)), 0L);
#define MSG_FORWARD_WM_ENTERIDLE(base, source, baseSource) \
	(void)(base::MessageProc)(WM_ENTERIDLE, (WPARAM)(UINT)(source), (LPARAM)(HWND)(hwndSource))

/* UINT OnGetDlgCode(LPMSG lpmsg) */
#define MSGMAP_WM_GETDLGCODE(fn) \
	case WM_GETDLGCODE: return (LRESULT)(DWORD)(UINT)(fn)(hwnd, (LPMSG)(lParam));
#define MSG_FORWARD_WM_GETDLGCODE(base, lpmsg) \
	(UINT)(DWORD)(base::MessageProc)(WM_GETDLGCODE, (lpmsg ? lpmsg->wParam : 0), (LPARAM)(LPMSG)(lpmsg))

/* HBRUSH OnCtlColor(HDC hdc, Child, int type) */
#define MSGMAP_WM_CTLCOLORMSGBOX(fn) \
	case WM_CTLCOLORMSGBOX: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_MSGBOX);
#define MSG_FORWARD_WM_CTLCOLORMSGBOX(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORMSGBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLOREDIT(fn) \
	case WM_CTLCOLOREDIT: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_EDIT);
#define MSG_FORWARD_WM_CTLCOLOREDIT(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLOREDIT, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLORLISTBOX(fn) \
	case WM_CTLCOLORLISTBOX: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_LISTBOX);
#define MSG_FORWARD_WM_CTLCOLORLISTBOX(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORLISTBOX, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLORBTN(fn) \
	case WM_CTLCOLORBTN: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_BTN);
#define MSG_FORWARD_WM_CTLCOLORBTN(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORBTN, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLORDLG(fn) \
	case WM_CTLCOLORDLG: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_DLG);
#define MSG_FORWARD_WM_CTLCOLORDLG(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORDLG, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLORSCROLLBAR(fn) \
	case WM_CTLCOLORSCROLLBAR: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_SCROLLBAR);
#define MSG_FORWARD_WM_CTLCOLORSCROLLBAR(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORSCROLLBAR, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

#define MSGMAP_WM_CTLCOLORSTATIC(fn) \
	case WM_CTLCOLORSTATIC: return (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_STATIC);
#define MSG_FORWARD_WM_CTLCOLORSTATIC(base, hdc, baseChild) \
	(HBRUSH)(UINT_PTR)(base::MessageProc)(WM_CTLCOLORSTATIC, (WPARAM)(HDC)(hdc), (LPARAM)(HWND)(hwndChild))

/* void OnSetFont(HWND hCtl, HFONT hfont, BOOL fRedraw) */
#define MSGMAP_WM_SETFONT(fn) \
	case WM_SETFONT: return ((fn)((HFONT)(wParam), (BOOL)(lParam)), 0L);
#define MSG_FORWARD_WM_SETFONT(base, hfont, fRedraw) \
	(void)(base::MessageProc)(WM_SETFONT, (WPARAM)(HFONT)(hfont), (LPARAM)(BOOL)(fRedraw))

/* HFONT OnGetFont() */
#define MSGMAP_WM_GETFONT(fn) \
	case WM_GETFONT: return (LRESULT)(UINT_PTR)(HFONT)(fn)();
#define MSG_FORWARD_WM_GETFONT(base) \
	(HFONT)(UINT_PTR)(base::MessageProc)(WM_GETFONT, 0L, 0L)

/* void OnDrawItem(const DRAWITEMSTRUCT * lpDrawItem) */
#define MSGMAP_WM_DRAWITEM(fn) \
	case WM_DRAWITEM: return ((fn)((const DRAWITEMSTRUCT *)(lParam)), 0L);
#define MSG_FORWARD_WM_DRAWITEM(base, lpDrawItem) \
	(void)(base::MessageProc)(WM_DRAWITEM, (WPARAM)(((const DRAWITEMSTRUCT *)lpDrawItem)->CtlID), (LPARAM)(const DRAWITEMSTRUCT *)(lpDrawItem))

/* void OnMeasureItem(MEASUREITEMSTRUCT * lpMeasureItem) */
#define MSGMAP_WM_MEASUREITEM(fn) \
	case WM_MEASUREITEM: return ((fn)((MEASUREITEMSTRUCT *)(lParam)), 0L);
#define MSG_FORWARD_WM_MEASUREITEM(base, lpMeasureItem) \
	(void)(base::MessageProc)(WM_MEASUREITEM, (WPARAM)(((MEASUREITEMSTRUCT *)lpMeasureItem)->CtlID), (LPARAM)(MEASUREITEMSTRUCT *)(lpMeasureItem))

/* void OnDeleteItem(const DELETEITEMSTRUCT * lpDeleteItem) */
#define MSGMAP_WM_DELETEITEM(fn) \
	case WM_DELETEITEM: return ((fn)((const DELETEITEMSTRUCT *)(lParam)), 0L);
#define MSG_FORWARD_WM_DELETEITEM(base, lpDeleteItem) \
	(void)(base::MessageProc)(WM_DELETEITEM, (WPARAM)(((const DELETEITEMSTRUCT *)(lpDeleteItem))->CtlID), (LPARAM)(const DELETEITEMSTRUCT *)(lpDeleteItem))

/* int OnCompareItem(const COMPAREITEMSTRUCT * lpCompareItem) */
#define MSGMAP_WM_COMPAREITEM(fn) \
	case WM_COMPAREITEM: return (LRESULT)(DWORD)(int)(fn)((const COMPAREITEMSTRUCT *)(lParam));
#define MSG_FORWARD_WM_COMPAREITEM(base, lpCompareItem) \
	(int)(DWORD)(base::MessageProc)(WM_COMPAREITEM, (WPARAM)(((const COMPAREITEMSTRUCT *)(lpCompareItem))->CtlID), (LPARAM)(const COMPAREITEMSTRUCT *)(lpCompareItem))

/* int OnVkeyToItem(UINT vk, Listbox, int iCaret) */
#define MSGMAP_WM_VKEYTOITEM(fn) \
	case WM_VKEYTOITEM: return (LRESULT)(DWORD)(int)(fn)((UINT)LOWORD(wParam), (HWND)(lParam), (int)(short)HIWORD(wParam));
#define MSG_FORWARD_WM_VKEYTOITEM(base, vk, baseListBox, iCaret) \
	(int)(DWORD)(base::MessageProc)(WM_VKEYTOITEM, MAKEWPARAM((vk), (iCaret)), (LPARAM)(hwndListBox))

/* int OnCharToItem(UINT ch, Listbox, int iCaret) */
#define MSGMAP_WM_CHARTOITEM(fn) \
	case WM_CHARTOITEM: return (LRESULT)(DWORD)(int)(fn)((UINT)LOWORD(wParam), (HWND)(lParam), (int)(short)HIWORD(wParam));
#define MSG_FORWARD_WM_CHARTOITEM(base, ch, baseListBox, iCaret) \
	(int)(DWORD)(base::MessageProc)(WM_CHARTOITEM, MAKEWPARAM((UINT)(ch), (UINT)(iCaret)), (LPARAM)(hwndListBox))

/* void OnQueueSync() */
#define MSGMAP_WM_QUEUESYNC(fn) \
	case WM_QUEUESYNC: return ((fn)(), 0L);
#define MSG_FORWARD_WM_QUEUESYNC(base) \
	(void)(base::MessageProc)(WM_QUEUESYNC, 0L, 0L)
#if (WINVER >= 0x030a)
/* void OnCommNotify(int cid, UINT flags) */
#define MSGMAP_WM_COMMNOTIFY(fn) \
	case WM_COMMNOTIFY: return ((fn)((int)(wParam), (UINT)LOWORD(lParam)), 0L);
#define MSG_FORWARD_WM_COMMNOTIFY(base, cid, flags) \
	(void)(base::MessageProc)(WM_COMMNOTIFY, (WPARAM)(cid), MAKELPARAM((flags), 0))
#endif

/* void OnDisplayChange(UINT bitsPerPixel, UINT cxScreen, UINT cyScreen) */
#define MSGMAP_WM_DISPLAYCHANGE(fn) \
	case WM_DISPLAYCHANGE: return ((fn)((UINT)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_DISPLAYCHANGE(base, bitsPerPixel, cxScreen, cyScreen) \
	(void)(base::MessageProc)(WM_DISPLAYCHANGE, (WPARAM)(UINT)(bitsPerPixel), (LPARAM)MAKELPARAM((UINT)(cxScreen), (UINT)(cyScreen)))

/* BOOL OnDeviceChange(UINT uEvent, DWORD dwEventData) */
#define MSGMAP_WM_DEVICECHANGE(fn) \
	case WM_DEVICECHANGE: return (LRESULT)(DWORD)(BOOL)(fn)((UINT)(wParam), (DWORD)(lParam));
#define MSG_FORWARD_WM_DEVICECHANGE(base, uEvent, dwEventData) \
	(BOOL)(DWORD)(base::MessageProc)(WM_DEVICECHANGE, (WPARAM)(UINT)(uEvent), (LPARAM)(DWORD)(dwEventData))

/* void OnContextMenu(HWND hContext, UINT xPos, UINT yPos) */
#define MSGMAP_WM_CONTEXTMENU(fn) \
	case WM_CONTEXTMENU: return ((fn)((HWND)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_CONTEXTMENU(base, baseContext, xPos, yPos) \
	(void)(base::MessageProc)(WM_CONTEXTMENU, (WPARAM)(HWND)(hwndContext), MAKELPARAM((UINT)(xPos), (UINT)(yPos)))

/* void OnCopyData(HWND hSender, PCOPYDATASTRUCT pcds) */
#define MSGMAP_WM_COPYDATA(fn) \
	case WM_COPYDATA: return ((fn)((HWND)(wParam), (PCOPYDATASTRUCT)lParam), 0L);
#define MSG_FORWARD_WM_COPYDATA(base, baseFrom, pcds) \
	(BOOL)(UINT)(DWORD)(base::MessageProc)(WM_COPYDATA, (WPARAM)(hwndFrom), (LPARAM)(pcds))

/* void OnHotKey(int idHotKey, UINT fuModifiers, UINT vk) */
#define MSGMAP_WM_HOTKEY(fn) \
	case WM_HOTKEY: return ((fn)((int)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
#define MSG_FORWARD_WM_HOTKEY(base, idHotKey, fuModifiers, vk) \
	(void)(base::MessageProc)(WM_HOTKEY, (WPARAM)(idHotKey), MAKELPARAM((fuModifiers), (vk)))
