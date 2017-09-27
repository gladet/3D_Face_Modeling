// OGLEventDoc.cpp : implementation of the COGLEventDoc class
//

#include "stdafx.h"
#include "OGLEvent.h"

#include "OGLEventDoc.h"
#include "MainFrm.h"
#include "OGLEventView.h"
#include "FormCommandView1.h"
#include "Texture.h"
#include "Txt_map.h"
#include "Data_input.h"
#include "Area_cmpt.h"
#include "Vlmn_cmpt.h"

#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COGLEventDoc

IMPLEMENT_DYNCREATE(COGLEventDoc, CDocument)

BEGIN_MESSAGE_MAP(COGLEventDoc, CDocument)
	//{{AFX_MSG_MAP(COGLEventDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_BN_CLICKED(IDC_READ_MODEL, OnReadModel)
	ON_BN_CLICKED(IDC_PT_LOC_1, OnPtLoc1)
	ON_BN_CLICKED(IDC_PT_LOC_2, OnPtLoc2)
	ON_BN_CLICKED(IDC_PROJ_MAP, OnProjMap)
	ON_BN_CLICKED(IDC_TXT_BLEND, OnTxtBlend)
	ON_BN_CLICKED(IDC_READ_IMAGE, OnReadImage)
	ON_BN_CLICKED(IDC_POSE_ADJ, OnPoseAdj)
	ON_COMMAND(ID_RIGHT_VIEW, OnRightView)
	ON_COMMAND(ID_LEFT_VIEW, OnLeftView)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_SAVE_DATA, OnSaveData)
	ON_COMMAND(ID_DATA_ALALYSIS, OnDataAlalysis)
	ON_BN_CLICKED(IDC_PRMT_OPT, OnPrmtOpt)
	ON_BN_CLICKED(IDC_INVISIBLE, OnInvisible)
	ON_BN_CLICKED(IDC_ITERATIVE, OnIterative)
	ON_BN_CLICKED(IDC_NEXTVIEW, OnNextview)
	ON_BN_CLICKED(IDC_SAVEIMAGE, OnSaveimage)
	ON_COMMAND(ID_READ_BKGD, OnReadBkgd)
	ON_COMMAND(ID_READ_FGD, OnReadFgd)
	ON_COMMAND(ID_IMG_DIFF, OnImgDiff)
	ON_BN_CLICKED(IDC_MAP, OnMap)
	ON_BN_CLICKED(IDC_PXL_COMP, OnPxlComp)
	ON_COMMAND(ID_AREA_CMPT, OnAreaCmpt)
	ON_COMMAND(ID_VLMN_CMPT, OnVlmnCmpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COGLEventDoc construction/destruction
COGLEventDoc::COGLEventDoc()
{
	m_read = false;

	m_style = 2;

	which_face = 1;

	textured = false;

	if_rot = TRUE;	

	obj_fp_lc = img_fp_lc = true;

	img_pt_num = 0;//初始化时就会调用OnLButtonUp函数

	mdl_pt_num = 0;//?	
	
	img_loaded = false;

	N_E.Resize(3);
	N_M.Resize(3);
	N_S.Resize(3);
	N_F.Resize(3);

	transformed = false;

	pose_adj = false;
	x_rot = y_rot = z_rot = 0;

	//which_view = 1;	
	num_view = 0;
	which_loc = 1;//初始化为位姿调整前的点标定

	width = height = 0;

	cylindrical_map = false;//标识是否已经生成了纹理图
	txt_mapped = false;//标识是否显示纹理图
	mesh_projed = false;//标志是否显示网格投影

	with_txt = true;//默认obj文件中已附带纹理信息，也便于以后扩充功能
	txt_type = false;//针对附带颜色信息的obj文件，在顶点坐标后读出其相应的颜色值//true;//默认obj文件中附带纹理信息是以纹理坐标表示的

	m_pBitmap = NULL;
	m_pBkgd = NULL;
	m_pTransfered = NULL;
	texture_map = NULL;
	txt_map = NULL;
	txt_tris = NULL;
	
	rot_matrix.Resize(3, 3);//用于记录整个位姿调整过程中所有旋转操作的累积
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(i == j)	rot_matrix[i][j] = 1;
			else rot_matrix[i][j] = 0;
		}
	}

	num_view = 0;

	temp_img_pt_num = 0;

	mdl_pts_opted = false;

	optimized = false;

	iter_index = 1;

	K1 = 0;

	temp_iter_num = iter_num = 0;

	iter_begin = false;
	iter_over = false;

	mapped = false;

	data_cmpt = false;

	map_pxl_num_sum = 0;//初始化

	//初始化为空指针
	view_data = NULL;//存储各视图图像数据的指针，使用数组更易于操作？
	biHeight = NULL;//存储各视图图像的高度
	biWidth = NULL;//存储各视图图像的宽度
	biAlign = NULL;

	//vtx_clr = new double[M];//记录网格顶点对应的灰度信息

	/*
	//此时M尚未初始化！
	deno_of_weight = new double[M];//记录网格顶点对应于各视图的权值的公共分母
	weight = new double*[num_view];//记录网格顶点对应于各视图的权值
	for(int k = 0; k < num_view; k++)
	{
		weight[k] = new double[M];
	}
	vtx_map_gray = new double*[num_view];//记录网格顶点在各视图中对应像素处的灰度
	for(k = 0; k < num_view; k++)
	{
		vtx_map_gray[k] = new double[M];
	}
	derive_x = new double*[num_view];//记录网格顶点在各视图中对应像素处沿X方向的偏导
	for(k = 0; k < num_view; k++)
	{
		derive_x[k] = new double[M];
	}
	derive_y = new double*[num_view];//记录网格顶点在各视图中对应像素处沿Y方向的偏导
	for(k = 0; k < num_view; k++)
	{
		derive_y[k] = new double[M];
	}
	vtx_gray = new double[M];//记录网格顶点对应的灰度信息
	vis_img = new bool*[num_view];//记录与当前所遍历像素对应的网格顶点在哪些视图中可见，切记每次便利都要重新初始化！
	for(k = 0; k < M; k++)
	{
		vis_img[k] = new bool[M];
	}
	*/
}

COGLEventDoc::~COGLEventDoc()
{
	int i;
	//首先清空原先存储在数组中的数据，释放内存空间
	//清空顶点及法向量信息
	for(i = 0; i < M; i++)
	{
		delete []face1_points[i];
		//delete []face1_normals[i];
	}
	delete []face1_points;
	
	for(i = 0; i < M1; i++)
	{
		//delete []face1_points[i];
		delete []face1_normals[i];
	}
	delete []face1_normals;
	
	//清空纹理坐标信息
	if(texture_map != NULL)
	{
		for(i = 0; i < K1; i++)
		{
			delete []texture_map[i];
		}
		delete []texture_map;
	}
	
	//清空三角形信息（包括顶点和法向量）
	for(i = 0; i < L1; i++)
	{
		delete []triangles1[i];
		delete []nml_tris[i];			
	}
	delete []triangles1;
	delete []nml_tris;
	
	if(txt_tris != NULL)//with_txt)//读入obj文件中若附带了纹理坐标信息，对应三角形序号信息也要清空
	{
		for(i = 0; i < L1; i++)
		{
			delete []txt_tris[i];
		}
		delete []txt_tris;
	}
	
	list_dbpt_2::iterator dbpt_itrt_2;
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator intpt_itrt;
	list_charpt::iterator itrt_charpt;	

	for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
	for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	img_pts.erase(img_pts.begin(), img_pts.end());
	for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
	for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
	
	if(txt_map != NULL)//txt_map是显示当前视图对应的生成纹理，而texture_map是显示融合的或原obj文件中附带的纹理图
	{
		for(i = 0; i < K1; i++)//若未执行Txt_Mapping，K1 = 0？也不一定
		{
			delete []txt_map[i];
		}
		delete []txt_map;
	}
	
	//若已经映射了若干视图，要逐一将其对应的纹理坐标清空
	for(dbpt_itrt_2 = face1_texture.begin(); dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2)
	{
		if((*dbpt_itrt_2) != NULL)//其实只要看一看链表的构造过程，就知道这种判断是无意义的
		{
			for(i = 0; i < K1; i++)
			{
				if((*dbpt_itrt_2)[i] != NULL)
					delete [](*dbpt_itrt_2)[i];//每个指针指向一个二维纹理坐标
			}
			delete [](*dbpt_itrt_2);
		}
	}
	//清空链表
	face1_texture.erase(face1_texture.begin(), face1_texture.end());
	
	//将各视图对应的可视内积图清空
	for(dbpt_itrt = view_inner.begin(); dbpt_itrt != view_inner.end(); ++dbpt_itrt)
	{
		if((*dbpt_itrt) != NULL)
			delete [](*dbpt_itrt);		
	}
	//清空链表
	view_inner.erase(view_inner.begin(), view_inner.end());
	
	//free(m_pTransfered);
	if(m_pTransfered != NULL)//而m_pTransfered的内存分配是在OnTxtBlend函数中完成的
		delete []m_pTransfered;//因为是用new方式分配内存的
	
	for(itrt_charpt = view_pt.begin(); itrt_charpt != view_pt.end(); ++itrt_charpt)
	{
		if((*itrt_charpt) != NULL)
		{
			if(*itrt_charpt != m_pBitmap)//与m_pBitmap共同指向的内存空间交由m_pBitmap自身来释放
			{
				free(*itrt_charpt);
				(*itrt_charpt) = NULL;//将视图指针赋为空
			}
		}
	}
	//直接清空链表即可
	view_pt.erase(view_pt.begin(), view_pt.end());
	if(m_pBitmap != NULL)
	{
		free(m_pBitmap);//因为m_pBitmap都是通过调用ReadBitmap中的malloc来赋值的
		//因为并没有额外的内存分配，所以不用释放
		m_pBitmap = NULL;
	}	
}

//********************************************
// GetRenderView
//********************************************
CView *COGLEventDoc::GetOGLEventView() 
{
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	CView *pView = (CView *)pFrame->m_wndSplitter2.GetPane(0,0);
	return pView;
}

CView *COGLEventDoc::GetTextureView() 
{
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	CView *pView = (CView *)pFrame->m_wndSplitter2.GetPane(0,1);
	return pView;
}

CView *COGLEventDoc::GetFormCommandView() 
{
	COGLEventApp *pApp = (COGLEventApp *)AfxGetApp();
	CMainFrame *pFrame = (CMainFrame *)pApp->m_pMainWnd;
	CView *pView = (CView *)pFrame->m_wndSplitter.GetPane(0,0);
	return pView;
}

BOOL COGLEventDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COGLEventDoc serialization

void COGLEventDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COGLEventDoc diagnostics

#ifdef _DEBUG
void COGLEventDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COGLEventDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COGLEventDoc commands

void COGLEventDoc::OnFileOpen() 
{

	
}

void COGLEventDoc::Tran_To_Center()
{
	CVisDMatrix cen_point1(3, 1);
	int i, j;
	
	//初始化
	for(j = 0; j < 3; j++)
	{
		cen_point1[j][0] = 0;
	}	
	
	//求重心
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			//求和. cgi[]是列向量
			cen_point1[j][0] = cen_point1[j][0] + face1_points[i][j];
		}
	}
	
	//取平均值
	cen_point1 /= (double)M;	
	
	//平移质心至原点处
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			face1_points[i][j] -= cen_point1[j][0];			
		}
	}
}

BOOL COGLEventDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	m_strFileName = lpszPathName;
	if (ReadBitmap(lpszPathName, m_pBitmap))
		UpdateAllViews(NULL);
	
	img_loaded = true;
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();	
	pView->Invalidate(FALSE);

	return TRUE;
}

BOOL COGLEventDoc::ReadBitmap(LPCTSTR lpszPathName, unsigned char *& buffer)
{
	FILE *file;
	file = fopen( lpszPathName, "rb");
	if(!file)
		return FALSE;
	fpos_t posend, posbegin;

	fseek(file, 0, SEEK_END);
	fgetpos(file, &posend);
	fseek(file, 0, SEEK_SET);
	fgetpos(file,&posbegin);
	size_t filesize = (size_t)(posend - posbegin);

	current_size = filesize;	
	
	if( buffer == NULL )
		buffer = (unsigned char*)malloc( filesize);
	//如果释放掉了内存，则无法供纹理融合适用
	else
	{
		//free(buffer);//好像是不应该释放的?
		buffer = NULL;
		buffer = (unsigned char*)malloc( filesize);
	}
	if( buffer == NULL )	
	{
		AfxMessageBox(IDM_MEMORY_INSUFFICIENT, MB_OK, 0);
		return FALSE;
	}
	memset(buffer, 0, filesize);
	size_t sizeread = fread(buffer, sizeof(unsigned char), filesize, file);
	if(sizeread != filesize)
	{
		AfxMessageBox(IDM_READFILE_ERROR, MB_OK, 0);
		free(buffer);
		buffer = NULL;
		fclose(file);
		return FALSE;
	}
	if( buffer[0] != 'B'||buffer[1] != 'M')
	{
		AfxMessageBox(IDM_FILEFORMAT_ERROR, MB_OK, 0);
		free(buffer);
		buffer = NULL;
		fclose(file);
		return FALSE;
	}
	fclose(file);
	return TRUE;
}

bool COGLEventDoc::ReadObj_Extd(double **& data_points, double **& data_normals, double **& data_texture, int **& triangles, int **& nml_tris, int **& txt_tris, int& num_points, int& num_normals, int& num_txt, int& num_tris, bool& with_txt)
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("obj文件(*.obj)|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}
	else 
	{
		return false;
	}

	FILE *fp, *pre_fp;
	int i, j, temp_index;
	char c;
	double temp;
	char str[1000];//不知道最大长度可能达到多大，保险起见。
	list_dbpt list_points, list_normals, list_txt;
	list_intpt list_tris, list_nml_tris, list_txt_tris;
	//上面链表最后有清空吗？
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator itrt_intpt;	
	double* temp_dbpt;
	int* temp_intpt, *temp_intpt_1, *temp_intpt_2;
	//正确初始化
	int sum_points = 0;
	int sum_normals = 0;
	int sum_txt = 0;
	int sum_tris = 0;
	int end_of_file = 0;//判断文件是否结束
	int tris_begin = 0;
	//作为一个参数输入
	//int with_txt = 1;//默认附带纹理信息

	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return false;
	}
	fseek(fp, 0L, SEEK_SET);
	while(fscanf(fp, "%s", &str) != -1)
	{
		//读入顶点信息
		while(str[0] == 'v' && str[1] == 0 && end_of_file != -1)
		{
			//if(sum_points == 11994)
				//sum_points = 11994;
			sum_points++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);//读出一个浮点数
				temp_dbpt[j] = temp;
			}
			list_points.insert(list_points.end(),temp_dbpt);

			//针对附带颜色信息的obj文件，在顶点坐标后读出其相应的颜色值
			sum_txt++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);//读出一个浮点数
				temp_dbpt[j] = temp;
			}
			list_txt.insert(list_txt.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);//这是读回车吗？其实是读入下一个字符串，可以用于判断下面是那种类型的数据
			if(sum_points == 23195)
			{
				sum_points = sum_points;
			}				
		}
		
		//读入顶点法向量信息
		while(str[0] == 'v' && str[1] == 'n' && end_of_file != -1)
		{
			sum_normals++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_normals.insert(list_normals.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);
		}
		
		while(str[0] == 'v' && str[1] == 't' && end_of_file != -1)
		{
			txt_type = true;
			sum_txt++;
			temp_dbpt = new double[2];//是不是应当为2？
			for(j = 0; j < 2; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_txt.insert(list_txt.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);
		}

		if(str[0] == 'v' && str[1] == 'c' && end_of_file != -1)
		{
			txt_type = false;
			sum_txt++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_txt.insert(list_txt.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);
		}
		while(str[0] == 'v' && str[1] == 'c' && end_of_file != -1)
		{
			sum_txt++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_txt.insert(list_txt.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);
		}

		while(str[0] == 'f' && str[1] == 0 && end_of_file != -1)
		{
			sum_tris++;
			temp_intpt = new int[3];
			if(tris_begin == 0 || with_txt == 1)//with_txt记录是否附带纹理坐标信息，默认是附带纹理信息的
				temp_intpt_1 = new int[3];//是用于记录当前三角形上的各顶点纹理的序号
			temp_intpt_2 = new int[3];//是用于记录当前三角形上的各顶点法向量的序号
			
			if(tris_begin == 0)
			{
				tris_begin = 1;
				fscanf(fp, "%d", &temp_index);
				temp_intpt[0] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
				fscanf(fp, "%c", &c);//c = '/'
				pre_fp = fp;
				fscanf(fp, "%c", &c);//未知其结构
				if(c == '/')//如果是两个斜杠
				{
					//with_txt = false;//没有纹理坐标信息
					fscanf(fp, "%d", &temp_index);
					temp_intpt_2[0] = temp_index - 1;
					for(j = 1; j < 3; j++)
					{					
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);	
						temp_intpt_2[j] = temp_index - 1;//记录当前三角面片各顶点法向量在数组中的标号
					}
				}
				else
				{
					fp = pre_fp;//退回到上一个读入位置
					fscanf(fp, "%d", &temp_index);
					temp_intpt_1[0] = temp_index - 1;//记录当前三角面片各顶点纹理在数组中的标号				
					fscanf(fp, "%c", &c);//c = '/'
					fscanf(fp, "%d", &temp_index);
					temp_intpt_2[0] = temp_index - 1;//记录当前三角面片各顶点法线在数组中的标号
					for(j = 1; j < 3; j++)//读入剩余两个顶点及其纹理和法向量的信息
					{
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_1[j] = temp_index - 1;//记录当前三角面片各顶点纹理在数组中的标号				
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_2[j] = temp_index - 1;//记录当前三角面片各顶点法线在数组中的标号	
					}
					list_txt_tris.insert(list_txt_tris.end(),temp_intpt_1);//将纹理信息也插入链表					
				}
				list_tris.insert(list_tris.end(),temp_intpt);	
				list_nml_tris.insert(list_nml_tris.end(),temp_intpt_2);
				end_of_file = fscanf(fp, "%s", &str);//文件结束难道只可能在三角形的信息之后吗？
			}
			else//可能间断了三角形的信息，接着再读入时就不必作上面的判断了
			{
				if(with_txt)//这里是在01_15改动的// == 0)
				{
					for(j = 0; j < 3; j++)
					{
						
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);	
						temp_intpt_2[j] = temp_index - 1;//记录当前三角面片各顶点法线在数组中的标号	
					}
					list_tris.insert(list_tris.end(),temp_intpt);
					list_nml_tris.insert(list_nml_tris.end(),temp_intpt_2);					
					end_of_file = fscanf(fp, "%s", &str);
				}
				else//这是针对什么情况呢？
				{
					for(j = 0; j < 3; j++)
					{
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_1[j] = temp_index - 1;//记录当前三角面片各顶点纹理在数组中的标号				
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_2[j] = temp_index - 1;//记录当前三角面片各顶点法线在数组中的标号	
					}
					list_tris.insert(list_tris.end(),temp_intpt);
					list_txt_tris.insert(list_txt_tris.end(),temp_intpt_1);
					list_nml_tris.insert(list_nml_tris.end(),temp_intpt_2);
					end_of_file = fscanf(fp, "%s", &str);
				}
			}
			if(sum_tris == 44978)
			{
				sum_tris = sum_tris;
			}
		}		
	}

	//将读入链表的顶点信息存入数组
	data_points = new double*[sum_points];
	for(dbpt_itrt = list_points.begin(), i = 0; dbpt_itrt != list_points.end(); ++dbpt_itrt, i++)
	{
		data_points[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			data_points[i][j] = (*dbpt_itrt)[j];
		}
	}
	num_points = i;

	//将读入链表的顶点法向量信息存入数组
	data_normals = new double*[sum_normals];
	for(dbpt_itrt = list_normals.begin(), i = 0; dbpt_itrt != list_normals.end(); ++dbpt_itrt, i++)
	{
		data_normals[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			data_normals[i][j] = (*dbpt_itrt)[j];
		}
	}
	num_normals = i;

	//将读入链表的顶点纹理坐标信息存入数组
	if(with_txt)
	{
		data_texture = new double*[sum_txt];
		if(txt_type)//的确是纹理坐标，即对应"vt"
		{
			for(dbpt_itrt = list_txt.begin(), i = 0; dbpt_itrt != list_txt.end(); ++dbpt_itrt, i++)
			{
				data_texture[i] = new double[2];
				for(j = 0; j < 2; j++)
				{
					data_texture[i][j] = (*dbpt_itrt)[j];
				}
			}
		}
		else//是颜色值，即对应"vc"
		{
			for(dbpt_itrt = list_txt.begin(), i = 0; dbpt_itrt != list_txt.end(); ++dbpt_itrt, i++)
			{
				data_texture[i] = new double[3];
				for(j = 0; j < 3; j++)
				{
					data_texture[i][j] = (*dbpt_itrt)[j];
				}
			}
		}
		num_txt = i;
	}

	//将读入链表的三角形信息存入数组
	triangles = new int*[sum_tris];
	for(itrt_intpt = list_tris.begin(), i = 0; itrt_intpt != list_tris.end(); ++itrt_intpt, i++)
	{
		triangles[i] = new int[3];
		for(j = 0; j < 3; j++)
		{
			triangles[i][j] = (*itrt_intpt)[j];
		}
	}
	if(with_txt)
	{
		/*
		txt_tris = new int*[sum_tris];//存储三角形三个顶点对应的纹理坐标的序号，是不是必须有纹理信息才执行这一操作呢？	
		for(itrt_intpt = list_txt_tris.begin(), i = 0; itrt_intpt != list_txt_tris.end(); ++itrt_intpt, i++)
		{
			txt_tris[i] = new int[3];
			for(j = 0; j < 3; j++)
			{
				txt_tris[i][j] = (*itrt_intpt)[j];
			}
		}
		*/
	}	
	nml_tris = new int*[sum_tris];//存储三角形三个顶点对应的法向量的序号，其实一般是与顶点序号相同的
	for(itrt_intpt = list_nml_tris.begin(), i = 0; itrt_intpt != list_nml_tris.end(); ++itrt_intpt, i++)
	{
		nml_tris[i] = new int[3];
		for(j = 0; j < 3; j++)
		{
			nml_tris[i][j] = (*itrt_intpt)[j];
		}
	}
	num_tris = i;

	fclose(fp);	

	delete []temp_dbpt;
	delete []temp_intpt;
	delete []temp_intpt_1;
	delete []temp_intpt_2;
	//清空链表
	list_points.erase(list_points.begin(), list_points.end());
	list_normals.erase(list_normals.begin(), list_normals.end());
	list_txt.erase(list_txt.begin(), list_txt.end());
	list_tris.erase(list_tris.begin(), list_tris.end());
	list_txt_tris.erase(list_txt_tris.begin(), list_txt_tris.end());
	list_nml_tris.erase(list_nml_tris.begin(), list_nml_tris.end());

	return true;//的确读入了模型数据信息
}

