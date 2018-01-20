
// RANSAC_EllipseFitting.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRANSAC_EllipseFittingApp:
// See RANSAC_EllipseFitting.cpp for the implementation of this class
//

class CRANSAC_EllipseFittingApp : public CWinAppEx
{
public:
	CRANSAC_EllipseFittingApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CRANSAC_EllipseFittingApp theApp;