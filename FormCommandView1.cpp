// FormCommandView1.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "MainFrm.h"
#include "FormCommandView1.h"
#include "OGLEventView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormCommandView1

IMPLEMENT_DYNCREATE(CFormCommandView1, CFormView)

CFormCommandView1::CFormCommandView1()
	: CFormView(CFormCommandView1::IDD)
{
	//{{AFX_DATA_INIT(CFormCommandView1)
	BackColor = RGB(0,0,0);
	Face1Color = RGB(127,127,255);
	AmbientColor = RGB(0,0,0);
	DiffuseColor = RGB(255,255,255);
	SpecularColor = RGB(0,0,0);
	m_M11 = _T("");
	m_M12 = _T("");
	m_M13 = _T("");
	m_M14 = _T("");
	m_M21 = _T("");
	m_M22 = _T("");
	m_M23 = _T("");
	m_M24 = _T("");
	m_M31 = _T("");
	m_M32 = _T("");
	m_M33 = _T("");
	m_M34 = _T("");
	m_error_sum = _T("");
	//}}AFX_DATA_INIT
}

CFormCommandView1::~CFormCommandView1()
{
}

void CFormCommandView1::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormCommandView1)
	DDX_Control(pDX, IDC_CHECK_TXTMAP, m_txtmap);
	DDX_Control(pDX, IDC_FRAME_COLOR_FACE1, m_Face1);
	DDX_Control(pDX,IDC_RADIO_MODEL0,m_ControlModel0);
	DDX_Control(pDX,IDC_RADIO_MODEL1,m_ControlModel1);
	DDX_Control(pDX,IDC_RADIO_MODEL2,m_ControlModel2);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_SPECULAR, m_ControlColorLightSpecular);
	DDX_Control(pDX, IDC_FRAME_COLOR_BACK, m_ControlBackColor);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_DIFFUSE, m_ControlColorLightDiffuse);
	DDX_Control(pDX, IDC_FRAME_COLOR_LIGHT_AMBIENT, m_ControlColorLightAmbient);
	DDX_Control(pDX, IDC_SLIDERSCALEX, m_SliderScaleX);
	DDX_Check(pDX, IDC_CHECK_LIGHTING, m_Lighting);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_Smooth);
	DDX_Check(pDX, IDC_CHECK_VROTATION, m_VRotation);
	DDX_Check(pDX, IDC_CHECK_Rot_X, m_Rot_X);
	DDX_Check(pDX, IDC_CHECK_Rot_Y, m_Rot_Y);
	DDX_Check(pDX, IDC_CHECK_Rot_Z, m_Rot_Z);
	DDX_Control(pDX,IDC_RADIO_PROJ,m_Proj);
	DDX_Control(pDX,IDC_RADIO_ORTH,m_Orth);
	DDX_Check(pDX, IDC_CHECK_AXIS, m_axis);
	DDX_Check(pDX, IDC_CHECK_ANTIALIAS, m_Antialias);
	DDX_Text(pDX, IDC_M11, m_M11);
	DDX_Text(pDX, IDC_M12, m_M12);
	DDX_Text(pDX, IDC_M13, m_M13);
	DDX_Text(pDX, IDC_M14, m_M14);
	DDX_Text(pDX, IDC_M21, m_M21);
	DDX_Text(pDX, IDC_M22, m_M22);
	DDX_Text(pDX, IDC_M23, m_M23);
	DDX_Text(pDX, IDC_M24, m_M24);
	DDX_Text(pDX, IDC_M31, m_M31);
	DDX_Text(pDX, IDC_M32, m_M32);
	DDX_Text(pDX, IDC_M33, m_M33);
	DDX_Text(pDX, IDC_M34, m_M34);
	DDX_Text(pDX, IDC_ERROR_SUM, m_error_sum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormCommandView1, CFormView)
	//{{AFX_MSG_MAP(CFormCommandView1)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK_LIGHTING, OnCheckLighting)
	ON_BN_CLICKED(IDC_CHECK_SMOOTH, OnCheckSmooth)
	ON_BN_CLICKED(IDC_CHECK_VROTATION, OnCheckVrotation)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_ANTIALIAS, OnCheckAntialias)
	ON_BN_CLICKED(IDC_RADIO_MODEL_VERTEX, OnRadioModelVertex)
	ON_BN_CLICKED(IDC_RADIO_MODEL_WIREFRAME, OnRadioModelWireframe)
	ON_BN_CLICKED(IDC_RADIO_MODEL_FACE, OnRadioModelFace)
	ON_BN_CLICKED(IDC_CHECK_Rot_X, OnCHECKRotX)
	ON_BN_CLICKED(IDC_CHECK_Rot_Y, OnCHECKRotY)
	ON_BN_CLICKED(IDC_CHECK_Rot_Z, OnCHECKRotZ)
	ON_BN_CLICKED(IDC_RADIO_PROJ, OnRadioProj)
	ON_BN_CLICKED(IDC_RADIO_ORTH, OnRadioOrth)
	ON_BN_CLICKED(IDC_CHECK_AXIS, OnCheckAxis)
	ON_BN_CLICKED(IDC_RADIO_PHOTO, OnRadioPhoto)
	ON_BN_CLICKED(IDC_RADIO_COLOR, OnRadioColor)
	ON_BN_CLICKED(IDC_CHECK_TXTMAP, OnCheckTxtmap)
	ON_BN_CLICKED(IDC_CHECK_MESHPROJ, OnCheckMeshproj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormCommandView1 diagnostics

#ifdef _DEBUG
void CFormCommandView1::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormCommandView1::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG

COGLEventDoc* CFormCommandView1::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COGLEventDoc)));
	return (COGLEventDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// CFormCommandView1 message handlers
//********************************************
// OnPaint
//********************************************
void CFormCommandView1::OnPaint() 
{
	// Device context for painting
	CPaintDC dc(this); 
	
	// Options are stored in Application
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	CRect rect;
	
	// Color back
	m_ControlBackColor.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushBack(BackColor);
	dc.FillRect(&rect,&BrushBack);

	m_Face1.GetWindowRect(&rect);
	ScreenToClient(&rect);
	CBrush BrushFace1(Face1Color);
	dc.FillRect(&rect,&BrushFace1);

	// Color light ambient
	m_ControlColorLightAmbient.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightAmbient(AmbientColor);
	dc.FillRect(&rect,&BrushLightAmbient);

	// Color light diffuse
	m_ControlColorLightDiffuse.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightDiffuse(DiffuseColor);
	dc.FillRect(&rect,&BrushLightDiffuse);

	// Color light specular
	m_ControlColorLightSpecular.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	CBrush BrushLightSpecular(SpecularColor);
	dc.FillRect(&rect,&BrushLightSpecular);
}

//********************************************
// GetRenderView
//********************************************
CView *CFormCommandView1::GetOGLEventView() 
{
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	CView *pView = (CView *)pFrame->m_wndSplitter2.GetPane(0,0);
	return pView;
}

//********************************************
// OnLButtonUp
//********************************************
void CFormCommandView1::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rect;
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	float r,g,b;

	// back color
	m_ControlBackColor.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(BackColor);
		if(dlg.DoModal()==IDOK)
		{
			BackColor = dlg.GetColor();	
			COGLEventView *pView = (COGLEventView *)GetOGLEventView();
			r = (float)GetRValue(BackColor) / 255.0f;
			g = (float)GetGValue(BackColor) / 255.0f;
			b = (float)GetBValue(BackColor) / 255.0f;
			pView->m_ClearColorRed   = r;
			pView->m_ClearColorGreen = g;
			pView->m_ClearColorBlue  = b;
			glClearColor(r,g,b,1.0f);
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE);
		}
	}

	m_Face1.GetWindowRect(&rect);	
	ScreenToClient(&rect);
	if(rect.PtInRect(point))
	{
		CColorDialog dlg(Face1Color);
		if(dlg.DoModal()==IDOK)
		{
			Face1Color = dlg.GetColor();	
			COGLEventView *pView = (COGLEventView *)GetOGLEventView();
			r = (float)GetRValue(Face1Color) / 255.0f;
			g = (float)GetGValue(Face1Color) / 255.0f;
			b = (float)GetBValue(Face1Color) / 255.0f;
			pView->m_Face1Red   = r;
			pView->m_Face1Green = g;
			pView->m_Face1Blue  = b;
			this->InvalidateRect(&rect,FALSE);
			pView->InvalidateRect(NULL,FALSE);
		}
	}

	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(m_Lighting == 1)//pDoc->m_style == 0 || pDoc->m_style == 2)
	{			
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);			
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);

		// ambient light color
		m_ControlColorLightAmbient.GetWindowRect(&rect);	
		ScreenToClient(&rect);
		if(rect.PtInRect(point))
		{
			CColorDialog dlg(AmbientColor);
			if(dlg.DoModal()==IDOK)
			{
				AmbientColor = dlg.GetColor();	
				COGLEventView *pView = (COGLEventView *)GetOGLEventView();
				// Refresh Light0
				r = (float)GetRValue(AmbientColor) / 255.0f;
				g = (float)GetGValue(AmbientColor) / 255.0f;
				b = (float)GetBValue(AmbientColor) / 255.0f;
				float ambientProperties[]  = {r,g,b,0.5f};
				glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
				// Refresh views
				this->InvalidateRect(&rect,FALSE);
				pView->InvalidateRect(NULL,FALSE); 
			}
		}		
		
		// specular light color
		m_ControlColorLightSpecular.GetWindowRect(&rect);	
		ScreenToClient(&rect);
		if(rect.PtInRect(point))
		{
			CColorDialog dlg(SpecularColor);
			if(dlg.DoModal()==IDOK)
			{
				SpecularColor = dlg.GetColor();	
				COGLEventView *pView = (COGLEventView *)GetOGLEventView();
				// Refresh Light0
				float r = (float)GetRValue(SpecularColor) / 255.0f;
				float g = (float)GetGValue(SpecularColor) / 255.0f;
				float b = (float)GetBValue(SpecularColor) / 255.0f;
				float	specularProperties[]  = {r,g,b,0.5f};
				glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
				// Refresh views
				this->InvalidateRect(&rect,FALSE);
				pView->InvalidateRect(NULL,FALSE); 
			}
		}		

		// diffuse light color
		m_ControlColorLightDiffuse.GetWindowRect(&rect);	
		ScreenToClient(&rect);
		if(rect.PtInRect(point))
		{
			CColorDialog dlg(DiffuseColor);
			if(dlg.DoModal()==IDOK)
			{
				DiffuseColor = dlg.GetColor();	
				COGLEventView *pView = (COGLEventView *)GetOGLEventView();
				// Refresh Light0
				float r = (float)GetRValue(DiffuseColor) / 255.0f;
				float g = (float)GetGValue(DiffuseColor) / 255.0f;
				float b = (float)GetBValue(DiffuseColor) / 255.0f;
				float	diffuseProperties[]  = {r,g,b,0.5f};
				glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
				// Refresh views
				this->InvalidateRect(&rect,FALSE);
				pView->InvalidateRect(NULL,FALSE); 
			}
		}
	}

	CFormView::OnLButtonUp(nFlags, point);
}
//********************************************
// OnCheckLighting
//********************************************
void CFormCommandView1::OnCheckLighting() 
{
	COGLEventDoc *pDoc = GetDocument();		
	if(pDoc->m_style == 0 || pDoc->m_style == 2)
	{
		m_Lighting = !m_Lighting;
		if(m_Lighting)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		this->GetOGLEventView()->InvalidateRect(NULL,FALSE);
	}	 
}

