#include <IkhWinLib2/CMsgTarget.h>
using namespace IkhProgram::IkhWinLib2;

#define WM_MY_MESSAGE (WM_USER + 1)

// CMyMsgTarget.h
class CMyMsgTarget : public CMsgTarget
{
	DECLARE_MSGMAP();
protected:
	BOOL OnCreate(LPCREATESTRUCT lpcs);
	void OnDestroy();
	LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
};

// CMyMsgTarget.cpp
BEGIN_MSGMAP(CMyMsgTarget, CMsgTarget)
	MSGMAP_WM_CREATE(OnCreate)
	MSGMAP_WM_DESTROY(OnDestroy)
	MSGMAP_CUSTOM(WM_MY_MESSAGE, OnMyMessage)
END_MSGMAP(CMyMsgTarget, CMsgTarget)

BOOL CMyMsgTarget::OnCreate(LPCREATESTRUCT lpcs)
{
	if (!MSG_FORWARD_WM_CREATE(CMsgTarget, lpcs))
		return FALSE;

	// TODO..
	return TRUE;
}

void CMyMsgTarget::OnDestroy()
{
	// TODO..

	MSG_FORWARD_WM_DESTROY(CMsgTarget);
}

LRESULT CMyMsgTarget::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	MSG_FORWARD_CUSTOM(CMsgTarget, WM_MY_MESSAGE, wParam, lParam);

	// TODO..
	return 0;
}
