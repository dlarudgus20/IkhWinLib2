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

#include <IkhWinLib2/CWindow.h>
#include <IkhWinLib2/CImageList.h>
#include <IkhWinLib2/CDbBufDC.h>
#include <IkhWinLib2/CDDBitmap.h>
using namespace IkhProgram::IkhWinLib2;

class CMainWnd final : public CWindow
{
	DECLARE_MSGMAP();
public:
	CMainWnd();
	void Create();

private:
	enum
	{
		IMAGE_SIZE = 48,
		BOARD_SIZE = 12,
		GAP = 10,
		MASK_COLOR = RGB(128, 255, 128),

		COUNT_OF_ITEMS = 12,
	};

	static const int WndSize = IMAGE_SIZE * BOARD_SIZE + GAP * 2;
	static const int ExtraWndWidth = GAP + 40;
	static const int ExtraWndHeight = 68 + IMAGE_SIZE + GAP * 2;
	static const DWORD WndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER;
	static const RECT ClientRect;

	CImageList m_HWImageList;
	CDDBitmap m_DieMark;
	CDDBitmap m_HWImage;

	enum Direction { LEFT, RIGHT, UP, DOWN };
	Direction m_Direction;
	std::list<POINT> m_Player;

	bool m_bLose = false;
	POINT m_LosePoint;
	bool m_bLoseHead = true;

	unsigned m_NextChrIdx = 1;
	uint32_t m_CompleteCount = 0;

	uint32_t m_NextItem;

	static const uint32_t PlayerHeadMask = 0x80000000U;
	static const uint32_t PlayerMask = 0x40000000U;
	static const uint32_t ImageMask = 0x3fffffffU;
	uint32_t m_board[BOARD_SIZE][BOARD_SIZE];
	static const RECT BoardRt;

	static const uint32_t InitTimeOut = 10;
	uint32_t m_TimeOut = InitTimeOut;

	std::unique_ptr<CDbBufBitmap> m_pDbBit;

	std::default_random_engine m_RandEng;
	std::uniform_int_distribution<LONG> m_RandDis;

	BOOL IsAroundHead(const POINT &p);

protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnPaint();
	void OnKey(UINT vk, BOOL fDown, int cRepeat, UINT flags);
	void OnTimer(UINT id);
	void OnSizing(UINT flags, LPRECT lpRect);
	void OnDestroy();
};

inline POINT AddPoint(const POINT &lhs, const POINT &rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}
inline bool operator ==(const POINT &lhs, const POINT &rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}
inline bool operator !=(const POINT &lhs, const POINT &rhs)
{
	return !(lhs == rhs);
}