//********************************************
// OnCheckSmooth
//********************************************
void CFormCommandView1::OnCheckSmooth() 
{
	m_Smooth = !m_Smooth;
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->m_smooth = !pView->m_smooth;
	if(m_Smooth)
		glShadeModel(GL_SMOOTH);
	else
		glShadeModel(GL_FLAT);
	this->GetOGLEventView()->InvalidateRect(NULL,FALSE); 
	
}

void CFormCommandView1::OnCheckVrotation() 
{
	m_VRotation = !m_VRotation;
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	if(m_VRotation)
		pView->SetTimer(1,10,NULL);
	else
		pView->KillTimer(1);
}

//********************************************
// OnInitialUpdate
//********************************************
void CFormCommandView1::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// Slider
	TRACE("Sliders : updating...\n");
	m_SliderScaleX.SetRange(1,100,TRUE);

	m_SliderScaleX.SetPos(30);

	m_ControlModel2.SetCheck(BST_CHECKED);
	m_Orth.SetCheck(BST_CHECKED);

	m_VRotation = FALSE;
	m_axis = FALSE;
	m_Antialias = FALSE;
	m_Lighting = TRUE;//默认为有光照
	m_Smooth = TRUE;//默认为此显示模式
	
	m_Rot_X = TRUE;//FALSE;
	m_Rot_Y = TRUE;
	m_Rot_Z = FALSE;
	
	this -> UpdateData(FALSE);
}

