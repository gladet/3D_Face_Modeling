// OGLEventView.cpp : implementation of the COGLEventView class
//

#include "stdafx.h"
#include "OGLEvent.h"

#include "OGLEventDoc.h"
#include "OGLEventView.h"
#include "ViewMap.h"

#include <math.h>

//#define PI 3.17159265

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COGLEventView

IMPLEMENT_DYNCREATE(COGLEventView, CView)

BEGIN_MESSAGE_MAP(COGLEventView, CView)
	//{{AFX_MSG_MAP(COGLEventView)
		ON_WM_DESTROY()
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_PAINT()
		ON_WM_TIMER()
		ON_WM_CREATE()
//		ON_COMMAND(ID_Modeling_Fract, OnModelingFract)
	ON_COMMAND(ID_FRONT_VIEW, OnFrontView)
	ON_COMMAND(ID_RIGHT_VIEW, OnRightView)
	ON_COMMAND(ID_LEFT_VIEW, OnLeftView)
	//}}AFX_MSG_MAP
		// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COGLEventView construction/destruction

COGLEventView::COGLEventView()
{
	// OpenGL
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	
	// Mouse
	m_LeftButtonDown = false;
	m_RightButtonDown = false;
	m_CursorRotation = AfxGetApp()->LoadCursor(IDC_CURSOR_ROTATION);

	m_axis = false;
	m_smooth = true;

	Rot_X = true;
	Rot_Y =	true;
	Rot_Z =	false;

	size_initialized = false;

	View_Mode = true;
	View_Changed = false;

	which_txt = 1;//�����ж�����ʾ�������õ�ɫ��ʾ��Ĭ��Ϊ��ɫ��ʾ

	// Colors
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
//	fra_mount = new Fract();
	InitGeometry();

}

//********************************************
// InitGeometry
//********************************************
void COGLEventView::InitGeometry(void)
{
	m_xRotation = 0.0f;
	m_yRotation = 0.0f;
	m_zRotation = 0.0f;
	m_xTranslation = 0.0f;
	m_yTranslation = 0.0f;
	m_zTranslation = -3.0f;
	m_xScaling = 1.0f;
	m_yScaling = 1.0f;
	m_zScaling = 1.0f;
}


COGLEventView::~COGLEventView()
{
}

BOOL COGLEventView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COGLEventView drawing

void COGLEventView::OnDraw(CDC* pDC)
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COGLEventView printing

BOOL COGLEventView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COGLEventView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COGLEventView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COGLEventView diagnostics

#ifdef _DEBUG
void COGLEventView::AssertValid() const
{
	CView::AssertValid();
}

void COGLEventView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COGLEventDoc* COGLEventView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COGLEventDoc)));
	return (COGLEventDoc*)m_pDocument;
}
#endif //_DEBUG

//********************************************
// OnCreate
// Create OpenGL rendering context 
//********************************************
int COGLEventView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if(SetWindowPixelFormat(hDC)==false)
		return 0;
	
	if(CreateViewGLContext(hDC)==false)
		return 0;
	
	// Default mode
	glPolygonMode(GL_FRONT, GL_FILL);//GL_LINE);
	//glPolygonMode(GL_BACK,GL_LINE);
	//
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);	

	m_Face1Red = 0.5f;
	m_Face1Green = 0.5f;
	m_Face1Blue = 1.0f;

	glClearDepth( 1.0f );		

	GLfloat	light_position_0[] = {30.0f, 30.0f, 30.0f, 1.0f};
	GLfloat	light_position_1[] = {-30.0f, 30.0f, 30.0f, 1.0f};
	
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_0);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
	
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//GLfloat	mat_diffuse[] = {0.0, 0.0, 0.0, 1.0};
	//glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);

	// Lights, material properties
	GLfloat	ambientProperties[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat	diffuseProperties[]  = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat	specularProperties[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lmodel_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	
	GLfloat local_view[] = {0.0f};//viewer is local?
	
	m_ClearColorRed=0.0f;
	m_ClearColorGreen=0.0f;
	m_ClearColorBlue=0.0f;
	
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);

	glLightfv( GL_LIGHT1, GL_AMBIENT, ambientProperties);
	glLightfv( GL_LIGHT1, GL_DIFFUSE, diffuseProperties);
	glLightfv( GL_LIGHT1, GL_SPECULAR, specularProperties);
	glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);//?	
	glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
	
	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_style == 0 || pDoc->m_style == 2)
	{			
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);			
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// COGLEventView message handlers
//********************************************
// OnDestroy
// Cleanup every OpenGL rendering context
//********************************************
void COGLEventView::OnDestroy() 
{
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	
	CView::OnDestroy();
}

