#if !defined(AFX_AREA_CMPT_H__765D7F4F_0DCF_4B42_ABD0_AD8F191E9891__INCLUDED_)
#define AFX_AREA_CMPT_H__765D7F4F_0DCF_4B42_ABD0_AD8F191E9891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Area_cmpt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Area_cmpt dialog

class Area_cmpt : public CDialog
{
// Construction
public:
	Area_cmpt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Area_cmpt)
	enum { IDD = IDD_AREA_CMPT };
	CString	m_area_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Area_cmpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Area_cmpt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AREA_CMPT_H__765D7F4F_0DCF_4B42_ABD0_AD8F191E9891__INCLUDED_)
