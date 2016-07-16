#include <IkhWinLib2/CApplication.h>
using namespace IkhProgram::IkhWinLib2;

// CMyApp.h
class CMyApp final : public CApplication
{
private:
	virtual int InitInstance() override;
};

// CMyApp.cpp
#include "CMyApp.h"

#include "CMyWindow.h"

IKHWINLIB2_APP_CLS(CMyApp)

int CMyApp::InitInstance()
{
	CMyWindow *pWnd = new CMyWindow;

	pWnd->Create();
	ShowWindow(*pWnd, SW_NORMAL);

	return (int)Run();
}