//********************************************
// OnSize
//********************************************
void COGLEventView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	size_x = cx;
	size_y = cy;
	
	// Set OpenGL perspective, viewport and mode
	CSize size(cx,cy);
	double aspect;
	aspect = (cy == 0)?(double)size.cx:(double)size.cx/(double)size.cy;

	glViewport(0,0,size.cx,size.cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(!View_Mode)//��DrawWithOpenGLͳһ����
		glFrustum(-0.875, 0.875, -1, 1, 0.75, 4);	
	else
		glOrtho (-1.5, 1.5, -1.5*(GLfloat)cy/(GLfloat)cx, 1.5*(GLfloat)cy/(GLfloat)cx, -10.0, 10.0);
	//gluPerspective(45, aspect, 1.0f, 15.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);
	
	//clear background
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);

	/*
	if(size_initialized == false)
	{
		size_initialized = true;
		glEnable(GL_DEPTH_TEST);	
	}
	*/
}

void COGLEventView::OnLButtonDown(UINT nFlags, 
								CPoint point) 
{
	m_LeftButtonDown = true;
	m_LeftDownPos = m_init_LeftDownPos = point;
	
	CView::OnLButtonDown(nFlags, point);
}

void COGLEventView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_LeftButtonDown = false;

	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
	
	if(pDoc->pose_adj)
	{
		pDoc->x_rot = pDoc->y_rot = pDoc->x_rot = 0;
	}
	
	if(m_init_LeftDownPos == point && pDoc->obj_fp_lc == false && pDoc->num_view == 0)//Ҫ�����㰴ǰ��λ�ò��䣬����ת���֣� ����ͼ���Ͽ�ʼ����Ͳ�����Ч
	{
		double x, y, z;
		double modelMatrix[16];
		double projMatrix[16];
		int viewport[4];
		
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
		glGetIntegerv(GL_VIEWPORT, viewport);
		
		float depth;
		
		//�Ѽ�����Ȳ���ʱ����ȡ�����е����ֵ
		if(glIsEnabled(GL_DEPTH_TEST))
		{
			glReadPixels(
			point.x,
			viewport[3] - point.y - 1,//point.y,
			1,
			1,
			GL_DEPTH_COMPONENT,
			GL_FLOAT,
			&depth
			);
		}
		
		//��ȡ��ά����λ��
		if(gluUnProject(
			point.x,//(double)point.x/viewport[2]*1.75-0.875,
			viewport[3] - point.y - 1,//(double)point.y/viewport[3]*2-1,
			depth,//0,//1.25,
			modelMatrix,
			projMatrix,
			viewport,
			&x,
			&y,
			&z
			))
		{
			//λ�˵���ǰ���궨��������ݽṹ�в���	
			if(pDoc->which_loc == 1)
			{
				double* temp_dbpt = new double[3];
				
				temp_dbpt[0] = x;
				temp_dbpt[1] = y;
				temp_dbpt[2] = z;

				pDoc->mdl_pts.insert(pDoc->mdl_pts.end(),temp_dbpt);

				//ͬʱҲ��pre_mdl_pts��¼
				temp_dbpt = NULL;
				temp_dbpt = new double[3];
				
				temp_dbpt[0] = x;
				temp_dbpt[1] = y;
				temp_dbpt[2] = z;
				pDoc->pre_mdl_pts.insert(pDoc->pre_mdl_pts.end(),temp_dbpt);

				pDoc->mdl_pt_num++;//�ȼ�¼�������ٽ�����Ŀ����
			}
			else
			{
				//��mdl_pt_numȡֵ����0��4��mdl_temp_pt���ú����أ���ȫ��Ϊ��ʾ�����
				pDoc->mdl_temp_pt[pDoc->mdl_pt_num][0] = x;
				pDoc->mdl_temp_pt[pDoc->mdl_pt_num][1] = y;
				pDoc->mdl_temp_pt[pDoc->mdl_pt_num][2] = z;

				//���ۼӴ��Գ�ʼ����ʼ��
				if(pDoc->mdl_pt_num%4 == 0)
				{
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][0] = 0;
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][1] = 0;
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][2] = 0;

					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][0] = 0;
					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][1] = 0;
					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][2] = 0;					
				}

				//�ۼ�
				pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][0] += x;
				pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][1] += y;
				pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4][2] += z;

				pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][0] += x;
				pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][1] += y;
				pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4][2] += z;				
				
				pDoc->mdl_pt_num++;//�ȼ�¼�������ٽ�����Ŀ����
				
				//��ƽ��
				if(pDoc->mdl_pt_num%4 == 0)
				{
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4-1][0] /= 4;
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4-1][1] /= 4;
					pDoc->mdl_ft_pt[pDoc->mdl_pt_num/4-1][2] /= 4;

					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4-1][0] /= 4;
					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4-1][1] /= 4;
					pDoc->pre_mdl_ft_pt[pDoc->mdl_pt_num/4-1][2] /= 4;
				}
				
				if(pDoc->mdl_pt_num == 8)		
				{
					pDoc->obj_fp_lc = true;
				}	
			}				
		}

		//pDoc->UpdateAllViews(NULL);
		InvalidateRect(NULL, false);
		//DrawWithOpenGL();//�޷Ǿ����ػ�ģ����ӱ궨�����
	}
	
	CView::OnLButtonUp(nFlags, point);
}

