#if !defined(AFX_VIEWMAP_H__0A41EC58_828B_4816_A357_C1DD532A6B47__INCLUDED_)
#define AFX_VIEWMAP_H__0A41EC58_828B_4816_A357_C1DD532A6B47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ViewMap dialog

class ViewMap : public CDialog
{
// Construction
public:
	ViewMap(CWnd* pParent = NULL);   // standard constructor
	void OnOK();	
	CWnd* m_pParent;

// Dialog Data
	//{{AFX_DATA(ViewMap)
	enum { IDD = IDD_VIEW_MAPPING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViewMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ViewMap)
		afx_msg void OnDestroy();	
	afx_msg void OnPoseAdj();
	afx_msg void OnProjMapping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMAP_H__0A41EC58_828B_4816_A357_C1DD532A6B47__INCLUDED_)
