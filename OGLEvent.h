// OGLEvent.h : main header file for the OGLEVENT application
//

#if !defined(AFX_OGLEVENT_H__B821E1D6_85B0_495A_9F6A_9D510F48E3C4__INCLUDED_)
#define AFX_OGLEVENT_H__B821E1D6_85B0_495A_9F6A_9D510F48E3C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COGLEventApp:
// See OGLEvent.cpp for the implementation of this class
//

class COGLEventApp : public CWinApp
{
public:
	COGLEventApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLEventApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);	
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COGLEventApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGLEVENT_H__B821E1D6_85B0_495A_9F6A_9D510F48E3C4__INCLUDED_)