/*void COGLEventDoc::ReadObj(double **& data_points, double **& data_normals, int **& triangles, int& num_points, int& num_tris)
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("obj文件(*.obj)|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}

	FILE *fp;
	int i, j, temp_tri_index, temp_index;
	char c;
	double temp;
	char str[81];
	
	//将cstrFileName中的点及对应法向量读入到内存数组当中
	fp = fopen(cstrFileName, "r+");
	if( fp == NULL )
	{
		#ifdef _DEBUG
					AfxMessageBox("The file can not be opened" , MB_OK, 0);					
		#endif
		return;
	}
	fseek(fp, 0L, SEEK_SET);

	fscanf(fp, "%s", &str);//s = "#"
	fscanf(fp, "%s", &str);//s = "File"
	fscanf(fp, "%s", &str);//s = "generated"
	fscanf(fp, "%s", &str);//s = "by"
	fscanf(fp, "%s", &str);//s = "InnovMetric"
	fscanf(fp, "%s", &str);//s = "Software"
	fscanf(fp, "%s", &str);//s = "Inc."
	fscanf(fp, "%s", &str);//s = "#"
	fscanf(fp, "%ld", &num_points);
	fscanf(fp, "%s", &str);//s = "vertices,"
	fscanf(fp, "%ld", &num_tris);
	fscanf(fp, "%s", &str);//s = "triangles"
	fscanf(fp, "%s", &str);//s = "mtllib"
	fscanf(fp, "%s", &str);//s = "wangpeng_smooth.mtl"
	fscanf(fp, "%s", &str);//s = "g"
	fscanf(fp, "%s", &str);//s = "default"
	fscanf(fp, "%c", &c);//c = 回车

	data_points = new double*[num_points];
	data_normals = new double*[num_points];
	triangles = new int*[num_tris];	
	
	i = 0;

	while(i < num_points)
	{
		data_points[i] = new double[3];

		fscanf(fp, "%c", &c);//c = 'v'
		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%lf", &temp);
			data_points[i][j] = temp;
		}
		fscanf(fp, "%c", &c);//c = 回车
		
		i++;
	}

	i = 0;
	while(i < num_points)
	{
		data_normals[i] = new double[3];
		fscanf(fp, "%c", &c);//c = 'v'
		fscanf(fp, "%c", &c);//c = 'n'
		
		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%lf", &temp);
			data_normals[i][j] = temp;
		}
		fscanf(fp, "%c", &c);
		
		i++;
	}

	fscanf(fp, "%s", &str);//s = "g"
	fscanf(fp, "%s", &str);//s = "default"
	fscanf(fp, "%s", &str);//s = "usemtl"
	fscanf(fp, "%s", &str);//s = "material_1"
	fscanf(fp, "%c", &c);//c = 回车
	
	temp_tri_index = 0;
	while(temp_tri_index < num_tris)
	{
		triangles[temp_tri_index] = new int[3];

		fscanf(fp, "%c", &c);//c = 'f'

		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%d", &temp_index);
			triangles[temp_tri_index][j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
			fscanf(fp, "%c", &c);//c = '/'
			fscanf(fp, "%c", &c);//c = '/'
			fscanf(fp, "%d", &temp_index);			
		}

		fscanf(fp, "%c", &c);//c = 回车
		
		temp_tri_index++;
	}

	fclose(fp);	
}
*/

void COGLEventDoc::ReadObj(double **& data_points, double **& data_normals, int **& triangles, int& num_points, int& num_normals, int& num_tris)
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("obj文件(*.obj)|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}

	FILE *fp;
	int i, j, temp_index;
	char c;
	double temp;
	char str[1000];//不知道最大长度可能达到多大，保险起见。
	list_dbpt list_points, list_normals;
	list_intpt list_tris;
	double* temp_dbpt;
	int* temp_intpt;
	int sum_points = 0;
	int sum_normals = 0;
	int sum_tris = 0;
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator itrt_intpt;
	int end_of_file = 0;

	fp = fopen(cstrFileName, "r+");
	fseek(fp, 0L, SEEK_SET);
	while(fscanf(fp, "%s", &str) != -1)
	{
		while(str[0] == 'v' && str[1] == 0 && end_of_file != -1)
		{
			sum_points++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_points.insert(list_points.end(),temp_dbpt);
			//delete []temp_dbpt;
			fscanf(fp, "%s", &str);
		}
		
		while(str[0] == 'v' && str[1] == 'n' && end_of_file != -1)
		{
			sum_normals++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);
				temp_dbpt[j] = temp;
			}
			list_normals.insert(list_normals.end(),temp_dbpt);
			//delete []temp_dbpt;
			fscanf(fp, "%s", &str);
		}
		
		while(str[0] == 'f' && str[1] == 0 && end_of_file != -1)
		{
			sum_tris++;
			temp_intpt = new int[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%d", &temp_index);
				temp_intpt[j] = temp_index - 1;//记录当前三角面片各顶点在数组中的标号
				fscanf(fp, "%c", &c);//c = '/'
				fscanf(fp, "%c", &c);//c = '/'
				fscanf(fp, "%d", &temp_index);					
			}
			list_tris.insert(list_tris.end(),temp_intpt);		
			//delete []temp_intpt;
			end_of_file = fscanf(fp, "%s", &str);
		}
		
	}


	data_points = new double*[sum_points];
	for(dbpt_itrt = list_points.begin(), i = 0; dbpt_itrt != list_points.end(); ++dbpt_itrt, i++)
	{
		data_points[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			data_points[i][j] = (*dbpt_itrt)[j];
		}
	}
	num_points = i;

	data_normals = new double*[sum_normals];
	for(dbpt_itrt = list_normals.begin(), i = 0; dbpt_itrt != list_normals.end(); ++dbpt_itrt, i++)
	{
		data_normals[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			data_normals[i][j] = (*dbpt_itrt)[j];
		}
	}
	num_normals = i;

	triangles = new int*[sum_tris];
	for(itrt_intpt = list_tris.begin(), i = 0; itrt_intpt != list_tris.end(); ++itrt_intpt, i++)
	{
		triangles[i] = new int[3];
		for(j = 0; j < 3; j++)
		{
			triangles[i][j] = (*itrt_intpt)[j];
		}
	}
	num_tris = i;

	fclose(fp);	
	delete []temp_dbpt;
	delete []temp_intpt;
}

void COGLEventDoc::Normalize(CVisDVector& vec)
{
	if(vec.Length() > 0)
	{
		double length = Length(vec);
		int j;
	
		for(j = 0; j < vec.Length(); j++)
			vec[j] /= (length>0?length:1);
	}
	else return;
}

double COGLEventDoc::Length(CVisDVector vec)
{
	if(vec.Length() > 0)
	{
		double length = 0;//初始化
		int j;
	
		for(j = 0; j < vec.Length(); j++)
			length += vec[j]*vec[j];

		length = sqrt(length);

		return length;
	}
	else return -1;		
}


void COGLEventDoc::TxtMapping() 
{	
	int i, j;//, k;			
	
	//double ** temp_txt = new double*[K1 = M];//实际上这里的K1与M本来就应当相等
	//double * temp_inner = new double[K1];
	
	//获取图像数据参数
	//获取当前视图指针
	unsigned char* pBitmap = temp_view;//m_pBitmap;
	if(pBitmap == NULL)
		return;
	LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
	unsigned char* pBitmapData = pBitmap + ((LPBITMAPFILEHEADER)pBitmap)->bfOffBits;
	unsigned long biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
	unsigned long biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
	
	//计算变换后与模型上标定点实际对应的图像点，与初始标定的图像点坐标进行比较，以检验现在的纹理映射是否正确
	list_dbpt::iterator dbpt_itrt, dbpt_itrt_1;
	list_intpt::iterator intpt_itrt;
	double temp_mdl_pt[3];
	//int *temp_img_pt;
	/*
	delete []temp_img_pt;//将原先分配的内存释放
	if(temp_mdl_pt != NULL)
	temp_mdl_pt = NULL;//前面temp_mdl_pt被用于构造链表，现在需要重新分配内存使用
	temp_mdl_pt = new double[2];
	*/
	
	for(i = 0; i < mdl_pt_num; i++)
	{
		//只要读出前两个坐标数值
		for(j = 0; j < 3; j++)
		{
			temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//并没有将模型上标记点转化到实际尺度
		}		
		
		/*temp_img_pt = new int[2];
		//直接根据映射矩阵计算模型上标记点在图像上实际对应的像素位置
		temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][0][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][0][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][0][2]+inter_mat_list[iter_index-1][0][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][1][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][1][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][1][2]+inter_mat_list[iter_index-1][1][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		
		act_img_pts.insert(act_img_pts.end(), temp_img_pt);//记录实际的图像点坐标位置*/

		act_img_pts_list[iter_index-1][i][1] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][0][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][0][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][0][2]+inter_mat_list[iter_index-1][0][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		act_img_pts_list[iter_index-1][i][0] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][1][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][1][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][1][2]+inter_mat_list[iter_index-1][1][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
	}	
	
	for(i = 0; i < K1; i++)
	{
		temp_txt[i] = new double[2];//分配内存空间
		temp_txt[i][1] = (face1_points[i][0]*inter_mat_list[iter_index-1][0][0]+face1_points[i][1]*inter_mat_list[iter_index-1][0][1]+face1_points[i][2]*inter_mat_list[iter_index-1][0][2]+inter_mat_list[iter_index-1][0][3])/(face1_points[i][0]*inter_mat_list[iter_index-1][2][0]+face1_points[i][1]*inter_mat_list[iter_index-1][2][1]+face1_points[i][2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]);
		temp_txt[i][0] = (face1_points[i][0]*inter_mat_list[iter_index-1][1][0]+face1_points[i][1]*inter_mat_list[iter_index-1][1][1]+face1_points[i][2]*inter_mat_list[iter_index-1][1][2]+inter_mat_list[iter_index-1][1][3])/(face1_points[i][0]*inter_mat_list[iter_index-1][2][0]+face1_points[i][1]*inter_mat_list[iter_index-1][2][1]+face1_points[i][2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]);
		temp_txt[i][0] /= biWidth;
		temp_txt[i][1] /= biHeight;
		
		if(temp_txt[i][0] > 1)
			temp_txt[i][0] = 1;
		if(temp_txt[i][1] > 1)
			temp_txt[i][1] = 1;
		if(temp_txt[i][0] < 0)
			temp_txt[i][0] = 0;
		if(temp_txt[i][1] < 0)
			temp_txt[i][1] = 0;

		/*
		while(temp_txt[i][0] > 1)
			temp_txt[i][0] -= 1;
		while(temp_txt[i][1] > 1)
			temp_txt[i][1] -= 1;
		while(temp_txt[i][0] < 0)
			temp_txt[i][0] += 1;
		while(temp_txt[i][1] < 0)
			temp_txt[i][1] += 1;
		*/
	}	
	
	//face1_texture.insert(face1_texture.end(), temp_txt);
	//view_pt.insert(view_pt.end(), m_pBitmap);
	
	CVisDVector m3(3);
	CVisDVector r3(3);
	
	for(j = 0; j < 3; j++)
	{
		m3[j] = inter_mat_list[iter_index-1][2][j];//第1到3个元素
	}
	
	double m_34 = inter_mat_list[iter_index-1][2][3];;
	r3 = m3*(m_34>0?1:-1);
	
	/*
	//计算对应于当前视图的内积
	CVisDVector temp(3);
	
	for(i = 0; i < K1; i++)
	{
		for(j = 0; j < 3; j++)
		{
			temp[j] = face1_normals[i][j];
		}
		
		temp_inner[i] = -r3*temp;
	}
	
	view_inner.insert(view_inner.end(), temp_inner);
	
	num_view++;
	*/
	
	textured = true;//表示已经生成了纹理图，当然只是当前视图的，当然这与transformed是不同的
	with_txt = false;//即便原obj文件中附带了纹理信息，也已经不再使用之
	
	//未第二次标点，无需再清零//mdl_pt_num = img_pt_num = 0;
	
	//只要COGLEventView对应窗口刷新即可
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->Invalidate(FALSE);
	pView->UpdateData(FALSE);
	
	CTexture *pView1 = (CTexture *)GetTextureView();
	pView1->Invalidate(FALSE);
	pView1->UpdateData(FALSE);
}

void COGLEventDoc::OnReadModel() 
{
	if(!m_read)//第一次读入模型
	{
		m_read = ReadObj_Extd(face1_points, face1_normals, texture_map, triangles1, nml_tris, txt_tris, M, M1, K1, L1, with_txt);		
		//if(!with_txt)//文件没有附带纹理坐标
		{
			temp_txt = new double*[K1 = M];
			for(int i = 0; i < K1; i++)
				temp_txt[i] = new double[2];
		}
	}
	else
	{
		int i;
		//首先清空原先存储在数组中的数据，释放内存空间
		//清空顶点及法向量信息
		for(i = 0; i < M; i++)
		{
			delete []face1_points[i];
			//delete []face1_normals[i];
		}
		delete []face1_points;
		
		for(i = 0; i < M1; i++)
		{
			//delete []face1_points[i];
			delete []face1_normals[i];
		}
		delete []face1_normals;

		//清空纹理坐标信息
		if(texture_map != NULL)
		{
			for(i = 0; i < K1; i++)
			{
				delete []texture_map[i];
			}
			delete []texture_map;
		}

		//清空三角形信息（包括顶点和法向量）
		for(i = 0; i < L1; i++)
		{
			delete []triangles1[i];
			delete []nml_tris[i];			
		}
		delete []triangles1;
		delete []nml_tris;

		if(with_txt)//读入obj文件中若附带了纹理坐标信息
		{
			/*
			for(i = 0; i < L1; i++)
			{
				delete []txt_tris[i];
			}
			delete []txt_tris;
			*/
		}
		
		obj_fp_lc = img_fp_lc = true;//开始标定且未完成
		mdl_pt_num = img_pt_num = 0;//初始化被标定的点数，其实投影映射后已经清零了
	
		list_dbpt_2::iterator dbpt_itrt_2;
		list_dbpt::iterator dbpt_itrt;
		list_intpt::iterator intpt_itrt;
		list_charpt::iterator itrt_charpt;
		//若已经映射了若干视图，要逐一将其对应的纹理坐标清空
		for(dbpt_itrt_2 = face1_texture.begin(); dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2)
		{
			for(i = 0; i < K1; i++)
			{
				delete [](*dbpt_itrt_2)[i];
			}
			delete [](*dbpt_itrt_2);
		}
		//清空链表
		face1_texture.erase(face1_texture.begin(), face1_texture.end());
		
		//将各视图对应的可视内积图清空
		for(dbpt_itrt = view_inner.begin(); dbpt_itrt != view_inner.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);		
		}
		//清空链表
		view_inner.erase(view_inner.begin(), view_inner.end());

		/*
		//这一部分暂时不用，如果读入模型而并不重新读入图像呢，为什么要清空内存，即便要清空重新读入，在ReadBitmap中也有相应的处理方案
		if(m_pBitmap != NULL)
		{
			free(m_pBitmap);//因为m_pBitmap都是通过调用ReadBitmap中的malloc来赋值的
			//因为并没有额外的内存分配，所以不用释放
			m_pBitmap = NULL;
		}
		*/
		//将各视图数据对应的内存清空
		for(itrt_charpt = view_pt.begin(); itrt_charpt != view_pt.end(); ++itrt_charpt)
		{
			if((*itrt_charpt) != NULL)
			{
				if(*itrt_charpt != m_pBitmap)//m_pBitmap暂时还不能清空
				{
					free(*itrt_charpt);
					(*itrt_charpt) = NULL;//将视图指针赋为空
				}
			}
		}
		//直接清空链表即可
		view_pt.erase(view_pt.begin(), view_pt.end());

		textured = false;
		with_txt = true;
		txt_type = false;//true;
		
		transformed = false;

		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());	
		for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
		for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		img_pts.erase(img_pts.begin(), img_pts.end());	
		for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());	

		//如果是在调用了OnTxtBlend之后再重新读入模型这些步骤就是有必要的
		//free(m_pTransfered);
		if(m_pTransfered != NULL)//而m_pTransfered的内存分配是在OnTxtBlend函数中完成的
			delete []m_pTransfered;//因为是用new方式分配内存的

		//将柱体纹理图的纹理坐标清空
		if(txt_map != NULL)
		{
			for(i = 0; i < K1; i++)
			{
				delete []txt_map[i];
			}
			delete []txt_map;
		}		

		cylindrical_map = false;
		
		//表示现在需要重新读入数据
		m_read = false;		

		K1 = 0;//清零

		//重新读入模型数据
		m_read = ReadObj_Extd(face1_points, face1_normals, texture_map, triangles1, nml_tris, txt_tris, M, M1, K1, L1, with_txt);				

		if(!with_txt)//文件没有附带纹理坐标
		{
			temp_txt = new double*[K1 = M];
			for(int i = 0; i < K1; i++)
				temp_txt[i] = new double[2];
		}
	}
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	
	pView->Invalidate(FALSE);	
}

//在模型上标记特征点
void COGLEventDoc::OnPtLoc1() 
{
	//which_loc = 1;//在第二次及之后的位姿调整时要初始化

	obj_fp_lc = false;//开始标定且未完成
	mdl_pt_num = 0;//初始化被标定的点数，其实投影映射后已经清零了
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator intpt_itrt;
	for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	img_pts.erase(img_pts.begin(), img_pts.end());
	for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());

	//因为act_mdl_pts和act_img_pts只在txt_mapping函数中才可能重新被生成，所以在此处清空
	for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
	for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
	}
	act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
}

