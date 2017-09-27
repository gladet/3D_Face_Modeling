#if !defined(AFX_OPERATION_H__28ED22F3_3883_4C6A_A5EC_6BE31C1B7AD2__INCLUDED_)
#define AFX_OPERATION_H__28ED22F3_3883_4C6A_A5EC_6BE31C1B7AD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Operation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Operation form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class Operation : public CFormView
{
protected:
	Operation();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(Operation)

// Form Data
public:
	//{{AFX_DATA(Operation)
	enum { IDD = IDD_OPERATION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Operation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~Operation();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(Operation)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATION_H__28ED22F3_3883_4C6A_A5EC_6BE31C1B7AD2__INCLUDED_)
