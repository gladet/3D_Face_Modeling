#if !defined(AFX_DATA_INPUT_H__3DECB572_8BCF_4D0E_A529_7501545DF3B2__INCLUDED_)
#define AFX_DATA_INPUT_H__3DECB572_8BCF_4D0E_A529_7501545DF3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Data_input.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Data_input dialog

class Data_input : public CDialog
{
// Construction
public:
	Data_input(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Data_input)
	enum { IDD = IDD_DATA_INPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Data_input)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Data_input)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATA_INPUT_H__3DECB572_8BCF_4D0E_A529_7501545DF3B2__INCLUDED_)