//在图像上标记特征点
void COGLEventDoc::OnPtLoc2() 
{
	//which_loc = 2;//是位姿调整后的标定
	
	//if(mdl_pt_num%8 == 0 && img_pt_num%8 == 0)//即便不是在上一次标定完成之后就要重新标定也应当被允许，也许是因为标定过程中出现错误而无法控制
	{
		num_view++;
		temp_img_pts = new list_intpt[1];
		img_pts_list.insert(img_pts_list.end(), temp_img_pts);
		temp_img_pt_cd = new list_intpt[1];
		img_pt_cd_list.insert(img_pt_cd_list.end(), temp_img_pt_cd);//注意temp_img_pt_cd用途不同于temp_img_pts

		img_fp_lc = false;	
		img_pt_num = 0;
		temp_img_pt_num = 0;

		//因为act_mdl_pts和act_img_pts只在txt_mapping函数中才可能重新被生成，所以在此处清空
		list_dbpt::iterator dbpt_itrt;
		list_intpt::iterator intpt_itrt;
		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());	
	}	
}

void COGLEventDoc::OnProjMap() 
{	
	if(transformed)//投影映射的前提是对应于各视图的映射矩阵都已经计算
	{
		//由于没有第二次标点，直接进行纹理映射
		TxtMapping();
	}
}

void COGLEventDoc::OnTxtBlend() 
{
	/*
	Txt_map dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(cylindrical_map == true)
			cylindrical_map = false;

		int i, j;
		double** txt_coord = new double*[num_view];//存储当前顶点在各视图中的纹理坐标
		for(j = 0; j < num_view; j++)
			txt_coord[j] = new double[2];			
		double* weight = new double[num_view];//存储当前顶点在各视图对应的的权重数值
		unsigned char** view_data = new unsigned char*[num_view];//存储各视图图像数据的指针，使用数组更易于操作？
		unsigned long* Height = new unsigned long[num_view];
		unsigned long* Width = new unsigned long[num_view];
		unsigned long* Align = new unsigned long[num_view];

		//pre_size = with = height;
		width = height = dlg.m_width;
		if(m_pTransfered != NULL)
		{
			//int size = sizeof(m_pTransfered);
			delete []m_pTransfered;
			m_pTransfered = NULL;
		}
		m_pTransfered = new unsigned char[width*height*3];
		if(txt_map != NULL)//txt_map并没有在该函数之外被分配内存过
		{
			for(i = 0; i < K1; i++)
			{
				if(txt_map[i] != NULL)
				{
					delete []txt_map[i];
				}
			}
			delete []txt_map;
		}
		if(texture_map != NULL)//若texture_map非空，只可能是读入的obj文件中附带了纹理坐标
		{
			for(i = 0; i < K1; i++)
			{
				if(texture_map[i] != NULL)
				{
					delete []texture_map[i];
				}
			}
			delete []texture_map;
		}
		
		txt_map = new double*[K1];//纹理坐标
		texture_map = new double*[K1];//各顶点对应的颜色数值，这与最初texture_map的含义有所不同，之前是用于记录纹理坐标而非颜色信息
		int x, y;
		double y_min = face1_points[0][0];//记录纵向上的最小值
		double y_max = face1_points[0][0];//记录纵向上的最大值

		list_charpt::iterator itrt_charpt;
		LPBITMAPINFO lpBitmapInfo;
		for(itrt_charpt = view_pt.begin(), i = 0; itrt_charpt != view_pt.end(); ++itrt_charpt, i++)
		{
			if((*itrt_charpt) == NULL)
				return;
			lpBitmapInfo = (LPBITMAPINFO)((*itrt_charpt) + 14);
			view_data[i] = (*itrt_charpt) + ((LPBITMAPFILEHEADER)(*itrt_charpt))->bfOffBits;
			Height[i] = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
			Width[i] = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
			Align[i] = (Width[i]*3+3)/4 *4;
		}

		list_dbpt::iterator dbpt_itrt;
		list_dbpt_2::iterator dbpt_itrt_2;
		
		int* cur = new int[num_view];//指示由纹理坐标计算得到的当前顶点颜色值在各视图中对应的位置
		int u, v;
		
		//求纵向上的最大最小值
		for(i = 0; i < M; i++)
		{
			if(face1_points[i][0] < y_min)
				y_min = face1_points[i][0];
			if(face1_points[i][0] > y_max)
				y_max = face1_points[i][0];
		}

		//计算各点的柱体纹理图坐标、颜色值
		for(i = 0; i < K1; i++)
		{
			//对z坐标值为零的情况要特殊处理
			x = (int)width*((((face1_points[i][2] == 0)?((face1_points[i][1] >= 0)?PI/2:-PI/2):atan(face1_points[i][1]/-face1_points[i][2]))+PI/2)/(2*PI));
			y = (int)(height*(face1_points[i][0]-y_min)/(y_max-y_min));

			//由内积计算当前顶点在各视图中对应的权重数值
			for(dbpt_itrt = view_inner.begin(), j = 0; dbpt_itrt != view_inner.end(); ++dbpt_itrt, j++)
			{
				weight[j] = (*dbpt_itrt)[i]>0?(*dbpt_itrt)[i]:0;
			}	
			//当前顶点在各视图中对应的纹理坐标
			for(dbpt_itrt_2 = face1_texture.begin(), j = 0; dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2, j++)
			{
				txt_coord[j][0] = (*dbpt_itrt_2)[i][0];
				txt_coord[j][1] = (*dbpt_itrt_2)[i][1];
			}	

			//当前顶点在各视图中对应的颜色值的位置
			for(j = 0; j < num_view; j++)
			{
				u = (txt_coord[j][0]*(Width[j]-1) - (int)(txt_coord[j][0]*(Width[j]-1)))<((int)(txt_coord[j][0]*(Width[j]-1))+1 - txt_coord[j][0]*(Width[j]-1))?(int)(txt_coord[j][0]*(Width[j]-1)):(int)(txt_coord[j][0]*(Width[j]-1))+1;
				v = (txt_coord[j][1]*(Height[j]-1) - (int)(txt_coord[j][1]*(Height[j]-1)))<((int)(txt_coord[j][1]*(Height[j]-1))+1 - txt_coord[j][1]*(Height[j]-1))?(int)(txt_coord[j][1]*(Height[j]-1)):(int)(txt_coord[j][1]*(Height[j]-1))+1;
				if(u == Width[j])
					u -= 1;
				if(v == Height[j])
					v -= 1;
				cur[j] = v*Align[j]+u*3;
			}

			double weight_sum = 0;
			for(j = 0; j < num_view; j++)
				weight_sum += weight[j];

			texture_map[i] = new double[3];
				
			for(j = 0; j < num_view; j++)
			{
				//因为有累加，所以先初始化
				if(j == 0)//在读第一幅位图时操作
				{
					//如果存在不止一个顶点对应到同一图像点，则可能不止一次被清空
					m_pTransfered[y*width*3+x*3] = 0;
					m_pTransfered[y*width*3+x*3+1] = 0;
					m_pTransfered[y*width*3+x*3+2] = 0;
					
					texture_map[i][0] = 0;
					texture_map[i][1] = 0;
					texture_map[i][2] = 0;
				}

				m_pTransfered[y*width*3+x*3] += weight[j]*view_data[j][cur[j]];
				m_pTransfered[y*width*3+x*3+1] += weight[j]*view_data[j][cur[j]+1];
				m_pTransfered[y*width*3+x*3+2] += weight[j]*view_data[j][cur[j]+2];				
				
				texture_map[i][0] += weight[j]*view_data[j][cur[j]];
				texture_map[i][1] += weight[j]*view_data[j][cur[j]+1];
				texture_map[i][2] += weight[j]*view_data[j][cur[j]+2];
			}

			m_pTransfered[y*width*3+x*3] = (unsigned char)(m_pTransfered[y*width*3+x*3]/weight_sum);
			m_pTransfered[y*width*3+x*3+1] = (unsigned char)(m_pTransfered[y*width*3+x*3+1]/weight_sum);
			m_pTransfered[y*width*3+x*3+2] = (unsigned char)(m_pTransfered[y*width*3+x*3+2]/weight_sum);
			
			texture_map[i][0] /= weight_sum;
			texture_map[i][1] /= weight_sum;
			texture_map[i][2] /= weight_sum;
			
			txt_map[i] = new double[2];
			txt_map[i][0] = (double)x/(width-1);
			txt_map[i][1] = (double)y/(height-1);
		}
		for(j = 0; j < num_view; j++)
			delete []txt_coord[j];		
		delete []txt_coord;
		delete []weight;
		delete []view_data;
		delete []Height;
		delete []Width;
		delete []Align;

		cylindrical_map = true;//标识生成了柱体纹理图，即完成了纹理融合		
	}
	else 
		AfxMessageBox("try again");	
		*/
}

void COGLEventDoc::OnReadImage() 
{
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("Bitmap (*.BMP)|*.BMP|"));

	if( dlg.DoModal()==IDOK )
	{
		CString cstrFileName = dlg.GetPathName();
		OnOpenDocument(cstrFileName);
	}		
}

void COGLEventDoc::OnPoseAdj() 
{
    Data_input data_input;
	if( data_input.DoModal()==IDOK )
	{
		list_dbpt::iterator dbpt_itrt;
		list_intpt::iterator intpt_itrt;
		for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
		for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
		for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		img_pts.erase(img_pts.begin(), img_pts.end());
		for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
		
		//因为act_mdl_pts和act_img_pts只在txt_mapping函数中才可能重新被生成，所以在此处清空
		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//释放链表中元素所指向的内存空间						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
		
		CString cstrFileName;
		CFileDialog dlg( TRUE,NULL,NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("txt文件(*.txt)|*.txt|"));
		if( dlg.DoModal()==IDOK )
		{
			cstrFileName = dlg.GetPathName();		
		}
		else 
		{
			return;
		}
		
		FILE *fp;
		fp = fopen(cstrFileName, "r+");
		if(fp == NULL)
		{
			AfxMessageBox("can not open the file");
			return;
		}
		fseek(fp, 0L, SEEK_SET);
		int num;
		double *temp_mdl_pt;
		int *temp_img_pt;
		double mdl_coord;
		int img_coord;
		char c;
		int i, j, k;

		//读入模型上已标记点数
		fscanf(fp, "%d", &num);
		mdl_pt_num = img_pt_num = num;
		fscanf(fp, "%c", &c);

		//读入模型上已标记点
		for(i = 0; i < mdl_pt_num; i++)
		{
			temp_mdl_pt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &mdl_coord);
				temp_mdl_pt[j] = mdl_coord;
			}		
			mdl_pts.insert(mdl_pts.end(), temp_mdl_pt);
		}
		fscanf(fp, "%c", &c);

		//读入视图数
		fscanf(fp, "%d", &num_view);
		fscanf(fp, "%c", &c);

		//读入在各视图中模型上已标记点的对应点
		for(k = 0; k < num_view; k++)
		{
			//读入当前视图上可见点数
			fscanf(fp, "%d", &temp_img_pt_num);
			img_pt_num_list.insert(img_pt_num_list.end(),temp_img_pt_num);
			fscanf(fp, "%c", &c);

			temp_img_pts = new list_intpt[1];//分配内存

			img_pts_list.insert(img_pts_list.end(), temp_img_pts);//插入链表指针的链表

			for(i = 0; i < img_pt_num; i++)
			{
				temp_img_pt = new int[2];//分配内存

				for(j = 0; j < 2; j++)
				{
					fscanf(fp, "%d", &img_coord);
					temp_img_pt[j] = img_coord;
				}		
				temp_img_pts[0].insert(temp_img_pts[0].end(), temp_img_pt);
			}

			fscanf(fp, "%c", &c);

			temp_img_pt_cd = new list_intpt[1];

			img_pt_cd_list.insert(img_pt_cd_list.end(), temp_img_pt_cd);

			for(i = 0; i < img_pt_num; i++)
			{
				temp_img_pt = new int[2];
				for(j = 0; j < 2; j++)
				{
					fscanf(fp, "%d", &img_coord);
					temp_img_pt[j] = img_coord;
				}		
				temp_img_pt_cd[0].insert(temp_img_pt_cd[0].end(), temp_img_pt);
			}

			fscanf(fp, "%c", &c);
		}
	}
	//if(mdl_pt_num == img_pt_num)	
	{
		//if(mdl_pt_num >= 6)
		{
			//构造矩阵
			CVisDMatrix C;//(2*mdl_pt_num, 9);
			CVisDMatrix D;//(2*mdl_pt_num, 3);
			CVisDVector y(9);
			CVisDVector z(3);
			CVisDMatrix E(3, 3);
			int i, j, k;
			list_dbpt::iterator dbpt_itrt;
			list_intpt::iterator intpt_itrt;
			list_llippt::iterator llippt_itrt;
			list_int::iterator int_itrt;

			Vis_Map = new bool*[mdl_pt_num];//行数即为模型上已标点数
			for(i = 0; i < mdl_pt_num; i++)
				Vis_Map[i] = new bool[num_view];//列数即为视图数

			Vis_Num = new int[mdl_pt_num];
			for(i = 0; i < mdl_pt_num; i++)
				Vis_Num[i] = 0;//初始化

			img_pts_mat = new int**[mdl_pt_num];//行数即为模型上已标点数
			for(i = 0; i < mdl_pt_num; i++)
				img_pts_mat[i] = new int*[num_view];//列数即为视图数

			inter_mat_list = new CVisDMatrix[num_view];

			int cur_view;//记录当前视图的序号

			//计算与各视图对应的映射矩阵
			for(llippt_itrt = img_pt_cd_list.begin(), int_itrt = img_pt_num_list.begin(), cur_view = 0; llippt_itrt != img_pt_cd_list.end(), int_itrt != img_pt_num_list.end(); ++llippt_itrt, ++int_itrt, cur_view++)
			{
				temp_img_pt_num = (*int_itrt);

				//根据当前视图中实际标记点数确定矩阵大小
				C.Resize(2*temp_img_pt_num, 9);
				D.Resize(2*temp_img_pt_num, 3);

				k = 0;
				for(dbpt_itrt = mdl_pts.begin(), intpt_itrt = (*llippt_itrt)[0].begin(), i = 0; dbpt_itrt != mdl_pts.end(), intpt_itrt != (*llippt_itrt)[0].end(), i < mdl_pt_num; ++dbpt_itrt, ++intpt_itrt, i++)
				{
					if((*intpt_itrt)[0] != -1)//当前遍历点在该视图中可见
					{
						Vis_Map[i][cur_view] = true;
						Vis_Num[i]++;

						//注意现在实际横向是y坐标而纵向是x坐标
						img_pts_mat[i][cur_view] = new int[2];
						img_pts_mat[i][cur_view][0] = (*intpt_itrt)[0];
						img_pts_mat[i][cur_view][1] = (*intpt_itrt)[1];

						for(j = 0; j < 3; j++)
						{
							D[2*k][j] = -(*intpt_itrt)[0]*(*dbpt_itrt)[j]*Max_V;//-ui*Xi[j]
							//打印偶数行的数据
							/*
							if(j != 2)
							fprintf(fp, "%f\t", D[2*i][j]);
							else 
							fprintf(fp, "%f\n", D[2*i][j]);
							*/
						}				
						
						for(j = 0; j < 3; j++)
						{
							D[2*k+1][j] = -(*intpt_itrt)[1]*(*dbpt_itrt)[j]*Max_V;//-vi*Xi[j]
							//打印奇数行的数据
							/*
							if(j != 2)
							fprintf(fp, "%f\t", D[2*i+1][j]);
							else 
							fprintf(fp, "%f\n", D[2*i+1][j]);
							*/
						}
						k++;//添加两个方程
					}
					else Vis_Map[i][cur_view] = false;
				}

				k = 0;
				for(dbpt_itrt = mdl_pts.begin(), intpt_itrt = (*llippt_itrt)[0].begin(), i = 0; dbpt_itrt != mdl_pts.end(), intpt_itrt != (*llippt_itrt)[0].end(), i < mdl_pt_num; ++dbpt_itrt, ++intpt_itrt, i++)
				{
					if((*intpt_itrt)[0] != -1)
					{
						for(j = 0; j < 3; j++)
						{
							C[2*k][j] = (*dbpt_itrt)[j]*Max_V;//还要变换到实际的坐标值
							//fprintf(fp, "%f\t", C[2*i][j]);
						}
						C[2*k][3] = 1;
						//fprintf(fp, "%f\t", C[2*i][3]);
						for(j = 0; j < 4; j++)
						{
							C[2*k][4+j] = 0;//从第5个到第8个元素的取值都是0
							//fprintf(fp, "%f\t", C[2*i][4+j]);
						}
						C[2*k][8] = -(*intpt_itrt)[0];//-ui
						//fprintf(fp, "%f\n", C[2*i][8]);
						
						for(j = 0; j < 4; j++)
						{
							C[2*k+1][j] = 0;//从第1个到第4个元素的取值都是0
							//fprintf(fp, "%f\t", C[2*i+1][j]);
						}
						for(j = 0; j < 3; j++)
						{
							C[2*k+1][4+j] = (*dbpt_itrt)[j]*Max_V;//还要变换到实际的坐标值
							//fprintf(fp, "%f\t", C[2*i+1][4+j]);
						}
						C[2*k+1][7] = 1;
						//fprintf(fp, "%f\t", C[2*i+1][7]);
						C[2*k+1][8] = -(*intpt_itrt)[1];//-vi
						//fprintf(fp, "%f\n", C[2*i+1][8]);
						k++;
					}					
				}
								
				E = D.Transposed()*D - D.Transposed()*C*(C.Transposed()*C).Inverted()*C.Transposed()*D;
				VisMinEigenValue(E, z);
				y = -(C.Transposed()*C).Inverted()*C.Transposed()*D*z;
				
				//temp_inter_mat = new CVisDMatrix[1];
				//temp_inter_mat[0].Resize(3, 4);//初始化时并没有给定大小
				//inter_mat_list.insert(inter_mat_list.end(), temp_inter_mat);
				inter_mat_list[cur_view].Resize(3, 4);//初始化时并没有给定大小
				
				//给中间参数矩阵赋值
				/*
				for(j = 0; j < 3; j++)
				{
					temp_inter_mat[0][0][j] = y[j];//第1到3个元素
					temp_inter_mat[0][1][j] = y[4+j];//第5到7个元素
					temp_inter_mat[0][2][j] = z[j];//第1到3个元素
				}
				temp_inter_mat[0][0][3] = y[3];//第4元素
				temp_inter_mat[0][1][3] = y[7];//第8个元素
				temp_inter_mat[0][2][3] = y[8];//第9个元素
				*/
				for(j = 0; j < 3; j++)
				{
					inter_mat_list[cur_view][0][j] = y[j];//第1到3个元素
					inter_mat_list[cur_view][1][j] = y[4+j];//第5到7个元素
					inter_mat_list[cur_view][2][j] = z[j];//第1到3个元素
				}
				inter_mat_list[cur_view][0][3] = y[3];//第4元素
				inter_mat_list[cur_view][1][3] = y[7];//第8个元素
				inter_mat_list[cur_view][2][3] = y[8];//第9个元素
			}

			iter_index = num_view;
			temp_view = m_pBitmap;
			
			mdl_pts_array = new double*[mdl_pt_num];//将模型上标记点存储到数组中
			opt_mdl_pts = new double*[mdl_pt_num];
			act_img_pts_list = new int**[num_view];
				
			double temp_mdl_pt[3];
			int temp_img_pt[2];
			double pre_err_sum, temp_err_sum;
			
			double pre_err_sum_2, temp_err_sum_2;//记录所有模型点在所有视图中实际映射点与初始标记点之间距离之和
			
			list_db pts_err_sum_list;//记录尚未更新的模型点对应的误差和项，最后会对其排序。
			
			bool* pts_update = new bool[mdl_pt_num];//标记模型点是否已更新
			bool update_over;//标记更新是否结束
			
			double** temp_opt_array = new double*[mdl_pt_num];//记录待更新的模型点
			
			double* pts_err_sum_array = new double[mdl_pt_num];//记录待更新的模型点对应的误差项
			double* pts_err_sum_array_1 = new double[mdl_pt_num];//记录更新前的模型点对应的误差项
			
			int update_sum = 0;//记录已更新的模型点数
			
			//初始化
			for(dbpt_itrt = mdl_pts.begin(), i = 0; dbpt_itrt != mdl_pts.end(), i < mdl_pt_num; ++dbpt_itrt, i++)
			{
				mdl_pts_array[i] = new double[3];
				for(j = 0; j < 3; j++)
					mdl_pts_array[i][j] = (*dbpt_itrt)[j];
				
				//分配内存
				opt_mdl_pts[i] = new double[3];		
			}
			
			for(i = 0; i < num_view; i++)
			{
				act_img_pts_list[i] = new int*[mdl_pt_num];
				for(j = 0; j < mdl_pt_num; j++)
				{
					act_img_pts_list[i][j] = new int[2];
				}
			}
			
			
			//fclose(fp);
			
			//obj_fp_lc = img_fp_lc = true;//这一轮已经标定完了
			//不必再显示被标定点
			pre_pt_num = mdl_pt_num;//保存历史数据
			//mdl_pt_num = img_pt_num = 0;//点数清零
			//清空链表——这一步暂且延后，因为数据在后面的映射误差计算时还要被使用
			/*
			mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
			img_pts.erase(img_pts.begin(), img_pts.end());
			img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
			*/			
			
			//已经计算了对应于各视图的映射矩阵
			transformed = true;

			//想记录各视图对应数据部分的指针及高宽信息，用于后续的颜色查找
			list_charpt::iterator itrt_charpt;
			LPBITMAPINFO lpBitmapInfo;
			view_data = new unsigned char*[num_view];//存储各视图图像数据的指针，使用数组更易于操作？
			biHeight = new unsigned long[num_view];//存储各视图图像的高度
			biWidth = new unsigned long[num_view];//存储各视图图像的宽度
			biAlign = new unsigned long[num_view];
			
			for(itrt_charpt = view_pt.begin(), k = 0; itrt_charpt != view_pt.end(); ++itrt_charpt, k++)
			{
				if((*itrt_charpt) == NULL)
					return;
				lpBitmapInfo = (LPBITMAPINFO)((*itrt_charpt) + 14);
				view_data[k] = (*itrt_charpt) + ((LPBITMAPFILEHEADER)(*itrt_charpt))->bfOffBits;
				biHeight[k] = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
				biWidth[k] = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
				biAlign[k] = (biWidth[k]*3+3)/4 *4;
			}
			
			UpdateAllViews(NULL);
		}
		//else			AfxMessageBox("the number of points localized must not be less than 6");
	}
	//else AfxMessageBox("the number of Points localized on the model must be equal to that localized on the image");
}

