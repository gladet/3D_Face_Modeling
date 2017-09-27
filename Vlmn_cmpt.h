#if !defined(AFX_VLMN_CMPT_H__2F0AF516_7E99_4A8E_A4FB_484D2674DE7F__INCLUDED_)
#define AFX_VLMN_CMPT_H__2F0AF516_7E99_4A8E_A4FB_484D2674DE7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Vlmn_cmpt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Vlmn_cmpt dialog

class Vlmn_cmpt : public CDialog
{
// Construction
public:
	Vlmn_cmpt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Vlmn_cmpt)
	enum { IDD = IDD_VLMN_CMPT };
	CString	m_vlmn_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Vlmn_cmpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Vlmn_cmpt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VLMN_CMPT_H__2F0AF516_7E99_4A8E_A4FB_484D2674DE7F__INCLUDED_)
