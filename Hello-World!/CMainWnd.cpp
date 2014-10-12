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

#include <IkhWinLib2/CWndClass.h>
#include <IkhWinLib2/CDbBufDC.h>

#include "resource.h"
#include "CMainWnd.h"

enum
{
	MOVE_TIMER_ELAPSE = 300,
	MOVE_TIMER = 0,
	CREATION_TIMER_ELAPSE = 3000,
	CREATION_TIMER = 1,
	TIMELIM_TIMER_ELAPSE = 1000,
	TIMELIM_TIMER = 2,
};

BEGIN_MSGMAP(CMainWnd, CWindow)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_PAINT(OnPaint)
	MSGMAP_WM_KEYDOWN(OnKey)
	MSGMAP_WM_TIMER(OnTimer)
	MSGMAP_WM_SIZING(OnSizing)
	MSGMAP_WM_DESTROY(OnDestroy)
END_MSGMAP(CMainWnd, CWindow)

const RECT CMainWnd::ClientRect = { 0, 0, WndSize + ExtraWndWidth, WndSize + ExtraWndHeight };
const RECT CMainWnd::BoardRt = { 0, 0, BOARD_SIZE, BOARD_SIZE };
static const TCHAR strHelloWorld[] = L"hello-world!";

CMainWnd::CMainWnd()
	: m_HWImageList(CImageList::Load(MAKEINTRESOURCE(IDB_HELLOWORLD_LIST), IMAGE_SIZE, MASK_COLOR))
	, m_HWImage(MAKEINTRESOURCE(IDB_HELLOWORLD_LIST))
	, m_DieMark(MAKEINTRESOURCE(IDB_DIE_MARK))
	, m_RandEng(GetTickCount()), m_RandDis(0, BOARD_SIZE - 1)
{
	memset(m_board, 0, sizeof(m_board));
	m_board[3][3] = 2;
	m_board[6][6] = PlayerHeadMask | 1;
	m_Player.push_front({ 6, 6 });
	m_Direction = LEFT;
	m_NextItem = 3;
	m_LosePoint = { -1, -1 };
}
void CMainWnd::Create()
{
	RECT rt = ClientRect;
	AdjustWindowRect(&rt, WndStyle, FALSE);

	CWindow::Create(
		CWndClass(nullptr, CS_HREDRAW | CS_VREDRAW, NULL),
		L"Hello-world! (made by 암겨혀, with IkhWinLib2 library)",
		WndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, rt.right - rt.left, rt.bottom - rt.top
		);
}

BOOL CMainWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CWindow, lpcs))
		return FALSE;

	MessageBox(*this,
		L"hello-world!\n\n"
		L"네.. 어디서 많이 봤던 벌레 키우기(?)게임입니다. 규칙도 거의 같고요.\n"
		L"하지만 이 게임만의 특별한 규칙이 있는데, **무.조.건** \"hello-world!\" 순서대로 아이템을 먹야아 합니다.\n"
		L"(끝에 느낌표를 빼먹으면 큰일난다는데...)\n"
		L"일정 시간마다 벌레가 한칸 앞으로 갑니다. 진행 방향은 방향키로 조정할 수 있습니다.\n"
		L"주의할 점은 실수로 벌레 몸통쪽으로 방향을 잡아버리면 벌레가 자기 몸쪽으로 이동하려다 머리박고 사망(...)합니다.\n"
		L"또한 게임판의 벽을 맞아도 사망하는데, 이 게임에서는 게임판의 격자라던지 벽이 그려져 있지 않으므로 조심해야 합니다 ^^\n"
		L"그리고 이 게임은 10초의 시간제한이 있습니다. 10초마다 최소 한개씩 아이템을 먹지 않으면 사.망. 입니다\n"
		L"\"hello-world!\"를 가능한 한 많이 만들어야 합니다.\n"
		L"그럼, 즐겜이용 <<\n\n"
		L"made by 임경현 (dlarudgus20, 암겨혀), with IkhWinLib2 library. BSD (2-Clause) license.\n"
		L"github page: https://github.com/dlarudgus20/IkhWinLib2",
		L"도움말!", MB_OK | MB_ICONINFORMATION);

	m_pDbBit.reset(new CDbBufBitmap(*this, (HBRUSH)(COLOR_WINDOW + 1)));
	SetTimer(*this, MOVE_TIMER, MOVE_TIMER_ELAPSE, nullptr);
	SetTimer(*this, CREATION_TIMER, CREATION_TIMER_ELAPSE, nullptr);
	SetTimer(*this, TIMELIM_TIMER, TIMELIM_TIMER_ELAPSE, nullptr);

	return TRUE;
}