void COGLEventView::OnMouseMove(UINT nFlags, 
							CPoint point) 
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		

	if(m_LeftButtonDown)
	{
		if(!pDoc->pose_adj)
		{
			if(Rot_X)
				m_xRotation += (float)(point.y - m_LeftDownPos.y) / 3.0f;
			if(Rot_Z)
				m_zRotation -= (float)(point.y - m_LeftDownPos.y) / 3.0f;
			if(Rot_Y)
				m_yRotation += (float)(point.x - m_LeftDownPos.x) / 3.0f;
			
			m_LeftDownPos = point;
			InvalidateRect(NULL,false);
		}
		else 
		{
			if(Rot_X)
				pDoc->x_rot = -(float)(point.y - m_LeftDownPos.y) / 3.0f;
			if(Rot_Z)
				pDoc->z_rot = (float)(point.y - m_LeftDownPos.y) / 3.0f;
			if(Rot_Y)
				pDoc->y_rot = (float)(point.x - m_LeftDownPos.x) / 3.0f;
			
			m_LeftDownPos = point;
			InvalidateRect(NULL,false);
		}
	}
	CView::OnMouseMove(nFlags, point);
}

//********************************************
// OnPaint
//********************************************
void COGLEventView::OnPaint() 
{
	// Device context for painting
	CPaintDC dc(this); 
	
	// Useful in singledoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	DrawWithOpenGL();
	// Double buffer
	SwapBuffers(hDC); //dc.m_ps.hdc
}

//********************************************
// SetWindowPixelFormat
//********************************************
BOOL COGLEventView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return false;
	}
	
	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
		return false;
	
	return true;
}

//********************************************
// CreateViewGLContext
// Create an OpenGL OGLEventing context
//********************************************
BOOL COGLEventView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
		return false;
	
	if(wglMakeCurrent(hDC,m_hGLContext)==false)
		return false;
	
	return true;
}

void COGLEventView::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
		{
		case 0:
			break;
		//Rotation
		case 1:
			m_yRotation += 5.0f;
			InvalidateRect(NULL, false);
			break;
		default:
			{}
		}
}