void COGLEventDoc::OnRightView() 
{
	CVisDMatrix rot(3, 3);
	int i, j;

	rot[0][0] = 0.0;
	rot[0][1] = 0.0;
	rot[0][2] = 1.0;
	rot[1][0] = 0.0;
	rot[1][1] = 1.0;
	rot[1][2] = 0.0;
	rot[2][0] = -1.0;
	rot[2][1] = 0.0;
	rot[2][2] = 0.0;
				
	
	//将模型由世界坐标变换到摄像机坐标从而现在视点即是原点
	CVisDMatrix temp(3, 1);
	for(i = 0; i < M; i++)
	{
		//点变换
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_points[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_points[i][j] = temp[j][0];
		}
		
		//法向量变换
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_normals[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_normals[i][j] = temp[j][0];
		}
	}	

	UpdateAllViews(NULL);
}

void COGLEventDoc::OnLeftView() 
{
	CVisDMatrix rot(3, 3);
	int i, j;

	rot[0][0] = 0.0;
	rot[0][1] = 0.0;
	rot[0][2] = -1.0;
	rot[1][0] = 0.0;
	rot[1][1] = 1.0;
	rot[1][2] = 0.0;
	rot[2][0] = 1.0;
	rot[2][1] = 0.0;
	rot[2][2] = 0.0;
				
	
	//将模型由世界坐标变换到摄像机坐标从而现在视点即是原点
	CVisDMatrix temp(3, 1);
	for(i = 0; i < M; i++)
	{
		//点变换
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_points[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_points[i][j] = temp[j][0];
		}
		
		//法向量变换
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_normals[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_normals[i][j] = temp[j][0];
		}
	}	
	UpdateAllViews(NULL);
}

void COGLEventDoc::OnSave() 
{
	CFileDialog dlg( FALSE, ".obj", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("*.obj|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		CString cstrFileName = dlg.GetPathName();	
		CFile f;
		CFileException ex;
		if( f.Open( cstrFileName, CFile::modeCreate | CFile::modeWrite, &ex ) )
		{	
			f.Close();
			//先考虑最简单的文件形式
			char c;
			//double db;
			int i, j;//, index;
			//开始写入数据
			FILE* fp;
			fp = fopen(cstrFileName, "w");
			
			//写入顶点坐标数据
			for(i = 0; i < M; i++)
			{
				c = 'v';
				fprintf(fp, "%c\t", c);
				for(j = 0; j < 3; j++)
				{
					if(j < 2)
						fprintf(fp, "%f\t", face1_points[i][j]);
					else
						fprintf(fp, "%f\n", face1_points[i][j]);
				}
			}

			//写入顶点法向量数据
			for(i = 0; i < M1; i++)
			{
				c = 'v';
				fprintf(fp, "%c", c);
				c = 'n';				
				fprintf(fp, "%c\t", c);
				for(j = 0; j < 3; j++)
				{
					if(j < 2)
						fprintf(fp, "%f\t", face1_normals[i][j]);
					else
						fprintf(fp, "%f\n", face1_normals[i][j]);
				}
			}

			if(textured)//自己生成了纹理图
			{
				double ** temp_txt;				
				if(cylindrical_map)//生成了柱体纹理图
				{
					temp_txt = texture_map;

					//写入顶点纹理，即与顶点对应的颜色值					
					for(i = 0; i < K1; i++)
					{
						c = 'v';
						fprintf(fp, "%c", c);
						c = 'c';//注意不同于't'				
						fprintf(fp, "%c\t", c);
						for(j = 0; j < 3; j++)
						{
							if(j < 2)
								fprintf(fp, "%f\t", temp_txt[i][j]);
							else
								fprintf(fp, "%f\n", temp_txt[i][j]);
						}					
					}
				}
				else//仍使用与单幅视图对应的纹理图
				{
					list_dbpt_2::iterator itrt_dbpt_2;
					//读出纹理图指针链表当中的最后一个元素，即与当前视图对应的纹理图
					for(itrt_dbpt_2 = face1_texture.begin(); itrt_dbpt_2 != face1_texture.end(); ++itrt_dbpt_2)
					{
						temp_txt = *itrt_dbpt_2;
					}

					//写入顶点纹理坐标				
					for(i = 0; i < K1; i++)
					{
						c = 'v';
						fprintf(fp, "%c", c);
						c = 't';				
						fprintf(fp, "%c\t", c);
						for(j = 0; j < 3; j++)
						{
							if(j < 2)
								fprintf(fp, "%f\t", temp_txt[i][j]);
							else
								fprintf(fp, "%f\n", temp_txt[i][j]);
						}					
					}
				}

				//写入三角形信息
				for(i = 0; i < L1; i++)
				{
					c = 'f';				
					fprintf(fp, "%c\t", c);
					for(j = 0; j < 3; j++)//逐一写入三角形三个顶点的信息
					{
						fprintf(fp, "%d", triangles1[i][j]+1);//是真实的序号，所以加1
						c = '/';				
						fprintf(fp, "%c", c);
						fprintf(fp, "%d", triangles1[i][j]+1);//纹理坐标与顶点序号是严格对应的，是真实的序号，所以加1
						//c = '/';				
						fprintf(fp, "%c", c);
						if(j < 2)
							fprintf(fp, "%d\t", nml_tris[i][j]+1);//是真实的序号，所以加1
						else
							fprintf(fp, "%d\n", nml_tris[i][j]+1);
					}					
				}
			}
			else//没有纹理坐标
			{
				//写入三角形信息
				for(i = 0; i < L1; i++)
				{
					c = 'f';				
					fprintf(fp, "%c\t", c);
					for(j = 0; j < 3; j++)//逐一写入三角形三个顶点的信息
					{
						fprintf(fp, "%d", triangles1[i][j]+1);//是真实的序号，所以加1
						c = '/';				
						fprintf(fp, "%c", c);
						fprintf(fp, "%c", c);
						if(j < 2)
							fprintf(fp, "%d\t", nml_tris[i][j]+1);//与顶点序号是严格对应的，是真实的序号，所以加1
						else
							fprintf(fp, "%d\n", nml_tris[i][j]+1);
					}
				}
			}
			fclose(fp);				
		}
		else
		{
			// Complain if an error happened.
			// There is no reason to delete the ex object.
			
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			cout << "Could not open the file: ";
			cout << szError;
			return;
		}		
	}
	else return;
}

void COGLEventDoc::OnSaveData() 
{
	int i, j;
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator intpt_itrt;
	list_llippt::iterator llippt_itrt;
	list_llippt::iterator llippt_itrt_1;
	list_int::iterator int_itrt;

	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt文件(*.txt)|*.txt|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}
	else 
	{
		return;
	}

	FILE *fp;
	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return;
	}
	fseek(fp, 0L, SEEK_END);//还是在最末尾写入，不要与前面已经写入的数据混淆
	fprintf(fp, "%d\n", mdl_pt_num);
	//打印模型标定点，目前仍然是归一化的数据
	for(dbpt_itrt = pre_mdl_pts.begin(), i = 1; dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt, i++)
	{
		if(i != mdl_pt_num)
		{
			for(j = 0; j < 3; j++)//现在是三维点
			{
				fprintf(fp, "%f\t", (*dbpt_itrt)[j]);//现在是浮点型数据				
			}
		}
		else//只有在最后一个点才换行
		{
			for(j = 0; j < 3; j++)
			{
				if(j != 2)//现在是三维点
					fprintf(fp, "%f\t", (*dbpt_itrt)[j]);
				else 
					fprintf(fp, "%f\n", (*dbpt_itrt)[j]);
			}	
		}
	}
	fprintf(fp, "%d\n", num_view);
	//打印图像标定点
	for(llippt_itrt = img_pts_list.begin(), llippt_itrt_1 = img_pt_cd_list.begin(), int_itrt = img_pt_num_list.begin(); llippt_itrt != img_pts_list.end(), llippt_itrt_1 != img_pt_cd_list.end(), int_itrt != img_pt_num_list.end(); ++llippt_itrt, ++llippt_itrt_1, ++int_itrt)
	{
		//该视图上可见点数
		fprintf(fp, "%d\n", (*int_itrt));

		for(intpt_itrt = (*llippt_itrt)[0].begin(), i = 1; intpt_itrt != (*llippt_itrt)[0].end(); ++intpt_itrt, i++)
		{
			if(i != mdl_pt_num)
			{
				for(j = 0; j < 2; j++)//现在是二维点
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//现在是浮点型数据				
				}
			}
			else//只有在最后一个点才换行
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//现在是二维点
						fprintf(fp, "%d\t", (*intpt_itrt)[j]);
					else 
						fprintf(fp, "%d\n", (*intpt_itrt)[j]);
				}	
			}
		}

		for(intpt_itrt = (*llippt_itrt_1)[0].begin(), i = 1; intpt_itrt != (*llippt_itrt_1)[0].end(); ++intpt_itrt, i++)
		{
			if(i != mdl_pt_num)
			{
				for(j = 0; j < 2; j++)//现在是二维点
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//现在是浮点型数据				
				}
			}
			else//只有在最后一个点才换行
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//现在是二维点
						fprintf(fp, "%d\t", (*intpt_itrt)[j]);
					else 
						fprintf(fp, "%d\n", (*intpt_itrt)[j]);
				}	
			}
		}		
	}
	/*
	for(llippt_itrt = img_pt_cd_list.begin(); llippt_itrt != img_pt_cd_list.end(); ++llippt_itrt)
	{
		for(intpt_itrt = (*llippt_itrt)[0].begin(), i = 1; intpt_itrt != (*llippt_itrt)[0].end(); ++intpt_itrt, i++)
		{
			if(i != mdl_pt_num)
			{
				for(j = 0; j < 2; j++)//现在是二维点
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//现在是浮点型数据				
				}
			}
			else//只有在最后一个点才换行
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//现在是二维点
						fprintf(fp, "%d\t", (*intpt_itrt)[j]);
					else 
						fprintf(fp, "%d\n", (*intpt_itrt)[j]);
				}	
			}
		}
	}
	*/
	fclose(fp);

	//如果第二次标定效果好，也可以存储，但是存储在不同的文件中
	if(	which_loc == 2 && obj_fp_lc && img_fp_lc)
	{
		cstrFileName;
		CFileDialog dlg_1( TRUE,NULL,NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("txt文件(*.txt)|*.txt|"));
		if( dlg_1.DoModal()==IDOK )
		{
			cstrFileName = dlg_1.GetPathName();		
		}
		else 
		{
			return;
		}
		
		FILE *fp;
		fp = fopen(cstrFileName, "r+");
		if(fp == NULL)
		{
			AfxMessageBox("can not open the file");
			return;
		}
		fseek(fp, 0L, SEEK_END);//还是在最末尾写入，不要与前面已经写入的数据混淆
		for(i = 0; i < 8; i++)
		{
			if(i < 7)
			{
				for(j = 0; j < 3; j++)
				{
					fprintf(fp, "%lf\t", mdl_temp_pt[i][j]);
				}
			}
			else
			{
				for(j = 0; j < 3; j++)
				{
					if(j != 2)//现在是三维点
						fprintf(fp, "%lf\t", mdl_temp_pt[i][j]);
					else 
						fprintf(fp, "%lf\n", mdl_temp_pt[i][j]);
				}
			}
		}
		for(i = 0; i < 8; i++)
		{			
			if(i < 7)
			{
				fprintf(fp, "%d\t", img_temp_pt[i].x);
				fprintf(fp, "%d\t", img_temp_pt[i].y);
			}
			else
			{
				fprintf(fp, "%d\t", img_temp_pt[i].x);				
				fprintf(fp, "%d\n", img_temp_pt[i].y);
			}
		}
		for(i = 0; i < 2; i++)
		{
			if(i < 1)
			{
				for(j = 0; j < 3; j++)
				{
					fprintf(fp, "%lf\t", pre_mdl_ft_pt[i][j]);
				}
			}
			else
			{
				for(j = 0; j < 3; j++)
				{
					if(j != 2)//现在是三维点
						fprintf(fp, "%lf\t", pre_mdl_ft_pt[i][j]);
					else 
						fprintf(fp, "%lf\n", pre_mdl_ft_pt[i][j]);
				}
			}
		}
		for(i = 0; i < 2; i++)
		{			
			if(i < 1)
			{
				fprintf(fp, "%d\t", img_ft_pt[i].x);
				fprintf(fp, "%d\t", img_ft_pt[i].y);
			}
			else
			{
				fprintf(fp, "%d\t", img_ft_pt[i].x);				
				fprintf(fp, "%d\n", img_ft_pt[i].y);
			}
		}
	}
	fclose(fp);	
	/*
	//这里不必急于清空吧，如果在txt_mapping中要保存呢？
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//释放链表中元素所指向的内存空间						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	*/
	
}

