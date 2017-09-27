// OGLEventView.h : interface of the COGLEventView class
//
/////////////////////////////////////////////////////////////////////////////
//#include "Fract.h"
#include "OGLEventDoc.h"

#if !defined(AFX_OGLEVENTVIEW_H__87125D0D_F0D1_498A_B778_DA00B6A8AFC8__INCLUDED_)
#define AFX_OGLEVENTVIEW_H__87125D0D_F0D1_498A_B778_DA00B6A8AFC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COGLEventView : public CView
{
protected: // create from serialization only
	COGLEventView();
	DECLARE_DYNCREATE(COGLEventView)

// Attributes
public:
	COGLEventDoc* GetDocument();

// Operations
public:
	HGLRC m_hGLContext;
	int m_GLPixelIndex;

	// Mouse 
	BOOL m_LeftButtonDown;
	BOOL m_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_init_LeftDownPos;
	CPoint m_RightDownPos;
	HCURSOR m_CursorRotation;

	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	void InitGeometry(void);	
	void DrawWithOpenGL();

	//Fract *fra_mount;
	int steps;
	
	float m_xRotation;
	float m_yRotation;
	float m_zRotation;
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;
	float m_xScaling;
	float m_yScaling;
	float m_zScaling;

	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	float m_Face1Red;
	float m_Face1Green;
	float m_Face1Blue;

	bool m_axis;
	bool m_smooth;//判断是以smooth还是flat模式显示

	BOOL Rot_X;
	BOOL Rot_Y;
	BOOL Rot_Z;

	BOOL size_initialized;

	BOOL View_Mode;
	BOOL View_Changed;

	int which_txt;

	int size_x, size_y;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLEventView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COGLEventView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COGLEventView)
		afx_msg void OnDestroy();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnPaint();
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//		afx_msg void OnModelingFract();
	afx_msg void OnFrontView();
	afx_msg void OnRightView();
	afx_msg void OnLeftView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OGLEventView.cpp
inline COGLEventDoc* COGLEventView::GetDocument()
   { return (COGLEventDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGLEVENTVIEW_H__87125D0D_F0D1_498A_B778_DA00B6A8AFC8__INCLUDED_)