void COGLEventView::DrawWithOpenGL()
{
	COGLEventDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����������������ݲ�����һ���ֵĲ���
	if(pDoc->m_read == true)
	{
		int i, j, k;

		pDoc->Tran_To_Center();		
		
		//��һ��ֻ��Ϊ��ʾ����
		//��������������������ϸ��������������һ��, ��Ϊ������������ͬ���ĳ߶ȹ�һ����ʾ
		double maxv, max_z, min_z, x, y, z;
		double max1[3] = {0.0, 0.0, 0.0};
		
		maxv = max_z = min_z = 0;

		for(i = 0; i < pDoc->M; i++)
		{
			if(pDoc->face1_points[i][2] > max_z) 
					max_z = pDoc->face1_points[i][2];//�����ݵ�z���������ֵ
			else if(pDoc->face1_points[i][2] < min_z) 
					min_z = pDoc->face1_points[i][2];//�����ݵ�z��������Сֵ

			for(j = 0; j < 3; j++)
			{
				if(fabs(pDoc->face1_points[i][j]) > max1[j]) 
					max1[j] = (float)fabs(pDoc->face1_points[i][j]);//���������ݵ�������ľ���ֵ�����ֵ		
			}
		}

		
		for(i = 0; i < 3; i++)
		{
			if(max1[i] > maxv)
				maxv = max1[i];
		}		

		//z�������Сֵ�߶ȱ任
		max_z /= maxv;
		min_z /= maxv;
		
		pDoc->Max_V = maxv;
		
		// clear background
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);

		//������͸��ͶӰ���л�
		if(View_Changed)
		{
			View_Changed = false;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if(!View_Mode)//ע�������ǵ�View_Mode=falseʱΪ͸��ͶӰ
			{
				if(!pDoc->transformed && !pDoc->with_txt)//û�и������Լ����ɵ�����
					glFrustum(-0.875, 0.875, -1, 1, 2-max_z-0.5, 2-min_z+0.5);
				else 
					glFrustum(-0.875, 0.875, -1, 1, min_z-(-2)-0.5, max_z-(-2)+0.5);	
			}
			else
				glOrtho (-1.5, 1.5, -1.5*(GLfloat)size_y/(GLfloat)size_x, 1.5*(GLfloat)size_y/(GLfloat)size_x, -10.0, 10.0);
			glMatrixMode(GL_MODELVIEW);
		}

		glLoadIdentity();
		//if(!pDoc->transformed && !pDoc->with_txt)//û�и������Լ����ɵ�����
		{			
			gluLookAt(0,0,2,0,0,0,0,1,0);
		}
		/*
		else
		{
			gluLookAt(0,0,-2,0,0,0,1,0,0);
		}*/
		
		// scale  
		glScalef(m_xScaling,m_yScaling,m_zScaling);

		// rotation
		//if(!pDoc->pose_adj)
		{
			glRotatef(m_xRotation,1.0f,0.0f,0.0f);		
			glRotatef(m_yRotation,0.0f,1.0f,0.0f);
			glRotatef(m_zRotation,0.0f,0.0f,1.0f);
		}
		
		if(pDoc->m_style == 0 || pDoc->m_style == 2)
		{
			double tri_vertexs[3][3];
			double tri_normals[3][3];

			if((!pDoc->textured && !pDoc->with_txt) || ((pDoc->textured || pDoc->with_txt) && which_txt == 1))//û����������obj�ļ���û�и�������ͼ��Ҫ��ɫ��ʾ
			{
				glColor3f(m_Face1Red, m_Face1Green, m_Face1Blue);
				glBegin(GL_TRIANGLES);
				//����ÿ��������
				for(i = 0; i < pDoc->L1; i++)
				{
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//if(pDoc->mapped && pDoc->vtx_map[pDoc->iter_index-1][pDoc->triangles1[i][0]][0] != -1 && pDoc->vtx_map[pDoc->iter_index-1][pDoc->triangles1[i][1]][0] != -1 && pDoc->vtx_map[pDoc->iter_index-1][pDoc->triangles1[i][2]][0] != -1)
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////								
					{					
						//���������ε��������㼰�䷨����
						for(j = 0; j < 3; j++)
						{
							for(k = 0; k < 3; k++)
							{
								//pDoc->triangles1[i][j] = pDoc->triangles1[i][j];
								tri_normals[j][k] = pDoc->face1_normals[pDoc->nml_tris[i][j]][k];
								tri_vertexs[j][k] = pDoc->face1_points[pDoc->triangles1[i][j]][k]/maxv;							
							}
						}						
						
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//if(pDoc->data_cmpt)
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						{
							if(m_smooth)//smooth style
							{
								//���������ε��������㼰�䷨����
								for(j = 0; j < 3; j++)
								{
									{
										glNormal3f(tri_normals[j][0], tri_normals[j][1], tri_normals[j][2]);
										//glColor3f(pDoc->vtx_gray[pDoc->triangles1[i][j]]/256, pDoc->vtx_gray[pDoc->triangles1[i][j]]/256, pDoc->vtx_gray[pDoc->triangles1[i][j]]/256);									
										glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
									}
								}	
							}
							else//flat style
							{
								//���������㷨������ƽ������ʾ��Ƭ�ķ�����
								glNormal3f((tri_normals[0][0]+tri_normals[1][0]+tri_normals[2][0])/3, 
									(tri_normals[0][1]+tri_normals[1][1]+tri_normals[2][1])/3, 
									(tri_normals[0][2]+tri_normals[1][2]+tri_normals[2][2])/3);
								for(j = 0; j < 3; j++)
								{
									//glColor3f(pDoc->vtx_gray[pDoc->triangles1[i][j]]/256, pDoc->vtx_gray[pDoc->triangles1[i][j]]/256, pDoc->vtx_gray[pDoc->triangles1[i][j]]/256);																			
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}
						}
					}
				}
				glEnd();				
			}
			else//��ȷ�����������ʾ
			{
				unsigned char *lpData ;
				unsigned long biHeight;
				unsigned long biWidth;
				unsigned long biAlign;
				//double **temp_txt;					
				
				if(pDoc->with_txt)//obj�ļ��и���������������Ϣ
				{
					if(pDoc->txt_type)
					{
						//���������������Ӧ��λͼ��Ϣ
						unsigned char* pBitmap = pDoc -> m_pBitmap;
						if(pBitmap == NULL)
						{
							//which_txt = 1;//���ǵ���һ��Ҫ��ʱ�ٱ�Ϊ2
							AfxMessageBox("you must open the relevant image at first");
							return;
						}
						LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
						//LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)pBitmap;
						lpData = pBitmap + ((LPBITMAPFILEHEADER)pBitmap)->bfOffBits;
						biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
						biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
						biAlign = (biWidth*3+3)/4 *4;
						pDoc->temp_txt = pDoc->texture_map;//������������texture_map��¼��					
						
						int u,v,cur;
						double tri_texture[3][2];
						
						glBegin(GL_TRIANGLES);
						
						for(i = 0; i < pDoc->L1; i++)
						{
							//���������ε��������㼰�䷨����					
							for(j = 0; j < 3; j++)
							{
								for(k = 0; k < 3; k++)
								{
									tri_vertexs[j][k] = pDoc->face1_points[pDoc->triangles1[i][j]][k]/maxv;
									tri_normals[j][k] = pDoc->face1_normals[pDoc->nml_tris[i][j]][k];
									if(k < 2)
									{
										if(pDoc->with_txt)//���obj�ļ�������������Ϣ����Ҳ�����������и����˵�ǰ�������������Ӧ�����
											tri_texture[j][k] = pDoc->temp_txt[pDoc->txt_tris[i][j]][k];
										else
											tri_texture[j][k] = pDoc->temp_txt[pDoc->triangles1[i][j]][k];//���붥������ϸ��Ӧ��
										while(tri_texture[j][k] < 0)
											tri_texture[j][k] += 1;
										while(tri_texture[j][k] > 1)
											tri_texture[j][k] -= 1;
									}
								}
							}
							
							if(m_smooth)//smooth style
							{
								//���������ε��������㼰�䷨����
								for(j = 0; j < 3; j++)
								{
									u = (tri_texture[j][0]*(biWidth-1) - (int)(tri_texture[j][0]*(biWidth-1)))<((int)(tri_texture[j][0]*(biWidth-1))+1 - tri_texture[j][0]*(biWidth-1))?(int)(tri_texture[j][0]*(biWidth-1)):(int)(tri_texture[j][0]*(biWidth-1))+1;
									v = (tri_texture[j][1]*(biHeight-1) - (int)(tri_texture[j][1]*(biHeight-1)))<((int)(tri_texture[j][1]*(biHeight-1))+1 - tri_texture[j][1]*(biHeight-1))?(int)(tri_texture[j][1]*(biHeight-1)):(int)(tri_texture[j][1]*(biHeight-1))+1;
									if(u == biWidth)
										u -= 1;
									if(v == biHeight)
										v -= 1;
									cur = v*biAlign+3*u;
									glColor3f((double)lpData[cur + 2]/256, (double)lpData[cur + 1]/256, (double)lpData[cur]/256);
									glNormal3f(tri_normals[j][0], tri_normals[j][1], tri_normals[j][2]);						
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}
							else//flat style
							{
								//���������㷨������ƽ������ʾ��Ƭ�ķ�����
								glNormal3f((tri_normals[0][0]+tri_normals[1][0]+tri_normals[2][0])/3, 
									(tri_normals[0][1]+tri_normals[1][1]+tri_normals[2][1])/3, 
									(tri_normals[0][2]+tri_normals[1][2]+tri_normals[2][2])/3);
								for(j = 0; j < 3; j++)
								{
									u = (tri_texture[j][0]*(biWidth-1) - (int)(tri_texture[j][0]*(biWidth-1)))<((int)(tri_texture[j][0]*(biWidth-1))+1 - tri_texture[j][0]*(biWidth-1))?(int)(tri_texture[j][0]*(biWidth-1)):(int)(tri_texture[j][0]*(biWidth-1))+1;
									v = (tri_texture[j][1]*(biHeight-1) - (int)(tri_texture[j][1]*(biHeight-1)))<((int)(tri_texture[j][1]*(biHeight-1))+1 - tri_texture[j][1]*(biHeight-1))?(int)(tri_texture[j][1]*(biHeight-1)):(int)(tri_texture[j][1]*(biHeight-1))+1;
									if(u == biWidth)
										u -= 1;
									if(v == biHeight)
										v -= 1;
									cur = v*biAlign+3*u;
									glColor3f((double)lpData[cur + 2]/256, (double)lpData[cur + 1]/256, (double)lpData[cur]/256);
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}					
						}	
						
						glEnd();
					}
					else
					{
						glBegin(GL_TRIANGLES);						
									
						for(i = 0; i < pDoc->L1; i++)
						{
							for(j = 0; j < 3; j++)
							{
								for(k = 0; k < 3; k++)
								{
									tri_vertexs[j][k] = pDoc->face1_points[pDoc->triangles1[i][j]][k]/maxv;
									tri_normals[j][k] = pDoc->face1_normals[pDoc->nml_tris[i][j]][k];								
								}
							}						
							
							if(m_smooth)//smooth style
							{
								//���������ε��������㼰�䷨����
								for(j = 0; j < 3; j++)
								{
									//glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][2]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][0]/256);
									glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][0]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][2]/256);
									glNormal3f(tri_normals[j][0], tri_normals[j][1], tri_normals[j][2]);						
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}
							else//flat style
							{
								//���������㷨������ƽ������ʾ��Ƭ�ķ�����
								glNormal3f((tri_normals[0][0]+tri_normals[1][0]+tri_normals[2][0])/3, 
										(tri_normals[0][1]+tri_normals[1][1]+tri_normals[2][1])/3, 
										(tri_normals[0][2]+tri_normals[1][2]+tri_normals[2][2])/3);
								for(j = 0; j < 3; j++)
								{
									//glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][2]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][0]/256);
									glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][0]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][2]/256);
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}	
						}	

						glEnd();
					}
				}
				else//ʹ���Լ����ɵ��������꼰��Ӧ��ͼ
				{
					if(!pDoc->cylindrical_map || !pDoc->txt_mapped)//��ʹ���ںϵ���������ͼ��ֻ���뵱ǰ��ͼ��Ӧ������ͼ
					{					
						unsigned char* temp_view_pt = pDoc->temp_view;
						
						/*
						list_charpt::iterator itrt_charpt;
						//������ͼָ�������е����һ��Ԫ��
						for(itrt_charpt = pDoc->view_pt.begin(); itrt_charpt != pDoc->view_pt.end(); ++itrt_charpt)
						{
							temp_view_pt = (*itrt_charpt);
						}
						*/

						if(temp_view_pt == NULL)
							return;
						LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)(temp_view_pt+14);
						LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)temp_view_pt;
						lpData = NULL;
						lpData = temp_view_pt + lpBitmapFileHeader->bfOffBits;
						biHeight = lpBitmapInfoHeader->biHeight;
						biWidth = lpBitmapInfoHeader->biWidth;
						biAlign = (biWidth*3+3)/4 *4;

						/*
						list_dbpt_2::iterator itrt_dbpt_2;
						//��������ͼָ�������е����һ��Ԫ��
						for(itrt_dbpt_2 = pDoc->face1_texture.begin(); itrt_dbpt_2 != pDoc->face1_texture.end(); ++itrt_dbpt_2)
						{
							temp_txt = *itrt_dbpt_2;
						}
						*/
						
						
						int u,v,cur;
						double tri_texture[3][2];
						
						glBegin(GL_TRIANGLES);
						
						for(i = 0; i < pDoc->L1; i++)
						{
							//���������ε��������㼰�䷨����					
							for(j = 0; j < 3; j++)
							{
								for(k = 0; k < 3; k++)
								{
									tri_vertexs[j][k] = pDoc->face1_points[pDoc->triangles1[i][j]][k]/maxv;
									tri_normals[j][k] = pDoc->face1_normals[pDoc->nml_tris[i][j]][k];
									if(k < 2)
									{
										if(pDoc->with_txt)//���obj�ļ�������������Ϣ����Ҳ�����������и����˵�ǰ�������������Ӧ�����
											tri_texture[j][k] = pDoc->temp_txt[pDoc->txt_tris[i][j]][k];
										else
											tri_texture[j][k] = pDoc->temp_txt[pDoc->triangles1[i][j]][k];//���붥������ϸ��Ӧ��
										while(tri_texture[j][k] < 0)
											tri_texture[j][k] += 1;
										while(tri_texture[j][k] > 1)
											tri_texture[j][k] -= 1;
									}
								}
							}
							
							if(m_smooth)//smooth style
							{
								//���������ε��������㼰�䷨����
								for(j = 0; j < 3; j++)
								{
									u = (tri_texture[j][0]*(biWidth-1) - (int)(tri_texture[j][0]*(biWidth-1)))<((int)(tri_texture[j][0]*(biWidth-1))+1 - tri_texture[j][0]*(biWidth-1))?(int)(tri_texture[j][0]*(biWidth-1)):(int)(tri_texture[j][0]*(biWidth-1))+1;
									v = (tri_texture[j][1]*(biHeight-1) - (int)(tri_texture[j][1]*(biHeight-1)))<((int)(tri_texture[j][1]*(biHeight-1))+1 - tri_texture[j][1]*(biHeight-1))?(int)(tri_texture[j][1]*(biHeight-1)):(int)(tri_texture[j][1]*(biHeight-1))+1;
									if(u == biWidth)
										u -= 1;
									if(v == biHeight)
										v -= 1;
									cur = v*biAlign+3*u;
									glColor3f((double)lpData[cur + 2]/256, (double)lpData[cur + 1]/256, (double)lpData[cur]/256);
									glNormal3f(tri_normals[j][0], tri_normals[j][1], tri_normals[j][2]);						
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}
							else//flat style
							{
								//���������㷨������ƽ������ʾ��Ƭ�ķ�����
								glNormal3f((tri_normals[0][0]+tri_normals[1][0]+tri_normals[2][0])/3, 
									(tri_normals[0][1]+tri_normals[1][1]+tri_normals[2][1])/3, 
									(tri_normals[0][2]+tri_normals[1][2]+tri_normals[2][2])/3);
								for(j = 0; j < 3; j++)
								{
									u = (tri_texture[j][0]*(biWidth-1) - (int)(tri_texture[j][0]*(biWidth-1)))<((int)(tri_texture[j][0]*(biWidth-1))+1 - tri_texture[j][0]*(biWidth-1))?(int)(tri_texture[j][0]*(biWidth-1)):(int)(tri_texture[j][0]*(biWidth-1))+1;
									v = (tri_texture[j][1]*(biHeight-1) - (int)(tri_texture[j][1]*(biHeight-1)))<((int)(tri_texture[j][1]*(biHeight-1))+1 - tri_texture[j][1]*(biHeight-1))?(int)(tri_texture[j][1]*(biHeight-1)):(int)(tri_texture[j][1]*(biHeight-1))+1;
									if(u == biWidth)
										u -= 1;
									if(v == biHeight)
										v -= 1;
									cur = v*biAlign+3*u;
									glColor3f((double)lpData[cur + 2]/256, (double)lpData[cur + 1]/256, (double)lpData[cur]/256);
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}					
						}	
						
						glEnd();
					}
					else//ʹ�û�ϵ�����ͼ
					{	
						glBegin(GL_TRIANGLES);						
									
						for(i = 0; i < pDoc->L1; i++)
						{
							for(j = 0; j < 3; j++)
							{
								for(k = 0; k < 3; k++)
								{
									tri_vertexs[j][k] = pDoc->face1_points[pDoc->triangles1[i][j]][k]/maxv;
									tri_normals[j][k] = pDoc->face1_normals[pDoc->nml_tris[i][j]][k];								
								}
							}						
							
							if(m_smooth)//smooth style
							{
								//���������ε��������㼰�䷨����
								for(j = 0; j < 3; j++)
								{
									glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][2]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][0]/256);
									glNormal3f(tri_normals[j][0], tri_normals[j][1], tri_normals[j][2]);						
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}
							else//flat style
							{
								//���������㷨������ƽ������ʾ��Ƭ�ķ�����
								glNormal3f((tri_normals[0][0]+tri_normals[1][0]+tri_normals[2][0])/3, 
										(tri_normals[0][1]+tri_normals[1][1]+tri_normals[2][1])/3, 
										(tri_normals[0][2]+tri_normals[1][2]+tri_normals[2][2])/3);
								for(j = 0; j < 3; j++)
								{
									glColor3f(pDoc->texture_map[pDoc->triangles1[i][j]][2]/256, pDoc->texture_map[pDoc->triangles1[i][j]][1]/256, pDoc->texture_map[pDoc->triangles1[i][j]][0]/256);
									glVertex3f(tri_vertexs[j][0], tri_vertexs[j][1], tri_vertexs[j][2]);						
								}
							}	
						}	
						glEnd();
					}
				}
			}
		}

		if(pDoc->m_style == 1)//ֻ��ʾ����
		{
			glColor3f(m_Face1Red, m_Face1Green, m_Face1Blue);	
			for(i = 0; i < pDoc->M; i++)
			{
				//if(pDoc->mapped && pDoc->vtx_map[pDoc->iter_index-1][i][0] != -1)
				{
					x=(pDoc->face1_points[i][0])/maxv;
					y=(pDoc->face1_points[i][1])/maxv;
					z=(pDoc->face1_points[i][2])/maxv;
					glBegin(GL_POINTS);
					//glNormal3f(pDoc->face1_normals[i][0], pDoc->face1_normals[i][0], pDoc->face1_normals[i][0]);
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//if(pDoc->data_cmpt)
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//	glColor3f(pDoc->vtx_gray[i]/256, pDoc->vtx_gray[i]/256, pDoc->vtx_gray[i]/256);																			
					glVertex3f(x, y, z);				
					glEnd();
				}				
			}
		}
		
		//��ʾ������
		if(m_axis)
		{
			//x��Ϊ��ɫ
			glColor3f(0, 1, 1);
			glBegin(GL_LINES);		
			x = 1.5;
			y = 0;
			z = 0;
			glVertex3f(x, y, z);
			
			x = 0;
			y = 0;
			z = 0;
			glVertex3f(x, y, z);
			glEnd();
			
			//y��ΪƷ��ɫ
			glColor3f(1, 0, 1);		
			glBegin(GL_LINES);		
			x = 0;
			y = 1.5;
			z = 0;
			glVertex3f(x, y, z);
			
			x = 0;
			y = 0;
			z = 0;
			glVertex3f(x, y, z);
			glEnd();
			
			//z��Ϊ��ɫ
			glColor3f(1, 1, 0);
			glBegin(GL_LINES);		
			x = 0;
			y = 0;
			z = 1.5;
			glVertex3f(x, y, z);
			
			x = 0;
			y = 0;
			z = 0;
			glVertex3f(x, y, z);
			glEnd();
		}

		//��ʾ���궨��
		if(pDoc->mdl_pt_num > 0)
		{			
			if(pDoc->which_loc == 1)
			{
				glColor3f(1.0, 0.0, 0.0);//�ú�ɫ����ʾ				
				
				if(!pDoc->mdl_pts_opted)//����δ�Ż�֮ǰ��ֻ��ʾģ���ϵĳ�ʼ�궨��
				{
					list_dbpt::iterator itrt;
					for(itrt = pDoc->mdl_pts.begin(); itrt != pDoc->mdl_pts.end(); ++itrt)
					{
						glPushMatrix();
						glTranslatef((*itrt)[0], (*itrt)[1], (*itrt)[2]);
						auxSolidSphere(0.0125);
						glPopMatrix();
					}
				}
				else//�����Ż�֮�󣬶Ա�ģ���ϱ궨���Ż�ǰ���λ��
				{
					glColor3f(1.0, 0.0, 0.0);//�ú�ɫ����ʾ								
					for(i = 0; i < pDoc->mdl_pt_num; i++)
					{
						glPushMatrix();
						glTranslatef(pDoc->mdl_pts_array[i][0], pDoc->mdl_pts_array[i][1], pDoc->mdl_pts_array[i][2]);
						auxSolidSphere(0.0125);
						glPopMatrix();
					}

					glColor3f(0.0, 1.0, 0.0);//����ɫ����ʾ	
					for(i = 0; i < pDoc->mdl_pt_num; i++)
					{
						glPushMatrix();
						glTranslatef(pDoc->opt_mdl_pts[i][0], pDoc->opt_mdl_pts[i][1], pDoc->opt_mdl_pts[i][2]);
						auxSolidSphere(0.0125);
						glPopMatrix();
					}
					if(pDoc->iter_begin)//�Ѿ���ʼ������
					{
						int num;
						glColor3f(1.0, 1.0, 0.0);
						for(i = 0; i < pDoc->mdl_pt_num; i++)
						{
							for(num = 0; num < pDoc->temp_iter_num; num++)
							{
								glPushMatrix();
								glTranslatef(pDoc->mdl_pts_trace[i][num][0], pDoc->mdl_pts_trace[i][num][1], pDoc->mdl_pts_trace[i][num][2]);
								auxSolidSphere(0.0015625);
								glPopMatrix();
							}
						}
					}
				}				
			}
			else
			{
				for(i = 0; i < pDoc->mdl_pt_num; i++)
				{
					glPushMatrix();
					glTranslatef(pDoc->mdl_temp_pt[i][0], pDoc->mdl_temp_pt[i][1], pDoc->mdl_temp_pt[i][2]);
					auxSolidSphere(0.0125);
					glPopMatrix();
				}
			}
		}

		//�ڼ����ʵ��ӳ���֮����ʾ�궨����ʵ�ʶ�Ӧ�㣬�Թ۲������
		list_dbpt::iterator itrt;
		glColor3f(1.0, 1.0, 0.0);//�û�ɫ����ʾ
		for(itrt = pDoc->act_mdl_pts.begin(), i = 0; itrt != pDoc->act_mdl_pts.end(); ++itrt, i++)
		{
			glPushMatrix();
			glTranslatef((*itrt)[0]/pDoc->Max_V, (*itrt)[1]/pDoc->Max_V, (*itrt)[2]/pDoc->Max_V);
			auxSolidSphere(0.0125);
			glPopMatrix();
		}
		if(i != 0)//��ȷ�Ѿ�������ʵ��ӳ���
		{
			glColor3f(1.0, 0.0, 1.0);//��Ʒ������ʾ
			for(itrt = pDoc->mdl_pts.begin(); itrt != pDoc->mdl_pts.end(); ++itrt)
			{
				glPushMatrix();
				glTranslatef((*itrt)[0]/pDoc->Max_V, (*itrt)[1]/pDoc->Max_V, (*itrt)[2]/pDoc->Max_V);
				auxSolidSphere(0.0125);
				glPopMatrix();
			}
		}
		glFlush();		
	}
}