void COGLEventDoc::OnDataAlalysis() 
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt文件(*.txt)|*.txt|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}
	else 
	{
		return;
	}

	FILE *fp;
	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return;
	}
	fseek(fp, 0L, SEEK_SET);
	int num;
	double mdl_coord;
	int img_coord;
	char c;
	int i, j;
	
	fscanf(fp, "%d", &num);
	fscanf(fp, "%c", &c);

	double **mdl_data_1 = new double*[num];
	double **mdl_data_2 = new double*[num];

	int **img_data_1 = new int*[num];
	int **img_data_2 = new int*[num];
	
	for(i = 0; i < num; i++)
	{
		mdl_data_1[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%lf", &mdl_coord);
			mdl_data_1[i][j] = mdl_coord;
		}	
	}
	fscanf(fp, "%c", &c);
	for(i = 0; i < num; i++)
	{
		img_data_1[i] = new int[2];
		for(j = 0; j < 2; j++)
		{
			fscanf(fp, "%d", &img_coord);
			img_data_1[i][j] = img_coord;
		}	
	}
	fscanf(fp, "%c", &c);
	//这一段数据并不需要进行比较
	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 2; j++)
		{
			fscanf(fp, "%d", &img_coord);			
		}		
	}
	fscanf(fp, "%c", &c);

	fscanf(fp, "%d", &num);
	fscanf(fp, "%c", &c);

	for(i = 0; i < num; i++)
	{
		mdl_data_2[i] = new double[3];
		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%lf", &mdl_coord);
			mdl_data_2[i][j] = mdl_coord;
		}	
	}
	fscanf(fp, "%c", &c);
	for(i = 0; i < num; i++)
	{
		img_data_2[i] = new int[2];
		for(j = 0; j < 2; j++)
		{
			fscanf(fp, "%d", &img_coord);
			img_data_2[i][j] = img_coord;
		}	
	}

	double **mdl_dif = new double*[num];
	int **img_dif = new int*[num];
	double max_mdl_dif_0, max_mdl_dif_1, max_mdl_dif_2, max_mdl_dif;
	int index_max_mdl_dif_0, index_max_mdl_dif_1, index_max_mdl_dif_2, index_max_mdl_dif;

	int max_img_dif_0, max_img_dif_1,max_img_dif;
	int index_max_img_dif_0, index_max_img_dif_1, index_max_img_dif;

	list_db mdl_dif_0, mdl_dif_1, mdl_dif_2, mdl_dif_sum;
	list_int index_mdl_dif_0, index_mdl_dif_1, index_mdl_dif_2, index_mdl_dif_sum;
	
	list_int img_dif_0, img_dif_1, img_dif_sum;
	list_int index_img_dif_0, index_img_dif_1, index_img_dif_sum;

	list_db::iterator db_itrt;
	list_int::iterator int_itrt;

	int num_mdl_dif_0, num_mdl_dif_1, num_mdl_dif_2, num_mdl_dif_sum;
	int num_img_dif_0, num_img_dif_1, num_img_dif_sum;

	double thres_mdl_dif_0, thres_mdl_dif_1, thres_mdl_dif_2, thres_mdl_dif_sum;
	int thres_img_dif_0, thres_img_dif_1, thres_img_dif_sum;

	thres_mdl_dif_0 = thres_mdl_dif_1 = thres_mdl_dif_2 = 0.005;
	thres_mdl_dif_sum = 0.015;//0.000075;/*sqrt = 0.0087*/
	thres_img_dif_0 = thres_img_dif_1 = 0;
	thres_img_dif_sum = 2;

	//初始化
	num_mdl_dif_0 = num_mdl_dif_1 = num_mdl_dif_2 = num_mdl_dif_sum = 0;
	num_img_dif_0 = num_img_dif_1 = num_img_dif_sum = 0;

	for(i = 0; i < num; i++)
	{
		mdl_dif[i] = new double[3];
		img_dif[i] = new int[2];

		if(i == 0)//初始化
		{
			mdl_dif[i][0] = fabs(mdl_data_2[i][0] - mdl_data_1[i][0]);
			max_mdl_dif_0 = mdl_dif[i][0];
			index_max_mdl_dif_0 = i+1;
			if(mdl_dif[i][0] > thres_mdl_dif_0)
			{
				mdl_dif_0.insert(mdl_dif_0.end(), mdl_dif[i][0]);
				index_mdl_dif_0.insert(index_mdl_dif_0.end(), i+1);
				num_mdl_dif_0++;
			}
			
			mdl_dif[i][1] = fabs(mdl_data_2[i][1] - mdl_data_1[i][1]);
			max_mdl_dif_1 = mdl_dif[i][1];
			index_max_mdl_dif_1 = i+1;
			if(mdl_dif[i][1] > thres_mdl_dif_1)
			{
				mdl_dif_1.insert(mdl_dif_1.end(), mdl_dif[i][1]);
				index_mdl_dif_1.insert(index_mdl_dif_1.end(), i+1);
				num_mdl_dif_1++;
			}
			
			mdl_dif[i][2] = fabs(mdl_data_2[i][2] - mdl_data_1[i][2]);
			max_mdl_dif_2 = mdl_dif[i][2];
			index_max_mdl_dif_2 = i+1;
			if(mdl_dif[i][2] > thres_mdl_dif_2)
			{
				mdl_dif_2.insert(mdl_dif_2.end(), mdl_dif[i][2]);
				index_mdl_dif_2.insert(index_mdl_dif_2.end(), i+1);
				num_mdl_dif_2++;
			}
			
			max_mdl_dif = mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2];//mdl_dif[i][0]*mdl_dif[i][0]+mdl_dif[i][1]*mdl_dif[i][1]+mdl_dif[i][2]*mdl_dif[i][2];
			index_max_mdl_dif = i+1;
			if(mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2] > thres_mdl_dif_sum)
			{
				mdl_dif_sum.insert(mdl_dif_sum.end(), mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2]);
				index_mdl_dif_sum.insert(index_mdl_dif_sum.end(), i+1);
				num_mdl_dif_sum++;
			}
			
			img_dif[i][0] = fabs(img_data_2[i][0] - img_data_1[i][0]);
			max_img_dif_0 = img_dif[i][0];
			index_max_img_dif_0 = i+1;
			if(img_dif[i][0] > thres_img_dif_0)
			{
				img_dif_0.insert(img_dif_0.end(), img_dif[i][0]);
				index_img_dif_0.insert(index_img_dif_0.end(), i+1);
				num_img_dif_0++;
			}

			img_dif[i][1] = fabs(img_data_2[i][1] - img_data_1[i][1]);
			max_img_dif_1 = img_dif[i][1];
			index_max_img_dif_1 = i+1;
			if(img_dif[i][1] > thres_img_dif_1)
			{
				img_dif_1.insert(img_dif_1.end(), img_dif[i][1]);
				index_img_dif_1.insert(index_img_dif_1.end(), i+1);
				num_img_dif_1++;
			}
			
			max_img_dif = img_dif[i][0]+img_dif[i][1];//img_dif[i][0]*img_dif[i][0]+img_dif[i][1]*img_dif[i][1];
			index_max_img_dif = i+1;
			if(img_dif[i][0]+img_dif[i][1] > thres_img_dif_sum)
			{
				img_dif_sum.insert(img_dif_sum.end(), img_dif[i][0]+img_dif[i][1]);
				index_img_dif_sum.insert(index_img_dif_sum.end(), i+1);
				num_img_dif_sum++;
			}
		}
		else
		{
			mdl_dif[i][0] = fabs(mdl_data_2[i][0] - mdl_data_1[i][0]);
			if(mdl_dif[i][0] > max_mdl_dif_0)
			{
				max_mdl_dif_0 = mdl_dif[i][0];
				index_max_mdl_dif_0 = i+1;
			}
			if(mdl_dif[i][0] > thres_mdl_dif_0)
			{
				mdl_dif_0.insert(mdl_dif_0.end(), mdl_dif[i][0]);
				index_mdl_dif_0.insert(index_mdl_dif_0.end(), i+1);
				num_mdl_dif_0++;
			}

			mdl_dif[i][1] = fabs(mdl_data_2[i][1] - mdl_data_1[i][1]);
			if(mdl_dif[i][1] > max_mdl_dif_1)
			{
				max_mdl_dif_1 = mdl_dif[i][1];
				index_max_mdl_dif_1 = i+1;
			}
			if(mdl_dif[i][1] > thres_mdl_dif_1)
			{
				mdl_dif_1.insert(mdl_dif_1.end(), mdl_dif[i][1]);
				index_mdl_dif_1.insert(index_mdl_dif_1.end(), i+1);
				num_mdl_dif_1++;
			}

			mdl_dif[i][2] = fabs(mdl_data_2[i][2] - mdl_data_1[i][2]);
			if(mdl_dif[i][2] > max_mdl_dif_2)
			{
				max_mdl_dif_2 = mdl_dif[i][2];
				index_max_mdl_dif_2 = i+1;
			}
			if(mdl_dif[i][2] > thres_mdl_dif_2)
			{
				mdl_dif_2.insert(mdl_dif_2.end(), mdl_dif[i][2]);
				index_mdl_dif_2.insert(index_mdl_dif_2.end(), i+1);
				num_mdl_dif_2++;
			}

			if(mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2] > max_mdl_dif)
			{
				max_mdl_dif = mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2];
				index_max_mdl_dif = i+1;
			}
			if(mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2] > thres_mdl_dif_sum)
			{
				mdl_dif_sum.insert(mdl_dif_sum.end(), mdl_dif[i][0]+mdl_dif[i][1]+mdl_dif[i][2]);
				index_mdl_dif_sum.insert(index_mdl_dif_sum.end(), i+1);
				num_mdl_dif_sum++;
			}
			
			img_dif[i][0] = fabs(img_data_2[i][0] - img_data_1[i][0]);
			if(img_dif[i][0] > max_img_dif_0)
			{
				max_img_dif_0 = img_dif[i][0];
				index_max_img_dif_0 = i+1;
			}
			if(img_dif[i][0] > thres_img_dif_0)
			{
				img_dif_0.insert(img_dif_0.end(), img_dif[i][0]);
				index_img_dif_0.insert(index_img_dif_0.end(), i+1);
				num_img_dif_0++;
			}

			img_dif[i][1] = fabs(img_data_2[i][1] - img_data_1[i][1]);
			if(img_dif[i][1] > max_img_dif_1)
			{
				max_img_dif_1 = img_dif[i][1];
				index_max_img_dif_1 = i+1;
			}
			if(img_dif[i][1] > thres_img_dif_1)
			{
				img_dif_1.insert(img_dif_1.end(), img_dif[i][1]);
				index_img_dif_1.insert(index_img_dif_1.end(), i+1);
				num_img_dif_1++;
			}

			if(img_dif[i][0]+img_dif[i][1] > max_img_dif)
			{
				max_img_dif = img_dif[i][0]+img_dif[i][1];
				index_max_img_dif = i+1;
			}
			if(img_dif[i][0]+img_dif[i][1] > thres_img_dif_sum)
			{
				img_dif_sum.insert(img_dif_sum.end(), img_dif[i][0]+img_dif[i][1]);
				index_img_dif_sum.insert(index_img_dif_sum.end(), i+1);
				num_img_dif_sum++;
			}
		}
	}
	fclose(fp);

	CFileDialog dlg_1( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt文件(*.txt)|*.txt|"));
	if( dlg_1.DoModal()==IDOK )
	{
		cstrFileName = dlg_1.GetPathName();		
	}
	else 
	{
		return;
	}

	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return;
	}
	fseek(fp, 0L, SEEK_END);

	for(i = 0; i < num; i++)
	{
		if(i < num-1)
		{
			for(j = 0; j < 3; j++)
			{
				fprintf(fp, "%lf\t", mdl_dif[i][j]);
			}
		}
		else
		{
			for(j = 0; j < 2; j++)
			{
				fprintf(fp, "%lf\t", mdl_dif[i][j]);
			}
			fprintf(fp, "%lf\n", mdl_dif[i][j]);
		}
	}

	for(i = 0; i < num; i++)
	{
		if(i < num-1)
		{
			for(j = 0; j < 2; j++)
			{
				fprintf(fp, "%d\t", img_dif[i][j]);
			}
		}
		else
		{
			for(j = 0; j < 1; j++)
			{
				fprintf(fp, "%d\t", img_dif[i][j]);
			}
			fprintf(fp, "%d\n", img_dif[i][j]);
		}
	}
	
	fprintf(fp, "the index of model point which has maximum absolute difference about x coordinate is %d.\n", index_max_mdl_dif_0);
	fprintf(fp, "and the absolute difference is %lf.\n", max_mdl_dif_0);
	fprintf(fp, "the index of model point which has maximum absolute difference about y coordinate is %d.\n", index_max_mdl_dif_1);
	fprintf(fp, "and the absolute difference is %lf.\n", max_mdl_dif_1);
	fprintf(fp, "the index of model point which has maximum absolute difference about z coordinate is %d.\n", index_max_mdl_dif_2);
	fprintf(fp, "and the absolute difference is %lf.\n", max_mdl_dif_2);
	fprintf(fp, "the index of model point which has maximum absolute difference sum is %d.\n", index_max_mdl_dif);
	fprintf(fp, "and the sum is %lf.\n", max_mdl_dif);//sqrt(max_mdl_dif));

	fprintf(fp, "the index of image point which has maximum absolute difference about x coordinate is %d.\n", index_max_img_dif_0);
	fprintf(fp, "and the absolute difference is %d.\n", max_img_dif_0);
	fprintf(fp, "the index of image point which has maximum absolute difference about y coordinate is %d.\n", index_max_img_dif_1);
	fprintf(fp, "and the absolute difference is %d.\n", max_img_dif_1);
	fprintf(fp, "the index of image point which has maximum absolute difference sum is %d.\n", index_max_img_dif);
	fprintf(fp, "and the sum is %d.\n", max_img_dif);//(int)sqrt((double)max_img_dif));

	if(num_mdl_dif_0 > 0)
	{
		fprintf(fp, "the indices of model points which have absolute deference about x coordinate more than %lf are:\n", thres_mdl_dif_0);
		for(int_itrt = index_mdl_dif_0.begin(), i = 0; int_itrt != index_mdl_dif_0.end(); ++int_itrt, i++)
		{
			if(i < num_mdl_dif_0 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else fprintf(fp, "%d\n", (*int_itrt));
		}

		for(db_itrt = mdl_dif_0.begin(), i = 0; db_itrt != mdl_dif_0.end(); ++db_itrt, i++)
		{
			if(i < num_mdl_dif_0 - 1)	
				fprintf(fp, "%lf\t", (*db_itrt));
			else
				fprintf(fp, "%lf\n", (*db_itrt));
		}
	}
	if(num_mdl_dif_1 > 0)
	{
		fprintf(fp, "the indices of model points which have absolute deference about y coordinate more than %lf are:\n", thres_mdl_dif_1);
		for(int_itrt = index_mdl_dif_1.begin(), i = 0; int_itrt != index_mdl_dif_1.end(); ++int_itrt, i++)
		{
			if(i < num_mdl_dif_1 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}

		for(db_itrt = mdl_dif_1.begin(), i = 0; db_itrt != mdl_dif_1.end(); ++db_itrt, i++)
		{
			if(i < num_mdl_dif_1 - 1)	
				fprintf(fp, "%lf\t", (*db_itrt));
			else
				fprintf(fp, "%lf\n", (*db_itrt));
		}
	}
	if(num_mdl_dif_2 > 0)
	{
		fprintf(fp, "the indices of model points which have absolute deference about z coordinate more than %lf are:\n", thres_mdl_dif_2);
		for(int_itrt = index_mdl_dif_2.begin(), i = 0; int_itrt != index_mdl_dif_2.end(); ++int_itrt, i++)
		{
			if(i < num_mdl_dif_2 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}

		for(db_itrt = mdl_dif_2.begin(), i = 0; db_itrt != mdl_dif_2.end(); ++db_itrt, i++)
		{
			if(i < num_mdl_dif_2 - 1)	
				fprintf(fp, "%lf\t", (*db_itrt));
			else
				fprintf(fp, "%lf\n", (*db_itrt));
		}
	}
	if(num_mdl_dif_sum > 0)
	{
		fprintf(fp, "the indices of model points which have absolute deference sum more than %lf are:\n", thres_mdl_dif_sum);
		for(int_itrt = index_mdl_dif_sum.begin(), i = 0; int_itrt != index_mdl_dif_sum.end(); ++int_itrt, i++)
		{
			if(i < num_mdl_dif_sum - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}
		for(db_itrt = mdl_dif_sum.begin(), i = 0; db_itrt != mdl_dif_sum.end(); ++db_itrt, i++)
		{
			if(i < num_mdl_dif_sum - 1)	
				fprintf(fp, "%f\t", (*db_itrt));
			else
				fprintf(fp, "%lf\n", (*db_itrt));
		}
	}

	if(num_img_dif_0 > 0)
	{
		fprintf(fp, "the indices of image points which have absolute deference about x coordinate more than %d are:\n", thres_img_dif_0);
		for(int_itrt = index_img_dif_0.begin(), i = 0; int_itrt != index_img_dif_0.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_0 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}

		for(int_itrt = img_dif_0.begin(), i = 0; int_itrt != img_dif_0.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_0 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}
	}
	fprintf(fp, "the indices of image points which have absolute deference about y coordinate more than %d are:\n", thres_img_dif_1);
	if(num_img_dif_1 > 0)
	{
		for(int_itrt = index_img_dif_1.begin(), i = 0; int_itrt != index_img_dif_1.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_1 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}

		for(int_itrt = img_dif_1.begin(), i = 0; int_itrt != img_dif_1.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_1 - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}
	}
	if(num_img_dif_sum > 0)
	{
		fprintf(fp, "the indices of image points which have absolute deference sum more than %d are:\n", thres_img_dif_sum);
		for(int_itrt = index_img_dif_sum.begin(), i = 0; int_itrt != index_img_dif_sum.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_sum - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}

		for(int_itrt = img_dif_sum.begin(), i = 0; int_itrt != img_dif_sum.end(); ++int_itrt, i++)
		{
			if(i < num_img_dif_sum - 1)
				fprintf(fp, "%d\t", (*int_itrt));
			else
				fprintf(fp, "%d\n", (*int_itrt));
		}
	}

	fclose(fp);

	for(i = 0; i < num; i++)
	{
		delete []mdl_data_1[i];
		delete []mdl_data_2[i];
		delete []img_data_1[i];
		delete []img_data_2[i];
		delete []mdl_dif[i];
		delete []img_dif[i];
	}
	delete []mdl_data_1;
	delete []mdl_data_2;
	delete []img_data_1;
	delete []img_data_2;
	delete []mdl_dif;
	delete []img_dif;
}


//利用已计算得到的对应于各视图的映射矩阵来进一步逐个优化模型上已标出的各点
void COGLEventDoc::OnPrmtOpt() 
{
	int i, j, k, cur_view;
	list_dbpt::iterator dbpt_itrt;
	CVisDMatrix K, U;
	CVisDMatrix cur_mdl_pt(3, 1);
	double cur_dis, dis_sum;	
	mdl_pts_trace = new double**[mdl_pt_num];//记录迭代过程中模型上标记点的移动轨迹
	iter_num = 125;//迭代次数

	double temp_mdl_pt[3];
	int temp_img_pt[2];
	double pre_err_sum, temp_err_sum;

	double pre_err_sum_2, temp_err_sum_2;//记录所有模型点在所有视图中实际映射点与初始标记点之间距离之和

	list_db pts_err_sum_list;//记录尚未更新的模型点对应的误差和项，最后会对其排序。
	
	bool* pts_update = new bool[mdl_pt_num];//标记模型点是否已更新
	bool update_over;//标记更新是否结束
	
	double** temp_opt_array = new double*[mdl_pt_num];//记录待更新的模型点
	
	double* pts_err_sum_array = new double[mdl_pt_num];//记录待更新的模型点对应的误差项
	double* pts_err_sum_array_1 = new double[mdl_pt_num];//记录更新前的模型点对应的误差项

	int update_sum = 0;//记录已更新的模型点数	

	//初始化
	for(i = 0; i < mdl_pt_num; i++)
	{
		mdl_pts_trace[i] = new double*[iter_num];
		for(j = 0; j < iter_num; j++)
			mdl_pts_trace[i][j] = new double[3];
	}
	
	for(i = 0; i < num_view; i++)
	{
		act_img_pts_list[i] = new int*[mdl_pt_num];
		for(j = 0; j < mdl_pt_num; j++)
		{
			act_img_pts_list[i][j] = new int[2];
		}
	}

	//初始化
	for(i = 0; i < mdl_pt_num; i++)
		pts_update[i] = false;	
		
	pre_err_sum_2 = temp_err_sum_2 = 0;//初始化

	for(i = 0; i < mdl_pt_num; i++)
	{
		//根据当前点在其中可见视图数来重新确定矩阵大小，每幅视图贡献两个方程
		K.Resize(Vis_Num[i]*2, 3);
		U.Resize(Vis_Num[i]*2, 1);
		k = 0;
		for(cur_view = 0; cur_view < num_view; cur_view++)
		{
			if(Vis_Map[i][cur_view])//若当前点在该视图中可见
			{
				//已经检查过其正确性
				K[2*k][0] = inter_mat_list[cur_view][0][0] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][0];
				K[2*k][1] = inter_mat_list[cur_view][0][1] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][1];
				K[2*k][2] = inter_mat_list[cur_view][0][2] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][2];
				U[2*k][0] = img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][0][3];

				K[2*k+1][0] = inter_mat_list[cur_view][1][0] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][0];
				K[2*k+1][1] = inter_mat_list[cur_view][1][1] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][1];
				K[2*k+1][2] = inter_mat_list[cur_view][1][2] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][2];
				U[2*k+1][0] = img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][1][3];

				k++;//添加了两个方程
			}
		}
		cur_mdl_pt = (K.Transposed()*K).Inverted()*K.Transposed()*U;
		
		pre_err_sum = temp_err_sum = 0;//初始化
		
		for(cur_view = 0; cur_view < num_view; cur_view++)
		{
			if(Vis_Map[i][cur_view])//当前遍历点在该视图中可见
			{
				//只要读出前两个坐标数值
				for(j = 0; j < 3; j++)
				{
					temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//并没有将模型上标记点转化到实际尺度
				}	
				
				temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				
				//映射点与实际标记点间距离
				pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
				
				//只要读出前两个坐标数值
				for(j = 0; j < 3; j++)
				{
					temp_mdl_pt[j] = cur_mdl_pt[j][0];
				}
				
				temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				
				//映射点与实际标记点间距离
				temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
			}
		}
		
		pre_err_sum_2 += pre_err_sum;
		temp_err_sum_2 += pre_err_sum;

		//只有当前误差和比更新前误差和小时才更新映射矩阵
		if(temp_err_sum < pre_err_sum)
		{
			pts_update[i] = true;
			
			//重新计算与各视图对应的映射矩阵							
			for(j = 0; j < 3; j++)
				opt_mdl_pts[i][j] = cur_mdl_pt[j][0]/Max_V;//注意映射矩阵是根据模型上标记点的实际尺度来计算的

			//temp_err_sum_2 += temp_err_sum;
			temp_err_sum_2 += (temp_err_sum-pre_err_sum);

			update_sum ++;
		}
		else
		{
			for(j = 0; j < 3; j++)
				opt_mdl_pts[i][j] = mdl_pts_array[i][j];

			pts_err_sum_array[i] = temp_err_sum;
			pts_err_sum_array_1[i] = pre_err_sum;
			pts_err_sum_list.insert(pts_err_sum_list.end(), temp_err_sum-pre_err_sum);//将该误差和项加入到链表中
			
			temp_opt_array[i] = new double[3];
			for(j = 0; j < 3; j++)
				temp_opt_array[i][j] = cur_mdl_pt[j][0]/Max_V;//注意映射矩阵是根据模型上标记点的实际尺度来计算的
		}
	}

	/*CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt文件(*.txt)|*.txt|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}
	FILE *fp;
	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return;
	}
	fseek(fp, 0L, SEEK_END);//还是在最末尾写入，不要与前面已经写入的数据混淆
	fprintf(fp, "%f\n", pre_err_sum_2);
	fprintf(fp, "%f\n", temp_err_sum_2);*/		

	CFormCommandView1 *pView = (CFormCommandView1 *)GetFormCommandView();
	pView->m_error_sum.Format("%.3lf", pre_err_sum_2);
	pView->UpdateData(FALSE);
	
	pts_err_sum_list.sort();//将链表中元素按升序排列
	
	update_over = false;

	if(update_sum == mdl_pt_num)//所有点均已更新
		update_over = true;
	
	//计算与各视图对应的映射矩阵
	while(!update_over)
	{
		for(i = 0; i < mdl_pt_num; i++)
		{
			if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())) && temp_err_sum_2+pts_err_sum_array[i]-pts_err_sum_array_1[i] < pre_err_sum_2)//该映射矩阵尚未更新
			{				
				pts_update[i] = true;
				
				for(j = 0; j < 3; j++)
					opt_mdl_pts[i][j] = temp_opt_array[i][j];
				
				//temp_err_sum_2 += pts_err_sum_array[i];//暂时只累加与已更新的模型点对应的误差和项
				temp_err_sum_2 += (pts_err_sum_array[i]-pts_err_sum_array_1[i]);//暂时只累加与已更新的模型点对应的误差和项

				update_sum ++;
				
				pts_err_sum_list.erase(pts_err_sum_list.begin());//将该误差和项从链表中删除	
				
				if(update_sum == mdl_pt_num)//所有点均已更新
				{
					update_over = true;
					break;
				}				
			}
			else
				if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())))//该映射矩阵尚未更新
				{
					update_over = true;//如果最小项都无效，不必再试其余项
					break;
				}
		}
	}
	
	mdl_pts_opted = true;//模型上标记各点已被优化
	
	//计算优化前后模型上对应点之间距离之和
	dis_sum = 0;//初始化 
	for(i = 0; i < mdl_pt_num; i++)
	{
		cur_dis = 0;//初始化
		for(j = 0; j < 3; j++)
		{
			cur_dis += (opt_mdl_pts[i][j] - mdl_pts_array[i][j])*(opt_mdl_pts[i][j] - mdl_pts_array[i][j]);
		}
		cur_dis = sqrt(cur_dis);//求平方根
		dis_sum += cur_dis;
	}
	
	//fprintf(fp, "%f\n\n", temp_err_sum_2);
	//fclose(fp);

	pView = (CFormCommandView1 *)GetFormCommandView();
	pView->m_error_sum.Format("%.3lf", temp_err_sum_2);
	pView->UpdateData(FALSE);
	
	UpdateAllViews(NULL);
	
}

