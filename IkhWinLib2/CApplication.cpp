// Copyright (c) 2014, ÀÓ°æÇö
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

#include "stdafx.h"
#include "IkhWinLib2/CApplication.h"
#include "IkhWinLib2/CMsgTarget.h"

using namespace IkhProgram::IkhWinLib2;

BEGIN_IKHWINLIB2()

std::unique_ptr<CApplication> CApplication::ms_pApp;
DWORD CApplication::ms_TlsModeless;

int CApplication::IWLMain()
{
	CWin32Thread::StaticCtor();

	struct Initor
	{
		Initor()
		{
			ms_TlsModeless = TlsAlloc();
			InitCommonControls();
			ms_pApp.reset(::CreateApplication());
		}
		~Initor()
		{
			ms_pApp.reset();
			TlsFree(ms_TlsModeless);
		}
	} _initor;

	return ms_pApp->Main(__argc, __wargv);
}

CApplication::CApplication() NOEXCEPT
{
	Attach(GetCurrentThread());
}

CApplication::~CApplication() NOEXCEPT
{
	Deattach();
}

END_IKHWINLIB2()

int main()
{
	return CApplication::IWLMain();
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return CApplication::IWLMain();
}