/*
void COGLEventView::OnModelingFract() 
{
	fra_mount->Generate_mount(0,0,128,128,3);
	InvalidateRect(NULL,false);
}
*/

void COGLEventView::OnFrontView() 
{
	//default view
	ViewMap *dlg=new ViewMap(this);
	dlg->Create(IDD_VIEW_MAPPING);
	dlg->ShowWindow(SW_SHOW);	
}

void COGLEventView::OnRightView() 
{
	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->textured = false;
	pDoc->which_view = 2;//right view	

	ViewMap *dlg=new ViewMap(this);
	dlg->Create(IDD_VIEW_MAPPING);
	dlg->ShowWindow(SW_SHOW);

	int i, j;
	CVisDMatrix rot_y(3, 3);
	CVisDMatrix temp_pt(3, 1);
	CVisDMatrix temp_nml(3, 1);
	
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			rot_y[i][j] = 0;
		}
	}		
	
	rot_y[1][1] = 1;
	rot_y[0][0] = rot_y[2][2] = cos(90*PI/180);
	rot_y[0][2] = sin(90*PI/180);
	rot_y[2][0] = -sin(90*PI/180);
	
	//��任
	for(i = 0; i < pDoc->M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			temp_pt[j][0] = pDoc->face1_points[i][j];
		}
		temp_pt = rot_y*temp_pt;
		for(j = 0; j < 3; j++)
		{
			pDoc->face1_points[i][j] = temp_pt[j][0];
		}
	}
	//�������任
	for(i = 0; i <pDoc->M1; i++)
	{
		for(j = 0; j < 3; j++)
		{
			temp_nml[j][0] = pDoc->face1_normals[i][j];
		}
		temp_nml = rot_y*temp_nml;
		for(j = 0; j < 3; j++)
		{
			pDoc->face1_normals[i][j] = temp_nml[j][0];
		}
	}

	pDoc->UpdateAllViews(NULL);
}

