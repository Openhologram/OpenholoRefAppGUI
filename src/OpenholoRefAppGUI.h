
// OpenholoRefAppGUI.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define REG_KEY L"Openholo-RefApp"

// COpenholoRefApp:
// See OpenholoRefAppGUI.cpp for the implementation of this class
//

class COpenholoRefApp : public CWinApp
{
public:
	COpenholoRefApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern COpenholoRefApp theApp;