//********************************************
// OnHScroll
//********************************************
void CFormCommandView1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateScale();
	GetOGLEventView()->InvalidateRect(NULL,FALSE); 
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CFormCommandView1::UpdateScale() 
{
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->m_xScaling = pView->m_yScaling = pView->m_zScaling = (float)m_SliderScaleX.GetPos()/20.0f;
	//pView->m_yScaling = (float)m_SliderScaleY.GetPos()/20.0f;
	//pView->m_zScaling = (float)m_SliderScaleZ.GetPos()/20.0f;

	/*if(m_LinkScale)
	{
		m_SliderScaleY.SetPos(m_SliderScaleX.GetPos());
		m_SliderScaleZ.SetPos(m_SliderScaleX.GetPos());
		pView->m_yScaling = pView->m_zScaling = pView->m_xScaling;
	}*/

	return TRUE;
}

//********************************************
// OnCheckAntialias
// Toggle antialiased lines
//********************************************
void CFormCommandView1::OnCheckAntialias() 
{
	m_Antialias = !m_Antialias;

	if(m_Antialias)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glLineWidth(1.5f);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
		glLineWidth(1.0f);
	}
	GetOGLEventView()->InvalidateRect(NULL,FALSE); 
}

void CFormCommandView1::OnRadioModelVertex() 
{
	// TODO: Add your control notification handler code here
	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	m_ControlModel0.SetCheck(BST_CHECKED);
	m_ControlModel1.SetCheck(BST_UNCHECKED);
	m_ControlModel2.SetCheck(BST_UNCHECKED);

	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_style = 1;
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	this->GetOGLEventView()->InvalidateRect(NULL,FALSE);	
}