void COGLEventView::OnLeftView() 
{
	COGLEventDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->textured = false;
	pDoc->which_view = 3;//left view	

	ViewMap *dlg=new ViewMap(this);
	dlg->Create(IDD_VIEW_MAPPING);
	dlg->ShowWindow(SW_SHOW);

	int i, j;
	CVisDMatrix rot_y(3, 3);
	CVisDMatrix temp_pt(3, 1);
	CVisDMatrix temp_nml(3, 1);
	
	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			rot_y[i][j] = 0;
		}
	}		
	
	rot_y[1][1] = 1;
	rot_y[0][0] = rot_y[2][2] = cos(-180*PI/180);
	rot_y[0][2] = sin(-180*PI/180);
	rot_y[2][0] = -sin(-180*PI/180);
	
	//��任
	for(i = 0; i < pDoc->M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			temp_pt[j][0] = pDoc->face1_points[i][j];
		}
		temp_pt = rot_y*temp_pt;
		for(j = 0; j < 3; j++)
		{
			pDoc->face1_points[i][j] = temp_pt[j][0];
		}
	}
	//�������任
	for(i = 0; i <pDoc->M1; i++)
	{
		for(j = 0; j < 3; j++)
		{
			temp_nml[j][0] = pDoc->face1_normals[i][j];
		}
		temp_nml = rot_y*temp_nml;
		for(j = 0; j < 3; j++)
		{
			pDoc->face1_normals[i][j] = temp_nml[j][0];
		}
	}

	pDoc->UpdateAllViews(NULL);
	
}