void COGLEventDoc::OnInvisible() 
{
	if(!img_fp_lc)	
	{
		int *temp_intpt = new int[2];
		
		temp_intpt[0] = -1;
		temp_intpt[1] = -1;
		
		temp_img_pts[0].insert(temp_img_pts[0].end(), temp_intpt);
		
		temp_intpt = new int[2];
		
		temp_intpt[0] = -1;
		temp_intpt[1] = -1;
		
		temp_img_pt_cd[0].insert(temp_img_pt_cd[0].end(), temp_intpt);
		
		img_pt_num++;
		
		if(img_pt_num == mdl_pt_num)//标记点数已经足够
		{
			img_fp_lc = true;
			img_pt_num_list.insert(img_pt_num_list.end(),temp_img_pt_num);
		}
	}
}

void COGLEventDoc::OnIterative() 
{	
	int i, j, k;	
	
	temp_iter_num = 0;//初始化
	iter_begin = true;//开始迭代
	
	//构造矩阵
	CVisDMatrix C;//(2*mdl_pt_num, 9);
	CVisDMatrix D;//(2*mdl_pt_num, 3);
	CVisDVector y(9);
	CVisDVector z(3);
	CVisDMatrix E(3, 3);
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator intpt_itrt;
	list_llippt::iterator llippt_itrt;
	list_int::iterator int_itrt;
	CVisDMatrix K, U;
	CVisDMatrix cur_mdl_pt(3, 1);	
	
	int cur_view;//记录当前视图的序号
	double dis_sum, cur_dis;
		
	CVisDMatrix temp_mat(3, 4);
	double temp_mdl_pt[3];
	int temp_img_pt[2];
	double pre_err_sum, temp_err_sum;//记录所有模型点在某视图中实际映射点与初始标记点之间距离之和或记录某模型点在所有视图中实际映射点与初始标记点之间距离之和
	double pre_err_sum_2, temp_err_sum_2;//记录所有模型点在所有视图中实际映射点与初始标记点之间距离之和
	//double inter_err_sum_2;

	list_db views_err_sum_list;//记录尚未更新的映射矩阵对应的误差和项，最后会对其排序。
	list_db pts_err_sum_list;//记录尚未更新的模型点对应的误差和项，最后会对其排序。
	
	bool* views_update = new bool[num_view];//标记映射矩阵是否已更新
	bool* pts_update = new bool[mdl_pt_num];//标记模型点是否已更新
	bool update_over;//标记更新是否结束
	
	CVisDMatrix* temp_inter_mat_array = new CVisDMatrix[num_view];//记录待更新的映射矩阵
	double** temp_opt_array = new double*[mdl_pt_num];//记录待更新的模型点
	
	double* views_err_sum_array = new double[num_view];//记录待更新的映射矩阵对应的误差项
	double* views_err_sum_array_1 = new double[num_view];//记录待更新的映射矩阵对应的误差项
	double* pts_err_sum_array = new double[mdl_pt_num];//记录待更新的模型点对应的误差项
	double* pts_err_sum_array_1 = new double[mdl_pt_num];//记录待更新的模型点对应的误差项

	int update_sum = 0;

	/*
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt文件(*.txt)|*.txt|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}
	FILE *fp;
	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return;
	}
	fseek(fp, 0L, SEEK_END);//还是在最末尾写入，不要与前面已经写入的数据混淆
	*/

	//while(temp_iter_num < iter_num)//dis_sum > mdl_pt_num*3)
	{
		for(i = 0; i < mdl_pt_num; i++)
		{
			for(j = 0; j < 3; j++)
			{
				mdl_pts_array[i][j] = opt_mdl_pts[i][j];
			}

			for(j = 0; j < 3; j++)
			{
				mdl_pts_trace[i][temp_iter_num][j] = mdl_pts_array[i][j];
			}
		}
		temp_iter_num++;

		//初始化
		for(i = 0; i < num_view; i++)
			views_update[i] = false;
		for(i = 0; i < mdl_pt_num; i++)
			pts_update[i] = false;	
		
		pre_err_sum_2 = temp_err_sum_2 = 0;// = inter_err_sum_2 = 0;
		update_sum = 0;

		//计算与各视图对应的映射矩阵
		for(int_itrt = img_pt_num_list.begin(), cur_view = 0; int_itrt != img_pt_num_list.end(); ++int_itrt, cur_view++)
		{
			temp_img_pt_num = (*int_itrt);
			
			//根据当前视图中实际标记点数确定矩阵大小
			C.Resize(2*temp_img_pt_num, 9);
			D.Resize(2*temp_img_pt_num, 3);
			
			k = 0;
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//当前遍历点在该视图中可见
				{
					for(j = 0; j < 3; j++)
					{
						D[2*k][j] = -img_pts_mat[i][cur_view][0]*mdl_pts_array[i][j]*Max_V;//-ui*Xi[j]
						//打印偶数行的数据
						/*
						if(j != 2)
						fprintf(fp, "%f\t", D[2*i][j]);
						else 
						fprintf(fp, "%f\n", D[2*i][j]);
						*/
					}				
					
					for(j = 0; j < 3; j++)
					{
						D[2*k+1][j] = -img_pts_mat[i][cur_view][1]*mdl_pts_array[i][j]*Max_V;//-vi*Xi[j]
						//打印奇数行的数据
						/*
						if(j != 2)
						fprintf(fp, "%f\t", D[2*i+1][j]);
						else 
						fprintf(fp, "%f\n", D[2*i+1][j]);
						*/
					}
					k++;//添加两个方程
				}
			}
			
			k = 0;
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//当前遍历点在该视图中可见
				{
					for(j = 0; j < 3; j++)
					{
						C[2*k][j] = mdl_pts_array[i][j]*Max_V;//还要变换到实际的坐标值
						//fprintf(fp, "%f\t", C[2*i][j]);
					}
					C[2*k][3] = 1;
					//fprintf(fp, "%f\t", C[2*i][3]);
					for(j = 0; j < 4; j++)
					{
						C[2*k][4+j] = 0;//从第5个到第8个元素的取值都是0
						//fprintf(fp, "%f\t", C[2*i][4+j]);
					}
					C[2*k][8] = -img_pts_mat[i][cur_view][0];//-ui
					//fprintf(fp, "%f\n", C[2*i][8]);
					
					for(j = 0; j < 4; j++)
					{
						C[2*k+1][j] = 0;//从第1个到第4个元素的取值都是0
						//fprintf(fp, "%f\t", C[2*i+1][j]);
					}
					for(j = 0; j < 3; j++)
					{
						C[2*k+1][4+j] = mdl_pts_array[i][j]*Max_V;//还要变换到实际的坐标值
						//fprintf(fp, "%f\t", C[2*i+1][4+j]);
					}
					C[2*k+1][7] = 1;
					//fprintf(fp, "%f\t", C[2*i+1][7]);
					C[2*k+1][8] = -img_pts_mat[i][cur_view][1];//-vi
					//fprintf(fp, "%f\n", C[2*i+1][8]);
					k++;
				}					
			}
			
			E = D.Transposed()*D - D.Transposed()*C*(C.Transposed()*C).Inverted()*C.Transposed()*D;
			VisMinEigenValue(E, z);
			y = -(C.Transposed()*C).Inverted()*C.Transposed()*D*z;
			
			//重新计算与各视图对应的映射矩阵
			for(j = 0; j < 3; j++)
			{
				temp_mat[0][j] = y[j];//第1到3个元素
				temp_mat[1][j] = y[4+j];//第5到7个元素
				temp_mat[2][j] = z[j];//第1到3个元素
			}
			temp_mat[0][3] = y[3];//第4元素
			temp_mat[1][3] = y[7];//第8个元素
			temp_mat[2][3] = y[8];//第9个元素
			
			pre_err_sum = temp_err_sum = 0;//初始化
			
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//当前遍历点在该视图中可见
				{
					//只要读出前两个坐标数值
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//并没有将模型上标记点转化到实际尺度
					}	
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//映射点与实际标记点间距离
					pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*temp_mat[0][0]+temp_mdl_pt[1]*temp_mat[0][1]+temp_mdl_pt[2]*temp_mat[0][2]+temp_mat[0][3])/(temp_mdl_pt[0]*temp_mat[2][0]+temp_mdl_pt[1]*temp_mat[2][1]+temp_mdl_pt[2]*temp_mat[2][2]+temp_mat[2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*temp_mat[1][0]+temp_mdl_pt[1]*temp_mat[1][1]+temp_mdl_pt[2]*temp_mat[1][2]+temp_mat[1][3])/(temp_mdl_pt[0]*temp_mat[2][0]+temp_mdl_pt[1]*temp_mat[2][1]+temp_mdl_pt[2]*temp_mat[2][2]+temp_mat[2][3]));
					
					//映射点与实际标记点间距离
					temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));

				}
			}
			
			pre_err_sum_2 += pre_err_sum;
			temp_err_sum_2 += pre_err_sum;
			
			//只有当当前误差和比更新前误差和小时才更新映射矩阵
			if(temp_err_sum < pre_err_sum)
			{
				views_update[cur_view] = true;
				
				//重新计算与各视图对应的映射矩阵
				for(i = 0; i < 3; i++)
				{
					for(j = 0; j < 4; j++)
					{
						inter_mat_list[cur_view][i][j] = temp_mat[i][j];
					}
				}
				
				//temp_err_sum_2 += temp_err_sum;//暂时只累加与已更新的映射矩阵对应的误差和项	
				
				temp_err_sum_2 += (temp_err_sum - pre_err_sum);//用更新后误差和项替换更新前误差和项
				
				update_sum++;
			}
			else
			{
				views_err_sum_array[cur_view] = temp_err_sum;
				views_err_sum_array_1[cur_view] = pre_err_sum;
				views_err_sum_list.insert(views_err_sum_list.end(), temp_err_sum-pre_err_sum);//将该误差和项加入到链表中
				temp_inter_mat_array[cur_view].Resize(3, 4);
				for(i = 0; i < 3; i++)
				{
					for(j = 0; j < 4; j++)
					{
						temp_inter_mat_array[cur_view][i][j] = temp_mat[i][j];
					}
				}

			}
		}
		
		//fprintf(fp, "%f\n", pre_err_sum_2);
		//fprintf(fp, "%f\n", temp_err_sum_2);
		views_err_sum_list.sort();//将链表中元素按升序排列
		
		update_over = false;

		if(update_sum == num_view)
			update_over = true;

		//计算与各视图对应的映射矩阵
		while(!update_over)
		{
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(!views_update[cur_view] && (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] == (*views_err_sum_list.begin())) && temp_err_sum_2+views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] < pre_err_sum_2)//该映射矩阵尚未更新
				{				
					views_update[cur_view] = true;
					
					//重新计算与各视图对应的映射矩阵
					for(i = 0; i < 3; i++)
					{
						for(j = 0; j < 4; j++)
						{
							inter_mat_list[cur_view][i][j] = temp_inter_mat_array[cur_view][i][j];
						}
					}
					
					//temp_err_sum_2 += views_err_sum_array[cur_view];//暂时只累加与已更新的映射矩阵对应的误差和项
					temp_err_sum_2 += (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view]);
					
					views_err_sum_list.erase(views_err_sum_list.begin());//将该误差和项从链表中删除					
					
					update_sum++;

					if(update_sum == num_view)
					{
						update_over = true;
						break;
					}
					
				}
				else
					if(!views_update[cur_view] && (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] == (*views_err_sum_list.begin())))//该映射矩阵尚未更新
					{
						update_over = true;//如果最小项都无效，不必再试其余项
						break;
					}
			}
		}

		//fprintf(fp, "%f\n\n", temp_err_sum_2);
		
		CFormCommandView1 *pView = (CFormCommandView1 *)GetFormCommandView();
		pView->m_error_sum.Format("%.3lf", temp_err_sum_2);
		pView->UpdateData(FALSE);

		pre_err_sum_2 = temp_err_sum_2 = 0;
		update_sum = 0;

		//利用前面计算的对应各视图的映射矩阵来优化各模型标记点
		for(i = 0; i < mdl_pt_num; i++)
		{
			//根据当前点在其中可见视图数来重新确定矩阵大小，每幅视图贡献两个方程
			K.Resize(Vis_Num[i]*2, 3);
			U.Resize(Vis_Num[i]*2, 1);
			k = 0;
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(Vis_Map[i][cur_view])//若当前点在该视图中可见
				{
					//已经检查过其正确性
					K[2*k][0] = inter_mat_list[cur_view][0][0] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][0];
					K[2*k][1] = inter_mat_list[cur_view][0][1] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][1];
					K[2*k][2] = inter_mat_list[cur_view][0][2] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][2];
					U[2*k][0] = img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][0][3];
					
					K[2*k+1][0] = inter_mat_list[cur_view][1][0] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][0];
					K[2*k+1][1] = inter_mat_list[cur_view][1][1] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][1];
					K[2*k+1][2] = inter_mat_list[cur_view][1][2] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][2];
					U[2*k+1][0] = img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][1][3];
					
					k++;//添加了两个方程
				}
			}
			cur_mdl_pt = (K.Transposed()*K).Inverted()*K.Transposed()*U;
			
			pre_err_sum = temp_err_sum = 0;//初始化
			
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(Vis_Map[i][cur_view])//当前遍历点在该视图中可见
				{
					//只要读出前两个坐标数值
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//并没有将模型上标记点转化到实际尺度
					}	
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//映射点与实际标记点间距离
					pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
					
					//只要读出前两个坐标数值
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = cur_mdl_pt[j][0];
					}
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//映射点与实际标记点间距离
					temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
				}
			}
			
			pre_err_sum_2 += pre_err_sum;
			temp_err_sum_2 += pre_err_sum;
			
			//只有当当前误差和比更新前误差和小时才更新映射矩阵
			if(temp_err_sum < pre_err_sum)
			{
				pts_update[i] = true;
				
				//重新计算与各视图对应的映射矩阵							
				for(j = 0; j < 3; j++)
					opt_mdl_pts[i][j] = cur_mdl_pt[j][0]/Max_V;//注意映射矩阵是根据模型上标记点的实际尺度来计算的

				//temp_err_sum_2 += temp_err_sum;//暂时只累加与已更新的映射矩阵对应的误差和项
				temp_err_sum_2 += (temp_err_sum-pre_err_sum);//暂时只累加与已更新的映射矩阵对应的误差和项
				update_sum++;
			}
			else
			{
				pts_err_sum_array[i] = temp_err_sum;
				pts_err_sum_array_1[i] = pre_err_sum;
				pts_err_sum_list.insert(pts_err_sum_list.end(), temp_err_sum-pre_err_sum);//将该误差和项加入到链表中

				temp_opt_array[i] = new double[3];
				for(j = 0; j < 3; j++)
					temp_opt_array[i][j] = cur_mdl_pt[j][0]/Max_V;//注意映射矩阵是根据模型上标记点的实际尺度来计算的
			}
		}
		
		//fprintf(fp, "%f\n", pre_err_sum_2);
		//fprintf(fp, "%f\n", temp_err_sum_2);
		
		pts_err_sum_list.sort();//将链表中元素按升序排列
		
		update_over = false;

		if(update_sum == mdl_pt_num)
			update_over = true;		

		//计算与各视图对应的映射矩阵
		while(!update_over)
		{
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())) && temp_err_sum_2+pts_err_sum_array[i]-pts_err_sum_array_1[i]<pre_err_sum_2)//该映射矩阵尚未更新
				{
					
					pts_update[i] = true;
					
					for(j = 0; j < 3; j++)
						opt_mdl_pts[i][j] = temp_opt_array[i][j];
					
					//temp_err_sum_2 += pts_err_sum_array[i];//暂时只累加与已更新的模型点对应的误差和项
					temp_err_sum_2 += (pts_err_sum_array[i]-pts_err_sum_array_1[i]);//暂时只累加与已更新的模型点对应的误差和项
					
					pts_err_sum_list.erase(pts_err_sum_list.begin());//将该误差和项从链表中删除					

					update_sum++;
					if(update_sum == mdl_pt_num)
					{
						update_over = true;
						break;
					}					
				}
				else
					if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())))//该映射矩阵尚未更新
					{
						update_over = true;//如果最小项都无效，不必再试其余项
						break;
					}
			}
		}

		//计算优化前后模型上对应点之间距离之和
		dis_sum = 0;//初始化 
		for(i = 0; i < mdl_pt_num; i++)
		{
			cur_dis = 0;//初始化
			for(j = 0; j < 3; j++)
			{
				cur_dis += (opt_mdl_pts[i][j] - mdl_pts_array[i][j])*(opt_mdl_pts[i][j] - mdl_pts_array[i][j]);
			}
			cur_dis = sqrt(cur_dis);//求平方根
			dis_sum += cur_dis;
		}		

		//fprintf(fp, "%f\n\n", temp_err_sum_2);
		//fclose(fp);
		
		pView = (CFormCommandView1 *)GetFormCommandView();
		pView->m_error_sum.Format("%.3lf", temp_err_sum_2);

		pView = (CFormCommandView1 *)GetFormCommandView();
		pView->m_M11.Format("%.3lf", inter_mat_list[iter_index-1][0][0]);
		pView->m_M12.Format("%.3lf", inter_mat_list[iter_index-1][0][1]);
		pView->m_M13.Format("%.3lf", inter_mat_list[iter_index-1][0][2]);
		pView->m_M14.Format("%.3lf", inter_mat_list[iter_index-1][0][3]);
		pView->m_M21.Format("%.3lf", inter_mat_list[iter_index-1][1][0]);
		pView->m_M22.Format("%.3lf", inter_mat_list[iter_index-1][1][1]);
		pView->m_M23.Format("%.3lf", inter_mat_list[iter_index-1][1][2]);
		pView->m_M24.Format("%.3lf", inter_mat_list[iter_index-1][1][3]);
		pView->m_M31.Format("%.3lf", inter_mat_list[iter_index-1][2][0]);
		pView->m_M32.Format("%.3lf", inter_mat_list[iter_index-1][2][1]);
		pView->m_M33.Format("%.3lf", inter_mat_list[iter_index-1][2][2]);
		pView->m_M34.Format("%.3lf", inter_mat_list[iter_index-1][2][3]);

		pView->UpdateData(FALSE);

		//根据计算的映射矩阵进行纹理映射
		TxtMapping();
		
		//optimized = true;	
	}	
}

void COGLEventDoc::OnNextview() 
{
	if(transformed)
	{
		int i;
		list_llippt::iterator llippt_itrt;
		list_charpt::iterator charpt_itrt;
		
		iter_index = iter_index+1;
		if(iter_index > num_view)
			iter_index -= num_view;

		for(i = 0, llippt_itrt = img_pts_list.begin(), charpt_itrt = view_pt.begin(); i < iter_index; ++llippt_itrt, ++charpt_itrt, i++)
		{
			temp_img_pts = (*llippt_itrt);
			temp_view = (*charpt_itrt);
		}	
		TxtMapping();
	} 	
}

void COGLEventDoc::OnSaveimage() 
{
	view_pt.insert(view_pt.end(), m_pBitmap);	
}