void CMainWnd::OnPaint()
{
	if (!m_pDbBit)
		return;

	CDbBufDC dc = m_pDbBit->GetDbBufDC();
	dc.Clear();

	for (int x = 0; x < BOARD_SIZE; x++)
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			if (m_board[y][x] != 0)
			{
				UINT fStyle = ILD_TRANSPARENT;

				if (m_board[y][x] & PlayerHeadMask)
				{
					fStyle |= ILD_BLEND50;
				}
				else if (m_board[y][x] & PlayerMask)
				{
					fStyle |= ILD_BLEND25;
				}

				ImageList_Draw(m_HWImageList, (m_board[y][x] & ImageMask) - 1,
					dc, GAP + x * IMAGE_SIZE, GAP + y * IMAGE_SIZE, fStyle);
			}
		}
	}

	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbHatch = 0;
	lb.lbColor = 0xedd88d;
	HPEN hPen = ExtCreatePen(
		PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND,
		10, &lb, 0, nullptr
		);
	HPEN hOldPen = (HPEN)SelectObject(dc, hPen);
	int OldRop = GetROP2(dc);
	SetROP2(dc, R2_XORPEN);

	POINT pts[COUNT_OF_ITEMS];
	auto it = m_Player.rbegin();
	while (1)
	{
		for (int i = 0; i < COUNT_OF_ITEMS; i++, it++)
		{
			if (it == m_Player.rend())
				goto break_2;

			pts[i] = {
				GAP + it->x * IMAGE_SIZE + IMAGE_SIZE / 2,
				GAP + it->y * IMAGE_SIZE + IMAGE_SIZE / 2
			};
		}

		Polyline(dc, pts, COUNT_OF_ITEMS);
	}
break_2:
	DeleteObject(SelectObject(dc, hOldPen));
	SetROP2(dc, OldRop);

	TextOut(dc, GAP, WndSize + GAP, L"남은 시간 : " + std::to_wstring(m_TimeOut) + L" 초");

	if (m_bLose)
	{
		TextOut(dc, GAP, WndSize + 20 + GAP * 2, L"패배!");

		POINT p;
		if (m_bLoseHead)
		{
			p = m_Player.front();
			m_DieMark.DrawTransparent(dc,
				GAP + p.x * IMAGE_SIZE, GAP + p.y * IMAGE_SIZE,
				-1, -1, 0, 0, -1, -1,
				MASK_COLOR);
		}
		if (PtInRect(&BoardRt, m_LosePoint))
		{
			p = m_LosePoint;
			m_DieMark.DrawTransparent(dc,
				GAP + p.x * IMAGE_SIZE, GAP + p.y * IMAGE_SIZE,
				-1, -1, 0, 0, -1, -1,
				MASK_COLOR);
		}
	}

	if (m_CompleteCount > 0)
	{
		m_HWImage.DrawTransparent(dc,
			GAP, WndSize + 68 + GAP,
			-1, -1, 0, 0, -1, -1,
			MASK_COLOR);

		TextOut(dc,
			GAP + IMAGE_SIZE * COUNT_OF_ITEMS + GAP, WndSize + 68 + GAP,
			L"x " + std::to_wstring(m_CompleteCount));
	}

	dc.Draw();
}

void CMainWnd::OnKey(UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
	{
		case VK_LEFT: m_Direction = LEFT; break;
		case VK_RIGHT: m_Direction = RIGHT; break;
		case VK_UP: m_Direction = UP; break;
		case VK_DOWN: m_Direction = DOWN; break;
	}
}

