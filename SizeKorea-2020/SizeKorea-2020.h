
// SizeKorea-2020.h : main header file for the SizeKorea-2020 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CSizeKorea2020App:
// See SizeKorea-2020.cpp for the implementation of this class
//

class CSizeKorea2020App : public CWinAppEx
{
public:
	CSizeKorea2020App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CSizeKorea2020App theApp;