void COGLEventDoc::OnReadBkgd() 
{
	CFileDialog dlg( TRUE,NULL,NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Bitmap (*.BMP)|*.BMP|"));
	
	if( dlg.DoModal()==IDOK )
	{
		CString cstrFileName = dlg.GetPathName();
		OnOpenDocument(cstrFileName);
		if(m_pBkgd != NULL)
		{	
			free(m_pBkgd);
			m_pBkgd = NULL;
		}
		m_pBkgd = m_pBitmap;
	}	
}

void COGLEventDoc::OnReadFgd() 
{
	CFileDialog dlg( TRUE,NULL,NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Bitmap (*.BMP)|*.BMP|"));
	
	if( dlg.DoModal()==IDOK )
	{
		CString cstrFileName = dlg.GetPathName();
		OnOpenDocument(cstrFileName);
	}	
}

void COGLEventDoc::OnImgDiff() 
{
	int i, j, cur, edge_pos, pre_edge_pos, edge_num;
	//读入当前前景图像
	LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)(m_pBitmap+14);
	LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)m_pBitmap;
	unsigned char *lpData = m_pBitmap + lpBitmapFileHeader->bfOffBits;
	unsigned long biHeight = lpBitmapInfoHeader->biHeight;
	unsigned long biWidth = lpBitmapInfoHeader->biWidth;
	unsigned long biAlign = (biWidth*3+3)/4 *4;

	//读入当前背景图像
	LPBITMAPINFOHEADER lpBitmapInfoHeader_1 = (LPBITMAPINFOHEADER)(m_pBkgd+14);
	LPBITMAPFILEHEADER lpBitmapFileHeader_1 = (LPBITMAPFILEHEADER)m_pBkgd;
	unsigned char *lpData_1 = m_pBkgd + lpBitmapFileHeader_1->bfOffBits;
	unsigned long biHeight_1 = lpBitmapInfoHeader_1->biHeight;
	unsigned long biWidth_1 = lpBitmapInfoHeader_1->biWidth;

	unsigned char tempr, tempg, tempb, temp, prev;

	if(biHeight == biHeight_1 && biWidth == biWidth_1)
	{
		CFileDialog dlg( FALSE, ".bmp", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("Bitmap(*.bmp)|*.bmp|"));
		if( dlg.DoModal()==IDOK )
		{
			CString cstrFileName = dlg.GetPathName();	
			CFile f;
			CFileException e;
			if( f.Open( cstrFileName, CFile::modeCreate | CFile::modeWrite, &e ) )
			{
				for(i = 0; i < (int)biHeight; i++)
				{
					pre_edge_pos = edge_pos = 0;	
					edge_num = 0;
					for(j = 0; j < (int)biWidth; j++)//读出一行中的三分量存入对应数组
					{
						cur = i*biAlign+3*j;//当前像素起始位置
						tempb = abs(lpData[cur]-lpData_1[cur]);//要比较的是差异的大小
						tempg = abs(lpData[cur+1]-lpData_1[cur+1]);
						tempr = abs(lpData[cur+2]-lpData_1[cur+2]);
						if(j == 0)
						{
							prev = temp = (unsigned char)(tempr * 0.3 + tempg * 0.59 + tempb * 0.11);
						}
						else
						{
							prev = temp;
							temp = (unsigned char)(tempr * 0.3 + tempg * 0.59 + tempb * 0.11);
						}
						if(!((temp < 60 && prev > 60) || (temp > 60 && prev < 60)))
						{
							lpData[cur] = 0;
							lpData[cur+1] = 0;
							lpData[cur+2] = 0;
						}
						else
						{
							edge_num ++;
							
							pre_edge_pos = edge_pos;
							edge_pos = cur;	
							if(edge_num == 1)
							{
								lpData[edge_pos] = 0;
								lpData[edge_pos+1] = 0;
								lpData[edge_pos+2] = 255;
							}
							if(edge_num > 2)
							{
								lpData[pre_edge_pos] = 255;
								lpData[pre_edge_pos+1] = 0;
								lpData[pre_edge_pos+2] = 0;
							}
							
							/*if(pre_edge_pos == 0 && edge_pos == 0)
							{
								edge_pos = cur;
							}
							else
							{		
								if(pre_edge_pos != 0)
								{
									lpData[pre_edge_pos] = 0;
									lpData[pre_edge_pos+1] = 0;
									lpData[pre_edge_pos+2] = 0;
								}
								pre_edge_pos = edge_pos;
								edge_pos = cur;								
							}*/
						}
					}
				}
				f.Write( m_pBitmap, current_size);
			}
		}
		else 
		{
			AfxMessageBox("The File cannot be save.");
			return;
		}
	}
	else
	{
		AfxMessageBox("The foreground and background must have a same size.");
		return;
	}
}

//建立各幅视图中各像素与网格模型结点之间的对应关系
void COGLEventDoc::OnMap() 
{
}

void COGLEventDoc::Mapping()
{	
	unsigned long v, u, i, j, k;
	//记录各像素对应的当前与上一次的网格顶点的z坐标，把最接近于视点的网格顶点找出
	double* temp_z = NULL;//记录在当前视图中在这次循环得到的与各像素对应的网格顶点在旋转后的深度值
	double* prev_z = NULL;//记录在当前视图中在前次循环得到的与各像素对应的网格顶点在旋转后的深度值
	list_charpt::iterator itrt_charpt;//字符链表的遍历器		
	unsigned char* temp_view_pt;//当前视图的指针	
	CVisDVector m3(3);
	double m_34;
	CVisDVector temp(3);//现在感觉temp这个命名实在是不够明朗
	
	//初始化
	for(k = 0; k < num_view; k++)
	{
		for(i = 0; i < M; i++)
		{
			vtx_map[k][i][0] = vtx_map[k][i][1] = -1;
		}
	}

	for(k = 0; k < num_view; k++)
	{
		for(v = 0; v < biHeight[k]; v++)
		{
			for(u = 0; u < biWidth[k]; u++)
			{
				pxl_map[k][v*biWidth[k]+u] = -1;
			}			
		}
	}
				
	//最外层循环于视图指针的链表
	for(k = 0; k < num_view; k++)
	{
		//为当前输入视图的各像素确定其对应的网格顶点
		//计算与当前视图对应的旋转矩阵的第三行	
		for(j = 0; j < 3; j++)
		{
			m3[j] = inter_mat_list[k][2][j];//第1到3个元素
		}
		
		m_34 = inter_mat_list[k][2][3];;
		r3[k] = m3*(m_34>0?1:-1);
		
		//根据新的视图更新数据
		if(temp_z != NULL)
			delete []temp_z;
		if(prev_z != NULL)
			delete []prev_z;
		//空间大小是由该视图的高宽决定的
		temp_z = new double[biHeight[k]*biWidth[k]];
		prev_z = new double[biHeight[k]*biWidth[k]];

		/*
		//初始化
		for(i = 0; i < biHeight[k]*biWidth[k]; i++)
		{
			prev_z[i] = 10000;
		}
		*/
		
		//第二层循环遍历于网格模型的各顶点
		for(i = 0; i < M; i++)
		{
			for(j = 0; j < 3; j++)
			{
				temp[j] = face1_normals[i][j];//记录当前网格顶点的法向量
			}
			//现在忽然不明白为什么要加一个负号，哈哈，现在明白了，负号意味着Z轴负方向
			if(-r3[k]*temp>cos(PI/2))//如果旋转操作后该网格顶点的法向量与视线方向的夹角小于90度
			{
				//通过当前视图的映射矩阵来计算各网格顶点对应的像素（这样说比较的准确）
				v = (face1_points[i][0]*inter_mat_list[k][0][0]+face1_points[i][1]*inter_mat_list[k][0][1]+face1_points[i][2]*inter_mat_list[k][0][2]+inter_mat_list[k][0][3])/(face1_points[i][0]*inter_mat_list[k][2][0]+face1_points[i][1]*inter_mat_list[k][2][1]+face1_points[i][2]*inter_mat_list[k][2][2]+inter_mat_list[k][2][3]);
				u = (face1_points[i][0]*inter_mat_list[k][1][0]+face1_points[i][1]*inter_mat_list[k][1][1]+face1_points[i][2]*inter_mat_list[k][1][2]+inter_mat_list[k][1][3])/(face1_points[i][0]*inter_mat_list[k][2][0]+face1_points[i][1]*inter_mat_list[k][2][1]+face1_points[i][2]*inter_mat_list[k][2][2]+inter_mat_list[k][2][3]);
				
				if(u < biWidth[k] && v < biHeight[k])//注意边界问题
				{			
					//计算旋转后网格顶点的z坐标
					for(j = 0; j < 3; j++)
					{
						temp[j] = face1_points[i][j];
					}
					
					temp_z[v*biWidth[k]+u] = r3[k]*temp;//计算与当前网格顶点在旋转后的z坐标并更新其对应的像素的相应取值//我认为在这里是没有必要加一个负号的
					
					if(pxl_map[k][v*biWidth[k]+u] == -1 || temp_z[v*biWidth[k]+u] < prev_z[v*biWidth[k]+u])/*该像素尚未与任何网格顶点建立对应关系*///判断当前网格顶点是否距离视点更"近"，是从z轴负方向上看的
					{
						pxl_map[k][v*biWidth[k]+u] = i;//更新对应的网格顶点的序号
						//map_num[k][v*biWidth[k]+u]++;//与当前视图中该像素对应的网格顶点个数增加1个
						//不太明白这两行有什么意义
						/*
						if(temp_z[v*biWidth+u] < prev_z[v*biWidth+u])
							prev_z[v*biWidth+u] = temp_z[v*biWidth+u];	
						*/
						prev_z[v*biWidth[k]+u] = temp_z[v*biWidth[k]+u];					
					}
				}
			}
		}
		
		//计算出当前视图中与各像素对应的网格顶点之后，反过来计算各网格顶点在当前视图中的对应像素
		//对当前视图的像素进行循环
		//初始化！
		map_pxl_num[k] = 0;
		for(v = 0; v < biHeight[k]; v++)
		{			
			for(u = 0; u < biWidth[k]; u++)
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//该像素已经确定了对应的网格顶点
				{				
					vtx_map[k][pxl_map[k][v*biWidth[k]+u]][0] = u;
					vtx_map[k][pxl_map[k][v*biWidth[k]+u]][1] = v;
					//记录当前视图中存在对应网格顶点的像素的数目，之前已经进行了初始化
					map_pxl_num[k]++;
				}		
			}			
		}	
		//这里就会产生一个问题，会否存在网格顶点在某些视图中不存在对应像素呢？实际上换言之，就是是否不可见？从实际情况来看，这个回答当然应该是肯定的。
		//不可见的含义可以理解为被遮挡，那么一个顶点可能在像素对应的视线上的靠后位置。
		
		/*
		//考察各幅视图里每个像素可能与多少个网格顶点对应
		for(v = 0; v < biHeight; v++)
		{			
			for(u = 0; u < biWidth; u++)
			{
				if(map_num[k][v*biWidth+u] > 1)//该像素已经确定了对应的网格顶点
				{	
					map_num[k][v*biWidth+u] = map_num[k][v*biWidth+u];
				}		
			}			
		}	
		*/
	}
	
	//计算在各幅视图中拥有对应网格顶点的像素的数目之和，实际上也是总的误差函数的个数
	//初始化！要不每次迭代时就会累加上去的。。。
	map_pxl_num_sum = 0;
	for(k = 0; k < num_view; k++)
	{
		map_pxl_num_sum += map_pxl_num[k];
	}
	
	mapped = true;	
}

void COGLEventDoc::Data_Computation()
{
	int i, j, k, l, m, u_k, v_k;//u, v, ;
	int gray_k[9];//用于以Sobel掩膜计算在指定像素处的偏导
	CVisDVector pst(3);//记录与当前所遍历像素对应的网格顶点的空间坐标
	CVisDVector nml(3);//记录与当前所遍历像素对应的网格顶点的法向量	
	
	//每次更新投影矩阵参数后，都要重新计算相关的数据，包括权重，偏导，灰度信息
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			//pst[j] = face1_points[i][j];//法向量在求权值时是一定要用到的
			nml[j] = face1_normals[i][j];//法向量在求权值时是一定要用到的
		}
		
		//凡是累加项都要初始清0
		deno_of_weight[i] = 0;	//权重的分母部分初始化为0	
		vtx_gray[i] = 0;//像素对应灰度初始化为0
		
		//第二层循环于各视图
		for(k = 0; k < num_view; k++)
		{
			if(vtx_map[k][i][0] != -1)//该像素在该视图中存在一个对应像素
			{
				//初始化			
				vis_img[k][i] = true;
				if(r3[k]*nml>cos(PI/2))//判断网格顶点在视图k中是否可见，严格讲并不确定内积大于0是否与可见性判断等价？
					vis_img[k][i] = false;
				else
					deno_of_weight[i] += r3[k]*nml;//累加以计算分母部分，是网格顶点在其中可见的视图对应的内积的求和，这里把负号丢掉了
			}
			else //有可能初始化就都是为true
				vis_img[k][i] = false;
		}
		//计算网格顶点在各视图（顶点在其中可见）中对应的权值，对应像素的灰度，及进一步的，在对应像素处关于X和Y方向的偏导
		for(k = 0; k < num_view; k++)
		{
			if(vis_img[k][i])//若该网格顶点在视图k中可见
			{
				//计算该网格顶点对应于视图k的权值
				weight[k][i] = r3[k]*nml/deno_of_weight[i];
				//计算该网格顶点在视图k中对应像素处关于该像素颜色的偏导
				//计算指定像素的坐标，是3*3区域的中心
				//记录网格顶点i在视图k中对应的像素的坐标
				u_k = vtx_map[k][i][0];
				v_k = vtx_map[k][i][1];
				//计算gray_k，亦即指定像素周围8邻域的灰度值
				for(l = -1; l < 2; l++)//纵向
				{
					for(m = -1; m < 2; m++)//横向
					{
						if(l == 0 && m == 0)//掩膜中心位置，即网格顶点对应像素处
								vtx_map_gray[k][i] = view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)] * 0.11;							

						//这里已经假定指定像素一定不在边界上，否则将可能出现访问越界！从现在的这些图像看，是有可能出现在边界上的！
						if(v_k+l > -1 && v_k+l < biHeight[k] && u_k+m > -1 && u_k+m < biWidth[k])
						{
							gray_k[(l+1)*3+(m+1)] = view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)] * 0.11;							
						}
						else//分情况讨论
						{
							if(v_k+l == -1)//上边界
							{
								if(u_k+m == -1)//左边界
									gray_k[(l+1)*3+(m+1)] = view_data[k][2] * 0.3 + view_data[k][1] * 0.59 + view_data[k][0] * 0.11;
								else
								{
									if(u_k+m == biWidth[k])//右边界
										gray_k[(l+1)*3+(m+1)] = view_data[k][3*(biWidth[k]-1)+2] * 0.3 + view_data[k][3*(biWidth[k]-1)+1] * 0.59 + view_data[k][3*(biWidth[k]-1)] * 0.11;
									else
										gray_k[(l+1)*3+(m+1)] = view_data[k][3*(u_k+m)+2] * 0.3 + view_data[k][3*(u_k+m)+1] * 0.59 + view_data[k][3*(u_k+m)] * 0.11;
								}												
							}	
							if(v_k+l == biHeight[k])//下边界
							{
								if(u_k+m == -1)//左边界
									gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]] * 0.11;
								else
								{
									if(u_k+m == biWidth[k])//右边界
										gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)] * 0.11;
									else
										gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)] * 0.11;
								}												
							}
						}
					}
				}
				//在横向和纵向用Sobel掩膜计算分别得到u,v方向的偏导
				//记录网格顶点在各视图中对应像素处的偏导
				derive_x[k][i] = gray_k[2]-gray_k[0]+2*(gray_k[5]-gray_k[3])+gray_k[8]-gray_k[6];
				derive_y[k][i] = gray_k[6]-gray_k[0]+2*(gray_k[7]-gray_k[1])+gray_k[8]-gray_k[2];	
				//计算网格顶点对应的灰度，实际上是其在各视图中对应像素灰度的加权平均
				vtx_gray[i] += weight[k][i]*vtx_map_gray[k][i];
			}
		}
	}
	
	data_cmpt = true;
}

void COGLEventDoc::Jacobian_Construction(CVisDMatrix& Jac_Mat)
{
	int i, j, k, l, m, n, u, v;
	CVisDVector pst(3);//记录与当前所遍历像素对应的网格顶点的空间坐标
	CVisDVector nml(3);//记录与当前所遍历像素对应的网格顶点的法向量
	double cmpt[3];//记录(x,y,z,1)与投影矩阵三行向量的内积
	int counted_row = 0, counted_col, cur_cnt_fun;//记录在遍历过程中每轮遍历开始前已计算的误差函数，参数和当前这一轮中已计算的误差函数的个数
	
	//外层循环于视图指针的链表
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;
		//初始化，第二层循环遍历于各视图
		for(v = 0; v < biHeight[k]; v++)//遍历行
		{
			for(u = 0; u < biWidth[k]; u++)//遍历列
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//对一个拥有对应网格顶点的像素构造其在Jacaobian Matrix中的相应元素
				{
					//从pxl_map可以得到当前视图中各像素是否存在对应网格顶点，以及若存在对应网格顶点，其序号
					//空间坐标和法向量都要用到
					for(j = 0; j < 3; j++)
					{
						pst[j] = face1_points[pxl_map[k][v*biWidth[k]+u]][j];
						nml[j] = face1_normals[pxl_map[k][v*biWidth[k]+u]][j];
					}
					//进入具体的对各参数求导模块
					//对各幅视图遍历
					counted_col = 0;//初始化
					for(i = 0; i < num_view; i++)
					{
						if(vtx_map[i][pxl_map[k][v*biWidth[k]+u]][0] != -1)
						{
							//计算分母部分								
							cmpt[0] = pst[0]*inter_mat_list[i][0][0]+pst[1]*inter_mat_list[i][0][1]+pst[2]*inter_mat_list[i][0][2]+inter_mat_list[i][0][3];								
							cmpt[1] = pst[0]*inter_mat_list[i][1][0]+pst[1]*inter_mat_list[i][1][1]+pst[2]*inter_mat_list[i][1][2]+inter_mat_list[i][1][3];								
							cmpt[2] = pst[0]*inter_mat_list[i][2][0]+pst[1]*inter_mat_list[i][2][1]+pst[2]*inter_mat_list[i][2][2]+inter_mat_list[i][2][3];								
							//对行号遍历
							for(l = 0; l < 3; l++)
							{
								switch(l)
								{
								case 0:
									//对列号遍历，但是列4区分与前3列
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+m] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_y[i][pxl_map[k][v*biWidth[k]+u]]*pst[m]/cmpt[2];
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+3] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_y[i][pxl_map[k][v*biWidth[k]+u]]/cmpt[2];
									break;
								case 1:
									//对列号遍历，但是列4区分与前3列
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+4+m] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_x[i][pxl_map[k][v*biWidth[k]+u]]*pst[m]/cmpt[2];
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+4+3] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_x[i][pxl_map[k][v*biWidth[k]+u]]/cmpt[2];
									break;
								case 2:
									//对列号遍历，但是列4区分与前3列
									//初始化
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+8+m] = -pst[m]*weight[i][pxl_map[k][v*biWidth[k]+u]]*(derive_x[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[1]+derive_y[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[0])/(cmpt[2]*cmpt[2]);
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+8+3] = -weight[i][pxl_map[k][v*biWidth[k]+u]]*(derive_x[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[1]+derive_y[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[0])/(cmpt[2]*cmpt[2]);
									break;
								default:
									{}
								}																
							}							
						}
						else//如果第k幅视图中的第(u,v)像素对应的网格模型顶点在第i幅视图中不可见呢？当然对其参数求导都是0了
						{
							for(m = 0; m < 12; m++)
							{
								Jac_Mat[counted_row+cur_cnt_fun][counted_col+m] = 0;
							}
								
						}
						counted_col += 12;//完成对与当前遍历视图对应的投影矩阵的12个分量的求导
					}					
					cur_cnt_fun++;//记录已计算的与当前视图相关的误差函数
				}
			}
		}
		counted_row += map_pxl_num[k];//完成对与当前遍历视图对应的所有误差函数的求导
	}
}

void COGLEventDoc::Err_Fun_Computation(CVisDMatrix& cur_fun_vct)
{
	int k, u, v;
	int counted_row = 0, cur_cnt_fun;//, cur_sum_fun;//记录在遍历过程中每轮遍历开始前已计算的误差函数，参数和当前这一轮中已计算的误差函数的个数
	
	//在构造出Jacobian矩阵的基础上，可以编写LM算法的代码
	double cur_gray;//当前遍历像素的灰度值
	//构造误差函数向量
	//初始化
	counted_row = 0;
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;//初始化
		for(v = 0; v < biHeight[k]; v++)//遍历行
		{
			for(u = 0; u < biWidth[k]; u++)//遍历列
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//对一个拥有对应网格顶点的像素构造其在Jacaobian Matrix中的相应元素
				{
					//计算该像素灰度与其对应的网格顶点的颜色信息之差的平方
					cur_gray = view_data[k][v*biAlign[k]+3*u+2] * 0.3 + view_data[k][v*biAlign[k]+3*u+1] * 0.59 + view_data[k][v*biAlign[k]+3*u] * 0.11;										
					//计算误差函数的数值，注意平方
					cur_fun_vct[counted_row+cur_cnt_fun][0] = cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]];//(cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]) * (cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]);
					cur_cnt_fun++;
				}
			}
		}
		counted_row += map_pxl_num[k];
	}
}