void CMainWnd::OnTimer(UINT id)
{
	if (m_bLose)
		return;

	static POINT dp[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

	switch (id)
	{
		case MOVE_TIMER:
		{
			POINT ptTail = m_Player.back();
			uint32_t tail = m_board[ptTail.y][ptTail.x];

			uint32_t item = 0;

			// head 부분 처리
			auto it = m_Player.begin();
			POINT p = AddPoint(*it, dp[m_Direction]);
			if (!PtInRect(&BoardRt, p)									/* 게임판 밖이거나 */
				|| ((m_board[p.y][p.x] & PlayerMask) && p != ptTail)	/* 몸통을 밟거나 */
				|| (p == ptTail && p == *std::next(m_Player.begin()))	/* 몸길이 2인 상태에서 꼬리를 밟거나 */
				)
			{
				m_bLose = true;
				m_LosePoint = p;
				break;
			}
			else if (m_board[p.y][p.x] != 0 && p != ptTail)
			{
				// 아이템 충돌 처리
				item = m_board[p.y][p.x];

				if (strHelloWorld[item - 1] != strHelloWorld[m_NextChrIdx])
				{
					m_bLose = true;
					m_bLoseHead = false;
					m_LosePoint = p;
					break;
				}

				if (++m_NextChrIdx == COUNT_OF_ITEMS)
				{
					m_NextChrIdx = 0;
					m_CompleteCount++;

					PlaySound(MAKEINTRESOURCE(IDW_COMPLETE_ITEM), GetModuleHandle(NULL),
						SND_ASYNC | SND_RESOURCE);
				}
				else
				{
					PlaySound(MAKEINTRESOURCE(IDW_EAT_ITEM), GetModuleHandle(NULL),
						SND_ASYNC | SND_RESOURCE);
				}

				item |= PlayerMask;
			}
			m_board[p.y][p.x] = m_board[it->y][it->x];
			swap(*it, p);

			// 몸통 처리
			for (++it; it != m_Player.end(); ++it)
			{
				m_board[p.y][p.x] = m_board[it->y][it->x];
				swap(*it, p);
			}

			// tail 부분 처리
			m_board[p.y][p.x] = item;
			m_board[m_Player.back().y][m_Player.back().x] = tail;

			// 아이템 획득 처리
			if (item != 0)
			{
				m_Player.push_back(p);
				m_TimeOut = InitTimeOut;
			}

			break;
		}
		case CREATION_TIMER:
		{
			POINT p;
			do 
			{
				p = { m_RandDis(m_RandEng), m_RandDis(m_RandEng) };
			} while (m_board[p.y][p.x] != 0 || IsAroundHead(p));

			m_board[p.y][p.x] = m_NextItem;
			if (++m_NextItem == COUNT_OF_ITEMS + 1)
				m_NextItem = 1;

			break;
		}
		case TIMELIM_TIMER:
		{
			if (--m_TimeOut == 0)
			{
				m_bLose = true;
			}
			break;
		}
	}

	if (m_bLose)
		PlaySound(MAKEINTRESOURCE(IDW_GAMEOVER), GetModuleHandle(NULL), SND_ASYNC | SND_RESOURCE);

	InvalidateRect(*this, NULL, FALSE);
}

void CMainWnd::OnSizing(UINT flags, LPRECT lpRect)
{
	RECT rt = ClientRect;
	AdjustWindowRect(&rt, WndStyle, FALSE);

	lpRect->right = lpRect->left + (rt.right - rt.left);
	lpRect->bottom = lpRect->top + (rt.bottom - rt.top);
}

void CMainWnd::OnDestroy()
{
	PostQuitMessage(0);
	MSG_FORWARD_WM_DESTROY(CWindow);
}

BOOL CMainWnd::IsAroundHead(const POINT &p)
{
	POINT head = m_Player.front();
	RECT rt = { head.x - 1, head.y - 1, head.x + 2, head.y + 2 };
	return PtInRect(&rt, p);
}
