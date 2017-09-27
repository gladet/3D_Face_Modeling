#if !defined(AFX_FORMCOMMANDVIEW1_H__DFF3ADD2_466E_43A7_ABAC_D64078734FD2__INCLUDED_)
#define AFX_FORMCOMMANDVIEW1_H__DFF3ADD2_466E_43A7_ABAC_D64078734FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormCommandView1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "OGLEventDoc.h"

// CFormCommandView1 form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFormCommandView1 : public CFormView
{
protected:
	CFormCommandView1();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFormCommandView1)

// Form Data
public:
	//{{AFX_DATA(CFormCommandView1)
	enum { IDD = IDD_FORM_COMMAND };
	CButton	m_txtmap;
	CStatic	m_Face1;
	COLORREF BackColor;
	COLORREF Face1Color;
	COLORREF AmbientColor;
	COLORREF DiffuseColor;
	COLORREF SpecularColor;	
	CButton  m_ControlModel0;
	CButton  m_ControlModel1;
	CButton  m_ControlModel2;	
	CStatic	m_ControlColorLightSpecular;
	CStatic	m_ControlBackColor;
	CStatic	m_ControlColorLightDiffuse;
	CStatic	m_ControlColorLightAmbient;
	CSliderCtrl	m_SliderScaleX;
	BOOL	m_Lighting;
	BOOL	m_Smooth;
	BOOL	m_VRotation;
	BOOL	m_Rot_X;
	BOOL	m_Rot_Y;
	BOOL	m_Rot_Z;
	CButton  m_Proj;
	CButton  m_Orth;
	BOOL	m_axis;
	BOOL	m_Antialias;
	CString	m_M11;
	CString	m_M12;
	CString	m_M13;
	CString	m_M14;
	CString	m_M21;
	CString	m_M22;
	CString	m_M23;
	CString	m_M24;
	CString	m_M31;
	CString	m_M32;
	CString	m_M33;
	CString	m_M34;
	CString	m_error_sum;
	//}}AFX_DATA

// Attributes
public:
	COGLEventDoc *GetDocument();
	CView *GetOGLEventView(); 
	BOOL UpdateScale(); 
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormCommandView1)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormCommandView1();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormCommandView1)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCheckLighting();
	afx_msg void OnCheckSmooth();
	afx_msg void OnCheckVrotation();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckAntialias();
	afx_msg void OnRadioModelVertex();
	afx_msg void OnRadioModelWireframe();
	afx_msg void OnRadioModelFace();
	afx_msg void OnCHECKRotX();
	afx_msg void OnCHECKRotY();
	afx_msg void OnCHECKRotZ();
	afx_msg void OnRadioProj();
	afx_msg void OnRadioOrth();
	afx_msg void OnCheckAxis();
	afx_msg void OnRadioPhoto();
	afx_msg void OnRadioColor();
	afx_msg void OnCheckTxtmap();
	afx_msg void OnCheckMeshproj();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMCOMMANDVIEW1_H__DFF3ADD2_466E_43A7_ABAC_D64078734FD2__INCLUDED_)