void COGLEventDoc::Sum_Fun_Computation(double& cur_sum_fun)
{
	int k, u, v, cur_cnt_fun;
	
	//在构造出Jacobian矩阵的基础上，可以编写LM算法的代码
	double cur_gray;//当前遍历像素的灰度值
	//构造误差函数向量
	//初始化
	cur_sum_fun = 0;
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;//初始化
		for(v = 0; v < biHeight[k]; v++)//遍历行
		{
			for(u = 0; u < biWidth[k]; u++)//遍历列
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//对一个拥有对应网格顶点的像素构造其在Jacaobian Matrix中的相应元素
				{
					//计算该像素灰度与其对应的网格顶点的颜色信息之差的平方
					cur_gray = view_data[k][v*biAlign[k]+3*u+2] * 0.3 + view_data[k][v*biAlign[k]+3*u+1] * 0.59 + view_data[k][v*biAlign[k]+3*u] * 0.11;										
					//计算当前误差和函数
					cur_sum_fun += (cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]) * (cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]);					
				}
			}
		}
	}
}

void COGLEventDoc::OnPxlComp() 
{
	int i, j, k, u, v, cnt = 0;//, l, m, n;//, u_k, v_k;
	
	//int** map_num = new int*[num_view];//记录每个像素对应的网格顶点的个数
	r3 = new CVisDVector[num_view];//记录对应于各试图的旋转矩阵的第三行
	pxl_map = new int*[num_view];//记录在各视图中与各像素对应的网格顶点的序号，若无对应则记录为-1
	vtx_map = new int**[num_view];//记录各网格顶点在各视图中对应的像素的坐标
	map_pxl_num = new unsigned long[num_view];//记录在各试图中有对应网格顶点的像素的个数
		
	//分配内存空间及初始化
	for(k = 0; k < num_view; k++)
	{
		r3[k].Resize(3);//分配为三元素向量
		map_pxl_num[k] = 0;//初始化是一定要注意的
		vtx_map[k] = new int*[M];
		for(i = 0; i < M; i++)
		{
			vtx_map[k][i] = new int[2];
			vtx_map[k][i][0] = vtx_map[k][i][1] = -1;
		}
	}

	for(k = 0; k < num_view; k++)
	{
		pxl_map[k] = new int[biHeight[k]*biWidth[k]];
		//map_num[k] = new int[biHeight[k]*biWidth[k]];
		
		//初始化
		for(v = 0; v < biHeight[k]; v++)
		{
			for(u = 0; u < biWidth[k]; u++)
			{
				pxl_map[k][v*biWidth[k]+u] = -1;
				//map_num[k][v*biWidth[k]+u] = 0;
			}			
		}
	}
	
	//构造Jacobian Matrix
	CVisDMatrix Jac_Mat(1, 12*num_view);//行数是误差函数的个数；列数是变量的个数，亦即与各视图对应的投影矩阵的各元素
	
	//在构造出Jacobian矩阵的基础上，可以编写LM算法的代码
	double para, incr_fct, err, pre_sum_fun, cur_sum_fun;//参数阿尔法，增长因子贝塔，控制误差及误差和函数
	//CVisDMatrix pre_fun_vct(1, 1);//误差函数构成的向量
	CVisDMatrix cur_fun_vct(1, 1);//误差函数构成的向量
	CVisDMatrix proj_para(12*num_view, 1);//投影矩阵参数构成的向量
	CVisDMatrix cur_proj_para(12*num_view, 1);//投影矩阵参数构成的向量
	CVisDMatrix drt_vct(12*num_view, 1);//方向向量，注意其维数是所有投影矩阵参数个数之和
	CVisDMatrix unit_mat(12*num_view, 12*num_view);//单位矩阵
	CVisDMatrix grad_vct(12*num_view, 1);//1/2梯度向量
	//double cur_gray;//当前遍历像素的灰度值
	double vct_lg;//记录向量的模长
	bool stop = FALSE;//标记参数调整是否结束

	FILE* fp;
			
	deno_of_weight = new double[M];//记录网格顶点对应于各视图的权值的公共分母
	weight = new double*[num_view];//记录网格顶点对应于各视图的权值
	for(k = 0; k < num_view; k++)
	{
		weight[k] = new double[M];
	}
	vtx_map_gray = new double*[num_view];//记录网格顶点在各视图中对应像素处的灰度
	for(k = 0; k < num_view; k++)
	{
		vtx_map_gray[k] = new double[M];
	}
	derive_x = new double*[num_view];//记录网格顶点在各视图中对应像素处沿X方向的偏导
	for(k = 0; k < num_view; k++)
	{
		derive_x[k] = new double[M];
	}
	derive_y = new double*[num_view];//记录网格顶点在各视图中对应像素处沿Y方向的偏导
	for(k = 0; k < num_view; k++)
	{
		derive_y[k] = new double[M];
	}
	vtx_gray = new double[M];//记录网格顶点对应的灰度信息
	vis_img = new bool*[num_view];//记录与当前所遍历像素对应的网格顶点在哪些视图中可见，切记每次便利都要重新初始化！
	for(k = 0; k < num_view; k++)
	{
		vis_img[k] = new bool[M];
	}

	//pst.Resize(3);
	//nml.Resize(3);
	
	//初始化
	//构造投影矩阵参数向量
	for(k = 0; k < num_view; k++)
	{
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < 4; j++)
			{
				cur_proj_para[12*k+4*i+j][0] = proj_para[12*k+4*i+j][0] = inter_mat_list[k][i][j];
			}
		}
	}
	para = 0.01;
	incr_fct = 10;
	err = 5000;
	
	//老大，首先得要计算相关数据啊，怎么把这步给扔掉了:(
	Mapping();
	Data_Computation();
	
	/*
	CFileDialog dlg( FALSE, ".txt", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("*.txt|*.txt|"));
	if( dlg.DoModal()==IDOK )
	{
		CString cstrFileName = dlg.GetPathName();	
		CFile f;
		CFileException ex;
		if( f.Open( cstrFileName, CFile::modeCreate | CFile::modeWrite, &ex ) )
		{	
			f.Close();
			//先考虑最简单的文件形式
			//开始写入数据
			FILE* fp;
			fp = fopen(cstrFileName, "w");
			//写入顶点坐标数据
			for(i = 0; i < M; i++)
			{	
				//fprintf(fp, "%lf\n", vtx_gray[i]);
				for(j = 12*1; j < 12*2; j++)
				{
					if(j == 12*num_view-1)
						fprintf(fp, "%lf\n", Jac_Mat[i][j]);
					else
						fprintf(fp, "%lf\t", Jac_Mat[i][j]);
				}
			}
			fclose(fp);				
		}
		else
		{
			// Complain if an error happened.
			// There is no reason to delete the ex object.
			
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			cout << "Could not open the file: ";
			cout << szError;
			return;
		}		
	}
	else return;
	*/
	
	//计算误差和函数
	Sum_Fun_Computation(cur_sum_fun);

	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
	
	fseek(fp, 0, SEEK_END);
	
	//写入顶点坐标数据
	fprintf(fp, "%lf\n", cur_sum_fun);				
	
	fclose(fp);	
	
	pre_sum_fun = cur_sum_fun;
	
	para /= incr_fct;
	
	//构造误差函数向量
	cur_fun_vct.Resize(map_pxl_num_sum, 1);
	Err_Fun_Computation(cur_fun_vct);
	
	Jac_Mat.Resize(map_pxl_num_sum, 12*num_view);
	Jacobian_Construction(Jac_Mat);//构造Jacobian矩阵
	
	//VisSetToIdentity(unit_mat);//设为单位矩阵
	
	for(i = 0; i < 12*num_view; i++)
	{
		for(j = 0; j < 12*num_view; j++)
		{
			if(i == j)	
				unit_mat[i][i] = para;//对角线上元素设置为para，相当于para*unit_mat
			else
				unit_mat[i][i] = 0;
		}
	}
	
	drt_vct = -(Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);

	//更新投影矩阵参数
	cur_proj_para += drt_vct;

	grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//计算梯度向量

	//计算梯度向量的模长
	vct_lg = 0;//初始化
	for(i = 0; i < 12*num_view; i++)
	{		
		vct_lg += grad_vct[i][0]*grad_vct[i][0];
	} 
	vct_lg = sqrt(vct_lg);

	//还要计算参数更新后的误差和函数
	//用更新的投影矩阵参数cur_proj_para来更新投影矩阵inter_mat_list,这里首先是为了计算cur_sum_fun
	for(k = 0; k < num_view; k++)
	{
		for(i = 0; i < 3; i++)
		{
			for(j = 0; j < 4; j++)
			{
				inter_mat_list[k][i][j] = cur_proj_para[12*k+4*i+j][0];
			}
		}
	}

	Mapping();//当然应当先更新对应关系，再根据这一对应关系来计算相关参数
	Data_Computation();
	
	//计算误差和函数
	Sum_Fun_Computation(cur_sum_fun);
	
	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
	fseek(fp, 0, SEEK_END);
	
	//写入顶点坐标数据
	fprintf(fp, "%lf\n", cur_sum_fun);
	
	fprintf(fp, "%d\n", map_pxl_num_sum);
			
	fclose(fp);
				
	cnt++;
		
	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
	fseek(fp, 0, SEEK_END);		
	//写入顶点坐标数据
	fprintf(fp, "%d\n", cnt);	
	fclose(fp);
	
	while(!stop)//cnt < 1000)
	{
		while(cur_sum_fun < pre_sum_fun)//误差和减小
		{
			//的确需要更新proj_para
			proj_para = cur_proj_para;
				
			if(vct_lg <= err)//梯度小于允许误差，投影参数调整结束
			{
				stop = TRUE;				
				break;
			}
			else
			{
				pre_sum_fun = cur_sum_fun;
				
				para /= incr_fct;
				
				//构造误差函数向量
				cur_fun_vct.Resize(map_pxl_num_sum, 1);
				Err_Fun_Computation(cur_fun_vct);
				
				Jac_Mat.Resize(map_pxl_num_sum, 12*num_view);
				Jacobian_Construction(Jac_Mat);//构造Jacobian矩阵
				
				if(cnt == 13)
				{
					/*
					CFileDialog dlg( FALSE, ".txt", NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
						_T("*.txt|*.txt|"));
					if( dlg.DoModal()==IDOK )
					{
						CString cstrFileName = dlg.GetPathName();	
						CFile f;
						CFileException ex;
						if( f.Open( cstrFileName, CFile::modeCreate | CFile::modeWrite, &ex ) )
						{	
							f.Close();
							//先考虑最简单的文件形式
							//开始写入数据
							FILE* fp;
							fp = fopen(cstrFileName, "w");
							//写入顶点坐标数据
							for(i = 0; i < M; i++)
							{	
								//fprintf(fp, "%lf\n", vtx_gray[i]);
								for(j = 12*1; j < 12*2; j++)
								{
									if(j == 12*num_view-1)
										fprintf(fp, "%lf\n", Jac_Mat[i][j]);
									else
										fprintf(fp, "%lf\t", Jac_Mat[i][j]);
								}
							}
							fclose(fp);				
						}
						else
						{
							// Complain if an error happened.
							// There is no reason to delete the ex object.
							
							TCHAR szError[1024];
							ex.GetErrorMessage(szError, 1024);
							cout << "Could not open the file: ";
							cout << szError;
							return;
						}		
					}
					else return;
					*/
				}
				//VisSetToIdentity(unit_mat);//设为单位矩阵
				
				for(i = 0; i < 12*num_view; i++)
				{
					for(j = 0; j < 12*num_view; j++)
					{
						if(i == j)	
							unit_mat[i][i] = para;//对角线上元素设置为para，相当于para*unit_mat
						else
							unit_mat[i][i] = 0;
					}
				}

				if(cnt == 93)
				{
					//解方程
					CVisDMatrix itmd(12*num_view, 12*num_view);
					itmd = Jac_Mat.Transposed()*Jac_Mat;
					
					fp = fopen("G:\\Data\\research\\exhibition\\skull\\output.txt", "r+");
					
					fseek(fp, 0, SEEK_END);					
					
					//写入顶点坐标数据
					for(i = 0; i < 12*num_view; i++)
					{	
						//fprintf(fp, "%lf\n", vtx_gray[i]);
						for(j = 0; j < 12*num_view; j++)
						{
							if(j == 12*num_view-1)
								fprintf(fp, "%lf\n", itmd[i][j]);
							else
								fprintf(fp, "%lf\t", itmd[i][j]);
						}
					}

					fclose(fp);				
						
					itmd += unit_mat;
					itmd = itmd.Inverted();	
				}
				
				//解方程
				drt_vct = -(Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);
				
				//更新投影矩阵参数
				cur_proj_para += drt_vct;
				
				grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//计算梯度向量

				//计算梯度向量的模长	
				vct_lg = 0;//初始化
				for(i = 0; i < 12*num_view; i++)
				{		
					vct_lg += grad_vct[i][0]*grad_vct[i][0];
				} 
				vct_lg = sqrt(vct_lg);

				//还要计算参数更新后的误差和函数
				//用更新的投影矩阵参数proj_para来更新投影矩阵inter_mat_list
				for(k = 0; k < num_view; k++)
				{
					for(i = 0; i < 3; i++)
					{
						for(j = 0; j < 4; j++)
						{
							inter_mat_list[k][i][j] = cur_proj_para[12*k+4*i+j][0];
						}
					}
				}
				Mapping();//当然应当先更新对应关系，再根据这一对应关系来计算相关参数
				Data_Computation();
				//计算误差和函数
				Sum_Fun_Computation(cur_sum_fun);

				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
				fseek(fp, 0, SEEK_END);
	
				//写入顶点坐标数据
				fprintf(fp, "%lf\n", cur_sum_fun);				
				
				fprintf(fp, "%d\n", map_pxl_num_sum);
			
				fclose(fp);

				cnt++;
				
				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
				fseek(fp, 0, SEEK_END);		
				//写入顶点坐标数据
				fprintf(fp, "%d\n", cnt);	
				fclose(fp);	

				if(cnt == 5 || cnt == 13)
				{
					cnt = cnt;
				}
			}
		}

		while(cur_sum_fun >= pre_sum_fun)//误差和未减小
		{
			//这是并不更新任何数据,所以投影矩阵也要还原
			for(k = 0; k < num_view; k++)
			{
				for(i = 0; i < 3; i++)
				{
					for(j = 0; j < 4; j++)
					{
						inter_mat_list[k][i][j] = proj_para[12*k+4*i+j][0];
					}
				}
			}

			cur_proj_para = proj_para;

			if(vct_lg <= err)//梯度小于允许误差，投影参数调整结束
			{
				stop = TRUE;				
				break;
			}
			else
			{
				//这里并不需要更新
				//pre_sum_fun = cur_sum_fun;

				para *= incr_fct;

				//VisSetToIdentity(unit_mat);//设为单位矩阵
				
				for(i = 0; i < 12*num_view; i++)
				{
					for(j = 0; j < 12*num_view; j++)
					{
						if(i == j)	
							unit_mat[i][i] = para;//对角线上元素设置为para，相当于para*unit_mat
						else
							unit_mat[i][i] = 0;
					}
				}				
				
				if(cnt == 93)
				{
					//解方程
					CVisDMatrix itmd(12*num_view, 12*num_view);
					itmd = Jac_Mat.Transposed()*Jac_Mat;
					
					fp = fopen("G:\\Data\\research\\exhibition\\skull\\output.txt", "r+");
					
					fseek(fp, 0, SEEK_END);					
					
					//写入顶点坐标数据
					for(i = 0; i < 12*num_view; i++)
					{	
						//fprintf(fp, "%lf\n", vtx_gray[i]);
						for(j = 0; j < 12*num_view; j++)
						{
							if(j == 12*num_view-1)
								fprintf(fp, "%lf\n", itmd[i][j]);
							else
								fprintf(fp, "%lf\t", itmd[i][j]);
						}
					}

					fclose(fp);				
						
					itmd += unit_mat;
					itmd = itmd.Inverted();	

					drt_vct = Jac_Mat.Transposed()*cur_fun_vct;
					drt_vct = -itmd*drt_vct;
				}

				//解方程
				else drt_vct = (Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);
				
				//更新投影矩阵参数
				cur_proj_para += drt_vct;
				
				/*
				//由于Jac_Mat和cur_fun_vct都没有更新，所以不必重新计算
				grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//计算梯度向量

				//计算梯度向量的模长	
				vct_lg = 0;//初始化
				for(i = 0; i < 12*num_view; i++)
				{		
					vct_lg += grad_vct[i][0]*grad_vct[i][0];
				} 
				vct_lg = sqrt(vct_lg);
				*/

				//还要计算参数更新后的误差和函数
				//用更新的投影矩阵参数proj_para来更新投影矩阵inter_mat_list
				for(k = 0; k < num_view; k++)
				{
					for(i = 0; i < 3; i++)
					{
						for(j = 0; j < 4; j++)
						{
							inter_mat_list[k][i][j] = cur_proj_para[12*k+4*i+j][0];
						}
					}
				}
				
				Mapping();//当然应当先更新对应关系，再根据这一对应关系来计算相关参数
				Data_Computation();
				//计算误差和函数
				Sum_Fun_Computation(cur_sum_fun);

				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
				fseek(fp, 0, SEEK_END);
	
				//写入顶点坐标数据
				fprintf(fp, "%lf\n", cur_sum_fun);				
				
				fprintf(fp, "%d\n", map_pxl_num_sum);
			
				fclose(fp);
				
				cnt++;
				
				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
				fseek(fp, 0, SEEK_END);		
				//写入顶点坐标数据
				fprintf(fp, "%d\n", cnt);	
				fclose(fp);	

				if(cnt == 13)
				{
					cnt = cnt;
				}
			}
		}		
	}
}

void COGLEventDoc::OnAreaCmpt() 
{
	double com1, com2, com3, area;
	double x[4], y[4], z[4];
	int i, j;
	area = 0;
	for(i = 0; i < L1; i++)
	{
		//读出三角形的三个顶点及其法向量					
		for(j = 0; j < 3; j++)
		{
			x[j+1] = face1_points[triangles1[i][j]][0];
			y[j+1] = face1_points[triangles1[i][j]][1];
			z[j+1] = face1_points[triangles1[i][j]][2];				
		}
		com1 = (x[3]-x[2])*(x[1]-x[2])+(y[3]-y[2])*(y[1]-y[2])+(z[3]-z[2])*(z[1]-z[2]);
		com2 = (x[1]-x[3])*(x[1]-x[2])+(y[1]-y[3])*(y[1]-y[2])+(z[1]-z[3])*(z[1]-z[2]);
		com3 = (x[1]-x[2])*(x[1]-x[2])+(y[1]-y[2])*(y[1]-y[2])+(z[1]-z[2])*(z[1]-z[2]);
		x[0] = (x[2]*com2 +x[1]*com1)/com3;
		y[0] = (y[2]*com2 +y[1]*com1)/com3;
		z[0] = (z[2]*com2 +z[1]*com1)/com3;
		area += sqrt(((x[3]-x[0])*(x[3]-x[0])+(y[3]-y[0])*(y[3]-y[0])+(z[3]-z[0])*(z[3]-z[0]))*((x[2]-x[1])*(x[2]-x[1])+(y[2]-y[1])*(y[2]-y[1])+(z[2]-z[1])*(z[2]-z[1])));
	}
	area = area/2;
	Area_cmpt area_cmpt;
	area_cmpt.m_area_value.Format("%.3lf", area);
	if(area_cmpt.DoModal() == IDOK)
	{
	}
}


void COGLEventDoc::OnVlmnCmpt() 
{
	double vlmn;
	double x[4], y[4], z[4];
	int i, j;
	vlmn = 0;
	for(i = 0; i < L1; i++)
	{
		//读出三角形的三个顶点及其法向量					
		for(j = 0; j < 3; j++)
		{
			x[j+1] = face1_points[triangles1[i][j]][0];
			y[j+1] = face1_points[triangles1[i][j]][1];
			z[j+1] = face1_points[triangles1[i][j]][2];				
		}
		vlmn += (-x[3]*y[2]*z[1]+x[2]*y[3]*z[1]+x[3]*y[1]*z[2]-x[1]*y[3]*z[2]-x[2]*y[1]*z[3]+x[1]*y[2]*z[3]);
	}
	vlmn = fabs(vlmn);
	Vlmn_cmpt vlmn_cmpt;
	vlmn_cmpt.m_vlmn_value.Format("%.3lf", vlmn);
	if(vlmn_cmpt.DoModal() == IDOK)
	{
	}
	
}
