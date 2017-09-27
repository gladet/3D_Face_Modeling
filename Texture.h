#if !defined(AFX_TEXTURE_H__2C72F92E_677B_4C21_B78E_84E9A431419E__INCLUDED_)
#define AFX_TEXTURE_H__2C72F92E_677B_4C21_B78E_84E9A431419E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Texture.h : header file
//

#include "OGLEventDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CTexture view

class CTexture : public CView
{
protected:
	CTexture();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTexture)

// Attributes
public:
	COGLEventDoc* GetDocument();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTexture)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTexture();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTexture)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Texture.cpp
inline COGLEventDoc* CTexture::GetDocument()
   { return (COGLEventDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTURE_H__2C72F92E_677B_4C21_B78E_84E9A431419E__INCLUDED_)