void CFormCommandView1::OnRadioModelWireframe() 
{
	// TODO: Add your control notification handler code here
	m_ControlModel1.SetCheck(BST_CHECKED);
	m_ControlModel0.SetCheck(BST_UNCHECKED);
	m_ControlModel2.SetCheck(BST_UNCHECKED);

	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_style = 0;

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	this->GetOGLEventView()->InvalidateRect(NULL,FALSE); 	
}

void CFormCommandView1::OnRadioModelFace() 
{
	// TODO: Add your control notification handler code here
	m_ControlModel2.SetCheck(BST_CHECKED);
	m_ControlModel0.SetCheck(BST_UNCHECKED);
	m_ControlModel1.SetCheck(BST_UNCHECKED);

	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_style = 2;

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	m_Lighting = TRUE;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	this->GetOGLEventView()->InvalidateRect(NULL,FALSE); 	
}


void CFormCommandView1::OnCHECKRotX() 
{
	m_Rot_X = !m_Rot_X;
	CView * temp1 = this->GetOGLEventView();
	COGLEventView *temp = (COGLEventView *)temp1;
	temp -> Rot_X = !(temp -> Rot_X);
	if(m_Rot_X)
		m_Rot_Z = FALSE;
	temp->Rot_Z = FALSE;
	this -> UpdateData(FALSE);

	/*
	CWnd *temp = this -> GetDlgItem(IDC_CHECK_x);
	CRect rect;
	temp -> GetWindowRect(&rect);
	this->InvalidateRect(&rect,FALSE);
	*/	
}

void CFormCommandView1::OnCHECKRotY() 
{
	m_Rot_Y = !m_Rot_Y;
	CView * temp1 = this->GetOGLEventView();
	COGLEventView *temp = (COGLEventView *)temp1;	
	temp -> Rot_Y = !(temp -> Rot_Y);
}

void CFormCommandView1::OnCHECKRotZ() 
{
	m_Rot_Z = !m_Rot_Z;
	CView * temp1 = this->GetOGLEventView();
	COGLEventView *temp = (COGLEventView *)temp1;	
	temp -> Rot_Z = !(temp -> Rot_Z);
	if(m_Rot_Z)
		m_Rot_X = FALSE;
	temp -> Rot_X = FALSE;
	this -> UpdateData(FALSE);		
}

void CFormCommandView1::OnRadioProj() 
{
	COGLEventView * temp;
	CView *temp1 = this->GetOGLEventView();
	temp = (COGLEventView *)temp1;
	temp->View_Mode = false;
	temp->View_Changed = true;
	this->GetOGLEventView()->InvalidateRect(NULL, FALSE);
	
}

void CFormCommandView1::OnRadioOrth() 
{
	COGLEventView * temp;
	CView *temp1 = this->GetOGLEventView();
	temp = (COGLEventView *)temp1;
	temp->View_Mode = true;
	temp->View_Changed = true;		
	this->GetOGLEventView()->InvalidateRect(NULL, FALSE);
}

void CFormCommandView1::OnCheckAxis() 
{
	m_axis = !m_axis;	

	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
		
	if(m_axis)
	{
		pView->m_axis = true;
	}
	else
		pView->m_axis = false;	

	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView1::OnRadioPhoto() 
{
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->which_txt = 2;		

	pView->InvalidateRect(NULL, FALSE);
}

void CFormCommandView1::OnRadioColor() 
{
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->which_txt = 1;		

	pView->InvalidateRect(NULL, FALSE);	
}

void CFormCommandView1::OnCheckTxtmap() 
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->txt_mapped = !pDoc->txt_mapped;
	pDoc->UpdateAllViews(NULL);
}

void CFormCommandView1::OnCheckMeshproj() 
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->mesh_projed = !pDoc->mesh_projed;
	pDoc->UpdateAllViews(NULL);	
}


