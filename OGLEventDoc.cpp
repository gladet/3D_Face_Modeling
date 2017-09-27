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

	img_pt_num = 0;//��ʼ��ʱ�ͻ����OnLButtonUp����

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
	which_loc = 1;//��ʼ��Ϊλ�˵���ǰ�ĵ�궨

	width = height = 0;

	cylindrical_map = false;//��ʶ�Ƿ��Ѿ�����������ͼ
	txt_mapped = false;//��ʶ�Ƿ���ʾ����ͼ
	mesh_projed = false;//��־�Ƿ���ʾ����ͶӰ

	with_txt = true;//Ĭ��obj�ļ����Ѹ���������Ϣ��Ҳ�����Ժ����书��
	txt_type = false;//��Ը�����ɫ��Ϣ��obj�ļ����ڶ���������������Ӧ����ɫֵ//true;//Ĭ��obj�ļ��и���������Ϣ�������������ʾ��

	m_pBitmap = NULL;
	m_pBkgd = NULL;
	m_pTransfered = NULL;
	texture_map = NULL;
	txt_map = NULL;
	txt_tris = NULL;
	
	rot_matrix.Resize(3, 3);//���ڼ�¼����λ�˵���������������ת�������ۻ�
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

	map_pxl_num_sum = 0;//��ʼ��

	//��ʼ��Ϊ��ָ��
	view_data = NULL;//�洢����ͼͼ�����ݵ�ָ�룬ʹ����������ڲ�����
	biHeight = NULL;//�洢����ͼͼ��ĸ߶�
	biWidth = NULL;//�洢����ͼͼ��Ŀ��
	biAlign = NULL;

	//vtx_clr = new double[M];//��¼���񶥵��Ӧ�ĻҶ���Ϣ

	/*
	//��ʱM��δ��ʼ����
	deno_of_weight = new double[M];//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ�Ĺ�����ĸ
	weight = new double*[num_view];//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ
	for(int k = 0; k < num_view; k++)
	{
		weight[k] = new double[M];
	}
	vtx_map_gray = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش��ĻҶ�
	for(k = 0; k < num_view; k++)
	{
		vtx_map_gray[k] = new double[M];
	}
	derive_x = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���X�����ƫ��
	for(k = 0; k < num_view; k++)
	{
		derive_x[k] = new double[M];
	}
	derive_y = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���Y�����ƫ��
	for(k = 0; k < num_view; k++)
	{
		derive_y[k] = new double[M];
	}
	vtx_gray = new double[M];//��¼���񶥵��Ӧ�ĻҶ���Ϣ
	vis_img = new bool*[num_view];//��¼�뵱ǰ���������ض�Ӧ�����񶥵�����Щ��ͼ�пɼ����м�ÿ�α�����Ҫ���³�ʼ����
	for(k = 0; k < M; k++)
	{
		vis_img[k] = new bool[M];
	}
	*/
}

COGLEventDoc::~COGLEventDoc()
{
	int i;
	//�������ԭ�ȴ洢�������е����ݣ��ͷ��ڴ�ռ�
	//��ն��㼰��������Ϣ
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
	
	//�������������Ϣ
	if(texture_map != NULL)
	{
		for(i = 0; i < K1; i++)
		{
			delete []texture_map[i];
		}
		delete []texture_map;
	}
	
	//�����������Ϣ����������ͷ�������
	for(i = 0; i < L1; i++)
	{
		delete []triangles1[i];
		delete []nml_tris[i];			
	}
	delete []triangles1;
	delete []nml_tris;
	
	if(txt_tris != NULL)//with_txt)//����obj�ļ���������������������Ϣ����Ӧ�����������ϢҲҪ���
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
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
	for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	img_pts.erase(img_pts.begin(), img_pts.end());
	for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
	for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
	
	if(txt_map != NULL)//txt_map����ʾ��ǰ��ͼ��Ӧ������������texture_map����ʾ�ںϵĻ�ԭobj�ļ��и���������ͼ
	{
		for(i = 0; i < K1; i++)//��δִ��Txt_Mapping��K1 = 0��Ҳ��һ��
		{
			delete []txt_map[i];
		}
		delete []txt_map;
	}
	
	//���Ѿ�ӳ����������ͼ��Ҫ��һ�����Ӧ�������������
	for(dbpt_itrt_2 = face1_texture.begin(); dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2)
	{
		if((*dbpt_itrt_2) != NULL)//��ʵֻҪ��һ������Ĺ�����̣���֪�������ж����������
		{
			for(i = 0; i < K1; i++)
			{
				if((*dbpt_itrt_2)[i] != NULL)
					delete [](*dbpt_itrt_2)[i];//ÿ��ָ��ָ��һ����ά��������
			}
			delete [](*dbpt_itrt_2);
		}
	}
	//�������
	face1_texture.erase(face1_texture.begin(), face1_texture.end());
	
	//������ͼ��Ӧ�Ŀ����ڻ�ͼ���
	for(dbpt_itrt = view_inner.begin(); dbpt_itrt != view_inner.end(); ++dbpt_itrt)
	{
		if((*dbpt_itrt) != NULL)
			delete [](*dbpt_itrt);		
	}
	//�������
	view_inner.erase(view_inner.begin(), view_inner.end());
	
	//free(m_pTransfered);
	if(m_pTransfered != NULL)//��m_pTransfered���ڴ��������OnTxtBlend��������ɵ�
		delete []m_pTransfered;//��Ϊ����new��ʽ�����ڴ��
	
	for(itrt_charpt = view_pt.begin(); itrt_charpt != view_pt.end(); ++itrt_charpt)
	{
		if((*itrt_charpt) != NULL)
		{
			if(*itrt_charpt != m_pBitmap)//��m_pBitmap��ָͬ����ڴ�ռ佻��m_pBitmap�������ͷ�
			{
				free(*itrt_charpt);
				(*itrt_charpt) = NULL;//����ͼָ�븳Ϊ��
			}
		}
	}
	//ֱ�����������
	view_pt.erase(view_pt.begin(), view_pt.end());
	if(m_pBitmap != NULL)
	{
		free(m_pBitmap);//��Ϊm_pBitmap����ͨ������ReadBitmap�е�malloc����ֵ��
		//��Ϊ��û�ж�����ڴ���䣬���Բ����ͷ�
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
	
	//��ʼ��
	for(j = 0; j < 3; j++)
	{
		cen_point1[j][0] = 0;
	}	
	
	//������
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			//���. cgi[]��������
			cen_point1[j][0] = cen_point1[j][0] + face1_points[i][j];
		}
	}
	
	//ȡƽ��ֵ
	cen_point1 /= (double)M;	
	
	//ƽ��������ԭ�㴦
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
	//����ͷŵ����ڴ棬���޷��������ں�����
	else
	{
		//free(buffer);//�����ǲ�Ӧ���ͷŵ�?
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
	                 _T("obj�ļ�(*.obj)|*.obj|"));
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
	char str[1000];//��֪����󳤶ȿ��ܴﵽ��󣬱��������
	list_dbpt list_points, list_normals, list_txt;
	list_intpt list_tris, list_nml_tris, list_txt_tris;
	//������������������
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator itrt_intpt;	
	double* temp_dbpt;
	int* temp_intpt, *temp_intpt_1, *temp_intpt_2;
	//��ȷ��ʼ��
	int sum_points = 0;
	int sum_normals = 0;
	int sum_txt = 0;
	int sum_tris = 0;
	int end_of_file = 0;//�ж��ļ��Ƿ����
	int tris_begin = 0;
	//��Ϊһ����������
	//int with_txt = 1;//Ĭ�ϸ���������Ϣ

	fp = fopen(cstrFileName, "r+");
	if(fp == NULL)
	{
		AfxMessageBox("can not open the file");
		return false;
	}
	fseek(fp, 0L, SEEK_SET);
	while(fscanf(fp, "%s", &str) != -1)
	{
		//���붥����Ϣ
		while(str[0] == 'v' && str[1] == 0 && end_of_file != -1)
		{
			//if(sum_points == 11994)
				//sum_points = 11994;
			sum_points++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);//����һ��������
				temp_dbpt[j] = temp;
			}
			list_points.insert(list_points.end(),temp_dbpt);

			//��Ը�����ɫ��Ϣ��obj�ļ����ڶ���������������Ӧ����ɫֵ
			sum_txt++;
			temp_dbpt = new double[3];
			for(j = 0; j < 3; j++)
			{
				fscanf(fp, "%lf", &temp);//����һ��������
				temp_dbpt[j] = temp;
			}
			list_txt.insert(list_txt.end(),temp_dbpt);
			//delete []temp_dbpt;
			end_of_file = fscanf(fp, "%s", &str);//���Ƕ��س�����ʵ�Ƕ�����һ���ַ��������������ж��������������͵�����
			if(sum_points == 23195)
			{
				sum_points = sum_points;
			}				
		}
		
		//���붥�㷨������Ϣ
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
			temp_dbpt = new double[2];//�ǲ���Ӧ��Ϊ2��
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
			if(tris_begin == 0 || with_txt == 1)//with_txt��¼�Ƿ񸽴�����������Ϣ��Ĭ���Ǹ���������Ϣ��
				temp_intpt_1 = new int[3];//�����ڼ�¼��ǰ�������ϵĸ�������������
			temp_intpt_2 = new int[3];//�����ڼ�¼��ǰ�������ϵĸ����㷨���������
			
			if(tris_begin == 0)
			{
				tris_begin = 1;
				fscanf(fp, "%d", &temp_index);
				temp_intpt[0] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
				fscanf(fp, "%c", &c);//c = '/'
				pre_fp = fp;
				fscanf(fp, "%c", &c);//δ֪��ṹ
				if(c == '/')//���������б��
				{
					//with_txt = false;//û������������Ϣ
					fscanf(fp, "%d", &temp_index);
					temp_intpt_2[0] = temp_index - 1;
					for(j = 1; j < 3; j++)
					{					
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);	
						temp_intpt_2[j] = temp_index - 1;//��¼��ǰ������Ƭ�����㷨�����������еı��
					}
				}
				else
				{
					fp = pre_fp;//�˻ص���һ������λ��
					fscanf(fp, "%d", &temp_index);
					temp_intpt_1[0] = temp_index - 1;//��¼��ǰ������Ƭ�����������������еı��				
					fscanf(fp, "%c", &c);//c = '/'
					fscanf(fp, "%d", &temp_index);
					temp_intpt_2[0] = temp_index - 1;//��¼��ǰ������Ƭ�����㷨���������еı��
					for(j = 1; j < 3; j++)//����ʣ���������㼰������ͷ���������Ϣ
					{
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_1[j] = temp_index - 1;//��¼��ǰ������Ƭ�����������������еı��				
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_2[j] = temp_index - 1;//��¼��ǰ������Ƭ�����㷨���������еı��	
					}
					list_txt_tris.insert(list_txt_tris.end(),temp_intpt_1);//��������ϢҲ��������					
				}
				list_tris.insert(list_tris.end(),temp_intpt);	
				list_nml_tris.insert(list_nml_tris.end(),temp_intpt_2);
				end_of_file = fscanf(fp, "%s", &str);//�ļ������ѵ�ֻ�����������ε���Ϣ֮����
			}
			else//���ܼ���������ε���Ϣ�������ٶ���ʱ�Ͳ�����������ж���
			{
				if(with_txt)//��������01_15�Ķ���// == 0)
				{
					for(j = 0; j < 3; j++)
					{
						
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);	
						temp_intpt_2[j] = temp_index - 1;//��¼��ǰ������Ƭ�����㷨���������еı��	
					}
					list_tris.insert(list_tris.end(),temp_intpt);
					list_nml_tris.insert(list_nml_tris.end(),temp_intpt_2);					
					end_of_file = fscanf(fp, "%s", &str);
				}
				else//�������ʲô����أ�
				{
					for(j = 0; j < 3; j++)
					{
						fscanf(fp, "%d", &temp_index);
						temp_intpt[j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_1[j] = temp_index - 1;//��¼��ǰ������Ƭ�����������������еı��				
						fscanf(fp, "%c", &c);//c = '/'
						fscanf(fp, "%d", &temp_index);
						temp_intpt_2[j] = temp_index - 1;//��¼��ǰ������Ƭ�����㷨���������еı��	
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

	//����������Ķ�����Ϣ��������
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

	//����������Ķ��㷨������Ϣ��������
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

	//����������Ķ�������������Ϣ��������
	if(with_txt)
	{
		data_texture = new double*[sum_txt];
		if(txt_type)//��ȷ���������꣬����Ӧ"vt"
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
		else//����ɫֵ������Ӧ"vc"
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

	//�������������������Ϣ��������
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
		txt_tris = new int*[sum_tris];//�洢���������������Ӧ�������������ţ��ǲ��Ǳ�����������Ϣ��ִ����һ�����أ�	
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
	nml_tris = new int*[sum_tris];//�洢���������������Ӧ�ķ���������ţ���ʵһ�����붥�������ͬ��
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
	//�������
	list_points.erase(list_points.begin(), list_points.end());
	list_normals.erase(list_normals.begin(), list_normals.end());
	list_txt.erase(list_txt.begin(), list_txt.end());
	list_tris.erase(list_tris.begin(), list_tris.end());
	list_txt_tris.erase(list_txt_tris.begin(), list_txt_tris.end());
	list_nml_tris.erase(list_nml_tris.begin(), list_nml_tris.end());

	return true;//��ȷ������ģ��������Ϣ
}

/*void COGLEventDoc::ReadObj(double **& data_points, double **& data_normals, int **& triangles, int& num_points, int& num_tris)
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("obj�ļ�(*.obj)|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}

	FILE *fp;
	int i, j, temp_tri_index, temp_index;
	char c;
	double temp;
	char str[81];
	
	//��cstrFileName�еĵ㼰��Ӧ���������뵽�ڴ����鵱��
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
	fscanf(fp, "%c", &c);//c = �س�

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
		fscanf(fp, "%c", &c);//c = �س�
		
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
	fscanf(fp, "%c", &c);//c = �س�
	
	temp_tri_index = 0;
	while(temp_tri_index < num_tris)
	{
		triangles[temp_tri_index] = new int[3];

		fscanf(fp, "%c", &c);//c = 'f'

		for(j = 0; j < 3; j++)
		{
			fscanf(fp, "%d", &temp_index);
			triangles[temp_tri_index][j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
			fscanf(fp, "%c", &c);//c = '/'
			fscanf(fp, "%c", &c);//c = '/'
			fscanf(fp, "%d", &temp_index);			
		}

		fscanf(fp, "%c", &c);//c = �س�
		
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
	                 _T("obj�ļ�(*.obj)|*.obj|"));
	if( dlg.DoModal()==IDOK )
	{
		cstrFileName = dlg.GetPathName();		
	}

	FILE *fp;
	int i, j, temp_index;
	char c;
	double temp;
	char str[1000];//��֪����󳤶ȿ��ܴﵽ��󣬱��������
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
				temp_intpt[j] = temp_index - 1;//��¼��ǰ������Ƭ�������������еı��
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
		double length = 0;//��ʼ��
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
	
	//double ** temp_txt = new double*[K1 = M];//ʵ���������K1��M������Ӧ�����
	//double * temp_inner = new double[K1];
	
	//��ȡͼ�����ݲ���
	//��ȡ��ǰ��ͼָ��
	unsigned char* pBitmap = temp_view;//m_pBitmap;
	if(pBitmap == NULL)
		return;
	LPBITMAPINFO lpBitmapInfo = (LPBITMAPINFO)(pBitmap + 14);
	unsigned char* pBitmapData = pBitmap + ((LPBITMAPFILEHEADER)pBitmap)->bfOffBits;
	unsigned long biHeight = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biHeight;
	unsigned long biWidth = ((LPBITMAPINFOHEADER)lpBitmapInfo)->biWidth;
	
	//����任����ģ���ϱ궨��ʵ�ʶ�Ӧ��ͼ��㣬���ʼ�궨��ͼ���������бȽϣ��Լ������ڵ�����ӳ���Ƿ���ȷ
	list_dbpt::iterator dbpt_itrt, dbpt_itrt_1;
	list_intpt::iterator intpt_itrt;
	double temp_mdl_pt[3];
	//int *temp_img_pt;
	/*
	delete []temp_img_pt;//��ԭ�ȷ�����ڴ��ͷ�
	if(temp_mdl_pt != NULL)
	temp_mdl_pt = NULL;//ǰ��temp_mdl_pt�����ڹ�������������Ҫ���·����ڴ�ʹ��
	temp_mdl_pt = new double[2];
	*/
	
	for(i = 0; i < mdl_pt_num; i++)
	{
		//ֻҪ����ǰ����������ֵ
		for(j = 0; j < 3; j++)
		{
			temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//��û�н�ģ���ϱ�ǵ�ת����ʵ�ʳ߶�
		}		
		
		/*temp_img_pt = new int[2];
		//ֱ�Ӹ���ӳ��������ģ���ϱ�ǵ���ͼ����ʵ�ʶ�Ӧ������λ��
		temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][0][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][0][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][0][2]+inter_mat_list[iter_index-1][0][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][1][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][1][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][1][2]+inter_mat_list[iter_index-1][1][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		
		act_img_pts.insert(act_img_pts.end(), temp_img_pt);//��¼ʵ�ʵ�ͼ�������λ��*/

		act_img_pts_list[iter_index-1][i][1] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][0][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][0][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][0][2]+inter_mat_list[iter_index-1][0][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
		act_img_pts_list[iter_index-1][i][0] = int((temp_mdl_pt[0]*inter_mat_list[iter_index-1][1][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][1][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][1][2]+inter_mat_list[iter_index-1][1][3])/(temp_mdl_pt[0]*inter_mat_list[iter_index-1][2][0]+temp_mdl_pt[1]*inter_mat_list[iter_index-1][2][1]+temp_mdl_pt[2]*inter_mat_list[iter_index-1][2][2]+inter_mat_list[iter_index-1][2][3]));
	}	
	
	for(i = 0; i < K1; i++)
	{
		temp_txt[i] = new double[2];//�����ڴ�ռ�
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
		m3[j] = inter_mat_list[iter_index-1][2][j];//��1��3��Ԫ��
	}
	
	double m_34 = inter_mat_list[iter_index-1][2][3];;
	r3 = m3*(m_34>0?1:-1);
	
	/*
	//�����Ӧ�ڵ�ǰ��ͼ���ڻ�
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
	
	textured = true;//��ʾ�Ѿ�����������ͼ����Ȼֻ�ǵ�ǰ��ͼ�ģ���Ȼ����transformed�ǲ�ͬ��
	with_txt = false;//����ԭobj�ļ��и�����������Ϣ��Ҳ�Ѿ�����ʹ��֮
	
	//δ�ڶ��α�㣬����������//mdl_pt_num = img_pt_num = 0;
	
	//ֻҪCOGLEventView��Ӧ����ˢ�¼���
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	pView->Invalidate(FALSE);
	pView->UpdateData(FALSE);
	
	CTexture *pView1 = (CTexture *)GetTextureView();
	pView1->Invalidate(FALSE);
	pView1->UpdateData(FALSE);
}

void COGLEventDoc::OnReadModel() 
{
	if(!m_read)//��һ�ζ���ģ��
	{
		m_read = ReadObj_Extd(face1_points, face1_normals, texture_map, triangles1, nml_tris, txt_tris, M, M1, K1, L1, with_txt);		
		//if(!with_txt)//�ļ�û�и�����������
		{
			temp_txt = new double*[K1 = M];
			for(int i = 0; i < K1; i++)
				temp_txt[i] = new double[2];
		}
	}
	else
	{
		int i;
		//�������ԭ�ȴ洢�������е����ݣ��ͷ��ڴ�ռ�
		//��ն��㼰��������Ϣ
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

		//�������������Ϣ
		if(texture_map != NULL)
		{
			for(i = 0; i < K1; i++)
			{
				delete []texture_map[i];
			}
			delete []texture_map;
		}

		//�����������Ϣ����������ͷ�������
		for(i = 0; i < L1; i++)
		{
			delete []triangles1[i];
			delete []nml_tris[i];			
		}
		delete []triangles1;
		delete []nml_tris;

		if(with_txt)//����obj�ļ���������������������Ϣ
		{
			/*
			for(i = 0; i < L1; i++)
			{
				delete []txt_tris[i];
			}
			delete []txt_tris;
			*/
		}
		
		obj_fp_lc = img_fp_lc = true;//��ʼ�궨��δ���
		mdl_pt_num = img_pt_num = 0;//��ʼ�����궨�ĵ�������ʵͶӰӳ����Ѿ�������
	
		list_dbpt_2::iterator dbpt_itrt_2;
		list_dbpt::iterator dbpt_itrt;
		list_intpt::iterator intpt_itrt;
		list_charpt::iterator itrt_charpt;
		//���Ѿ�ӳ����������ͼ��Ҫ��һ�����Ӧ�������������
		for(dbpt_itrt_2 = face1_texture.begin(); dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2)
		{
			for(i = 0; i < K1; i++)
			{
				delete [](*dbpt_itrt_2)[i];
			}
			delete [](*dbpt_itrt_2);
		}
		//�������
		face1_texture.erase(face1_texture.begin(), face1_texture.end());
		
		//������ͼ��Ӧ�Ŀ����ڻ�ͼ���
		for(dbpt_itrt = view_inner.begin(); dbpt_itrt != view_inner.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);		
		}
		//�������
		view_inner.erase(view_inner.begin(), view_inner.end());

		/*
		//��һ������ʱ���ã��������ģ�Ͷ��������¶���ͼ���أ�ΪʲôҪ����ڴ棬����Ҫ������¶��룬��ReadBitmap��Ҳ����Ӧ�Ĵ�����
		if(m_pBitmap != NULL)
		{
			free(m_pBitmap);//��Ϊm_pBitmap����ͨ������ReadBitmap�е�malloc����ֵ��
			//��Ϊ��û�ж�����ڴ���䣬���Բ����ͷ�
			m_pBitmap = NULL;
		}
		*/
		//������ͼ���ݶ�Ӧ���ڴ����
		for(itrt_charpt = view_pt.begin(); itrt_charpt != view_pt.end(); ++itrt_charpt)
		{
			if((*itrt_charpt) != NULL)
			{
				if(*itrt_charpt != m_pBitmap)//m_pBitmap��ʱ���������
				{
					free(*itrt_charpt);
					(*itrt_charpt) = NULL;//����ͼָ�븳Ϊ��
				}
			}
		}
		//ֱ�����������
		view_pt.erase(view_pt.begin(), view_pt.end());

		textured = false;
		with_txt = true;
		txt_type = false;//true;
		
		transformed = false;

		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());	
		for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
		for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		img_pts.erase(img_pts.begin(), img_pts.end());	
		for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());	

		//������ڵ�����OnTxtBlend֮�������¶���ģ����Щ��������б�Ҫ��
		//free(m_pTransfered);
		if(m_pTransfered != NULL)//��m_pTransfered���ڴ��������OnTxtBlend��������ɵ�
			delete []m_pTransfered;//��Ϊ����new��ʽ�����ڴ��

		//����������ͼ�������������
		if(txt_map != NULL)
		{
			for(i = 0; i < K1; i++)
			{
				delete []txt_map[i];
			}
			delete []txt_map;
		}		

		cylindrical_map = false;
		
		//��ʾ������Ҫ���¶�������
		m_read = false;		

		K1 = 0;//����

		//���¶���ģ������
		m_read = ReadObj_Extd(face1_points, face1_normals, texture_map, triangles1, nml_tris, txt_tris, M, M1, K1, L1, with_txt);				

		if(!with_txt)//�ļ�û�и�����������
		{
			temp_txt = new double*[K1 = M];
			for(int i = 0; i < K1; i++)
				temp_txt[i] = new double[2];
		}
	}
	COGLEventView *pView = (COGLEventView *)GetOGLEventView();
	
	pView->Invalidate(FALSE);	
}

//��ģ���ϱ��������
void COGLEventDoc::OnPtLoc1() 
{
	//which_loc = 1;//�ڵڶ��μ�֮���λ�˵���ʱҪ��ʼ��

	obj_fp_lc = false;//��ʼ�궨��δ���
	mdl_pt_num = 0;//��ʼ�����궨�ĵ�������ʵͶӰӳ����Ѿ�������
	list_dbpt::iterator dbpt_itrt;
	list_intpt::iterator intpt_itrt;
	for(dbpt_itrt = mdl_pts.begin(); dbpt_itrt != mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	img_pts.erase(img_pts.begin(), img_pts.end());
	for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());

	//��Ϊact_mdl_pts��act_img_ptsֻ��txt_mapping�����вſ������±����ɣ������ڴ˴����
	for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
	for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
	{
		delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
}

//��ͼ���ϱ��������
void COGLEventDoc::OnPtLoc2() 
{
	//which_loc = 2;//��λ�˵�����ı궨
	
	//if(mdl_pt_num%8 == 0 && img_pt_num%8 == 0)//���㲻������һ�α궨���֮���Ҫ���±궨ҲӦ��������Ҳ������Ϊ�궨�����г��ִ�����޷�����
	{
		num_view++;
		temp_img_pts = new list_intpt[1];
		img_pts_list.insert(img_pts_list.end(), temp_img_pts);
		temp_img_pt_cd = new list_intpt[1];
		img_pt_cd_list.insert(img_pt_cd_list.end(), temp_img_pt_cd);//ע��temp_img_pt_cd��;��ͬ��temp_img_pts

		img_fp_lc = false;	
		img_pt_num = 0;
		temp_img_pt_num = 0;

		//��Ϊact_mdl_pts��act_img_ptsֻ��txt_mapping�����вſ������±����ɣ������ڴ˴����
		list_dbpt::iterator dbpt_itrt;
		list_intpt::iterator intpt_itrt;
		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());	
	}	
}

void COGLEventDoc::OnProjMap() 
{	
	if(transformed)//ͶӰӳ���ǰ���Ƕ�Ӧ�ڸ���ͼ��ӳ������Ѿ�����
	{
		//����û�еڶ��α�㣬ֱ�ӽ�������ӳ��
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
		double** txt_coord = new double*[num_view];//�洢��ǰ�����ڸ���ͼ�е���������
		for(j = 0; j < num_view; j++)
			txt_coord[j] = new double[2];			
		double* weight = new double[num_view];//�洢��ǰ�����ڸ���ͼ��Ӧ�ĵ�Ȩ����ֵ
		unsigned char** view_data = new unsigned char*[num_view];//�洢����ͼͼ�����ݵ�ָ�룬ʹ����������ڲ�����
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
		if(txt_map != NULL)//txt_map��û���ڸú���֮�ⱻ�����ڴ��
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
		if(texture_map != NULL)//��texture_map�ǿգ�ֻ�����Ƕ����obj�ļ��и�������������
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
		
		txt_map = new double*[K1];//��������
		texture_map = new double*[K1];//�������Ӧ����ɫ��ֵ���������texture_map�ĺ���������ͬ��֮ǰ�����ڼ�¼�������������ɫ��Ϣ
		int x, y;
		double y_min = face1_points[0][0];//��¼�����ϵ���Сֵ
		double y_max = face1_points[0][0];//��¼�����ϵ����ֵ

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
		
		int* cur = new int[num_view];//ָʾ�������������õ��ĵ�ǰ������ɫֵ�ڸ���ͼ�ж�Ӧ��λ��
		int u, v;
		
		//�������ϵ������Сֵ
		for(i = 0; i < M; i++)
		{
			if(face1_points[i][0] < y_min)
				y_min = face1_points[i][0];
			if(face1_points[i][0] > y_max)
				y_max = face1_points[i][0];
		}

		//����������������ͼ���ꡢ��ɫֵ
		for(i = 0; i < K1; i++)
		{
			//��z����ֵΪ������Ҫ���⴦��
			x = (int)width*((((face1_points[i][2] == 0)?((face1_points[i][1] >= 0)?PI/2:-PI/2):atan(face1_points[i][1]/-face1_points[i][2]))+PI/2)/(2*PI));
			y = (int)(height*(face1_points[i][0]-y_min)/(y_max-y_min));

			//���ڻ����㵱ǰ�����ڸ���ͼ�ж�Ӧ��Ȩ����ֵ
			for(dbpt_itrt = view_inner.begin(), j = 0; dbpt_itrt != view_inner.end(); ++dbpt_itrt, j++)
			{
				weight[j] = (*dbpt_itrt)[i]>0?(*dbpt_itrt)[i]:0;
			}	
			//��ǰ�����ڸ���ͼ�ж�Ӧ����������
			for(dbpt_itrt_2 = face1_texture.begin(), j = 0; dbpt_itrt_2 != face1_texture.end(); ++dbpt_itrt_2, j++)
			{
				txt_coord[j][0] = (*dbpt_itrt_2)[i][0];
				txt_coord[j][1] = (*dbpt_itrt_2)[i][1];
			}	

			//��ǰ�����ڸ���ͼ�ж�Ӧ����ɫֵ��λ��
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
				//��Ϊ���ۼӣ������ȳ�ʼ��
				if(j == 0)//�ڶ���һ��λͼʱ����
				{
					//������ڲ�ֹһ�������Ӧ��ͬһͼ��㣬����ܲ�ֹһ�α����
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

		cylindrical_map = true;//��ʶ��������������ͼ��������������ں�		
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
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
		for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
		for(intpt_itrt = img_pts.begin(); intpt_itrt != img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		img_pts.erase(img_pts.begin(), img_pts.end());
		for(intpt_itrt = img_pt_cd.begin(); intpt_itrt != img_pt_cd.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
		
		//��Ϊact_mdl_pts��act_img_ptsֻ��txt_mapping�����вſ������±����ɣ������ڴ˴����
		for(dbpt_itrt = act_mdl_pts.begin(); dbpt_itrt != act_mdl_pts.end(); ++dbpt_itrt)
		{
			delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_mdl_pts.erase(act_mdl_pts.begin(), act_mdl_pts.end());
		for(intpt_itrt = act_img_pts.begin(); intpt_itrt != act_img_pts.end(); ++intpt_itrt)
		{
			delete [](*intpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
		}
		act_img_pts.erase(act_img_pts.begin(), act_img_pts.end());
		
		CString cstrFileName;
		CFileDialog dlg( TRUE,NULL,NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("txt�ļ�(*.txt)|*.txt|"));
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

		//����ģ�����ѱ�ǵ���
		fscanf(fp, "%d", &num);
		mdl_pt_num = img_pt_num = num;
		fscanf(fp, "%c", &c);

		//����ģ�����ѱ�ǵ�
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

		//������ͼ��
		fscanf(fp, "%d", &num_view);
		fscanf(fp, "%c", &c);

		//�����ڸ���ͼ��ģ�����ѱ�ǵ�Ķ�Ӧ��
		for(k = 0; k < num_view; k++)
		{
			//���뵱ǰ��ͼ�Ͽɼ�����
			fscanf(fp, "%d", &temp_img_pt_num);
			img_pt_num_list.insert(img_pt_num_list.end(),temp_img_pt_num);
			fscanf(fp, "%c", &c);

			temp_img_pts = new list_intpt[1];//�����ڴ�

			img_pts_list.insert(img_pts_list.end(), temp_img_pts);//��������ָ�������

			for(i = 0; i < img_pt_num; i++)
			{
				temp_img_pt = new int[2];//�����ڴ�

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
			//�������
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

			Vis_Map = new bool*[mdl_pt_num];//������Ϊģ�����ѱ����
			for(i = 0; i < mdl_pt_num; i++)
				Vis_Map[i] = new bool[num_view];//������Ϊ��ͼ��

			Vis_Num = new int[mdl_pt_num];
			for(i = 0; i < mdl_pt_num; i++)
				Vis_Num[i] = 0;//��ʼ��

			img_pts_mat = new int**[mdl_pt_num];//������Ϊģ�����ѱ����
			for(i = 0; i < mdl_pt_num; i++)
				img_pts_mat[i] = new int*[num_view];//������Ϊ��ͼ��

			inter_mat_list = new CVisDMatrix[num_view];

			int cur_view;//��¼��ǰ��ͼ�����

			//���������ͼ��Ӧ��ӳ�����
			for(llippt_itrt = img_pt_cd_list.begin(), int_itrt = img_pt_num_list.begin(), cur_view = 0; llippt_itrt != img_pt_cd_list.end(), int_itrt != img_pt_num_list.end(); ++llippt_itrt, ++int_itrt, cur_view++)
			{
				temp_img_pt_num = (*int_itrt);

				//���ݵ�ǰ��ͼ��ʵ�ʱ�ǵ���ȷ�������С
				C.Resize(2*temp_img_pt_num, 9);
				D.Resize(2*temp_img_pt_num, 3);

				k = 0;
				for(dbpt_itrt = mdl_pts.begin(), intpt_itrt = (*llippt_itrt)[0].begin(), i = 0; dbpt_itrt != mdl_pts.end(), intpt_itrt != (*llippt_itrt)[0].end(), i < mdl_pt_num; ++dbpt_itrt, ++intpt_itrt, i++)
				{
					if((*intpt_itrt)[0] != -1)//��ǰ�������ڸ���ͼ�пɼ�
					{
						Vis_Map[i][cur_view] = true;
						Vis_Num[i]++;

						//ע������ʵ�ʺ�����y�����������x����
						img_pts_mat[i][cur_view] = new int[2];
						img_pts_mat[i][cur_view][0] = (*intpt_itrt)[0];
						img_pts_mat[i][cur_view][1] = (*intpt_itrt)[1];

						for(j = 0; j < 3; j++)
						{
							D[2*k][j] = -(*intpt_itrt)[0]*(*dbpt_itrt)[j]*Max_V;//-ui*Xi[j]
							//��ӡż���е�����
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
							//��ӡ�����е�����
							/*
							if(j != 2)
							fprintf(fp, "%f\t", D[2*i+1][j]);
							else 
							fprintf(fp, "%f\n", D[2*i+1][j]);
							*/
						}
						k++;//�����������
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
							C[2*k][j] = (*dbpt_itrt)[j]*Max_V;//��Ҫ�任��ʵ�ʵ�����ֵ
							//fprintf(fp, "%f\t", C[2*i][j]);
						}
						C[2*k][3] = 1;
						//fprintf(fp, "%f\t", C[2*i][3]);
						for(j = 0; j < 4; j++)
						{
							C[2*k][4+j] = 0;//�ӵ�5������8��Ԫ�ص�ȡֵ����0
							//fprintf(fp, "%f\t", C[2*i][4+j]);
						}
						C[2*k][8] = -(*intpt_itrt)[0];//-ui
						//fprintf(fp, "%f\n", C[2*i][8]);
						
						for(j = 0; j < 4; j++)
						{
							C[2*k+1][j] = 0;//�ӵ�1������4��Ԫ�ص�ȡֵ����0
							//fprintf(fp, "%f\t", C[2*i+1][j]);
						}
						for(j = 0; j < 3; j++)
						{
							C[2*k+1][4+j] = (*dbpt_itrt)[j]*Max_V;//��Ҫ�任��ʵ�ʵ�����ֵ
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
				//temp_inter_mat[0].Resize(3, 4);//��ʼ��ʱ��û�и�����С
				//inter_mat_list.insert(inter_mat_list.end(), temp_inter_mat);
				inter_mat_list[cur_view].Resize(3, 4);//��ʼ��ʱ��û�и�����С
				
				//���м��������ֵ
				/*
				for(j = 0; j < 3; j++)
				{
					temp_inter_mat[0][0][j] = y[j];//��1��3��Ԫ��
					temp_inter_mat[0][1][j] = y[4+j];//��5��7��Ԫ��
					temp_inter_mat[0][2][j] = z[j];//��1��3��Ԫ��
				}
				temp_inter_mat[0][0][3] = y[3];//��4Ԫ��
				temp_inter_mat[0][1][3] = y[7];//��8��Ԫ��
				temp_inter_mat[0][2][3] = y[8];//��9��Ԫ��
				*/
				for(j = 0; j < 3; j++)
				{
					inter_mat_list[cur_view][0][j] = y[j];//��1��3��Ԫ��
					inter_mat_list[cur_view][1][j] = y[4+j];//��5��7��Ԫ��
					inter_mat_list[cur_view][2][j] = z[j];//��1��3��Ԫ��
				}
				inter_mat_list[cur_view][0][3] = y[3];//��4Ԫ��
				inter_mat_list[cur_view][1][3] = y[7];//��8��Ԫ��
				inter_mat_list[cur_view][2][3] = y[8];//��9��Ԫ��
			}

			iter_index = num_view;
			temp_view = m_pBitmap;
			
			mdl_pts_array = new double*[mdl_pt_num];//��ģ���ϱ�ǵ�洢��������
			opt_mdl_pts = new double*[mdl_pt_num];
			act_img_pts_list = new int**[num_view];
				
			double temp_mdl_pt[3];
			int temp_img_pt[2];
			double pre_err_sum, temp_err_sum;
			
			double pre_err_sum_2, temp_err_sum_2;//��¼����ģ�͵���������ͼ��ʵ��ӳ������ʼ��ǵ�֮�����֮��
			
			list_db pts_err_sum_list;//��¼��δ���µ�ģ�͵��Ӧ������������������
			
			bool* pts_update = new bool[mdl_pt_num];//���ģ�͵��Ƿ��Ѹ���
			bool update_over;//��Ǹ����Ƿ����
			
			double** temp_opt_array = new double*[mdl_pt_num];//��¼�����µ�ģ�͵�
			
			double* pts_err_sum_array = new double[mdl_pt_num];//��¼�����µ�ģ�͵��Ӧ�������
			double* pts_err_sum_array_1 = new double[mdl_pt_num];//��¼����ǰ��ģ�͵��Ӧ�������
			
			int update_sum = 0;//��¼�Ѹ��µ�ģ�͵���
			
			//��ʼ��
			for(dbpt_itrt = mdl_pts.begin(), i = 0; dbpt_itrt != mdl_pts.end(), i < mdl_pt_num; ++dbpt_itrt, i++)
			{
				mdl_pts_array[i] = new double[3];
				for(j = 0; j < 3; j++)
					mdl_pts_array[i][j] = (*dbpt_itrt)[j];
				
				//�����ڴ�
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
			
			//obj_fp_lc = img_fp_lc = true;//��һ���Ѿ��궨����
			//��������ʾ���궨��
			pre_pt_num = mdl_pt_num;//������ʷ����
			//mdl_pt_num = img_pt_num = 0;//��������
			//�����������һ�������Ӻ���Ϊ�����ں����ӳ��������ʱ��Ҫ��ʹ��
			/*
			mdl_pts.erase(mdl_pts.begin(), mdl_pts.end());
			img_pts.erase(img_pts.begin(), img_pts.end());
			img_pt_cd.erase(img_pt_cd.begin(), img_pt_cd.end());
			*/			
			
			//�Ѿ������˶�Ӧ�ڸ���ͼ��ӳ�����
			transformed = true;

			//���¼����ͼ��Ӧ���ݲ��ֵ�ָ�뼰�߿���Ϣ�����ں�������ɫ����
			list_charpt::iterator itrt_charpt;
			LPBITMAPINFO lpBitmapInfo;
			view_data = new unsigned char*[num_view];//�洢����ͼͼ�����ݵ�ָ�룬ʹ����������ڲ�����
			biHeight = new unsigned long[num_view];//�洢����ͼͼ��ĸ߶�
			biWidth = new unsigned long[num_view];//�洢����ͼͼ��Ŀ��
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
				
	
	//��ģ������������任�����������Ӷ������ӵ㼴��ԭ��
	CVisDMatrix temp(3, 1);
	for(i = 0; i < M; i++)
	{
		//��任
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_points[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_points[i][j] = temp[j][0];
		}
		
		//�������任
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
				
	
	//��ģ������������任�����������Ӷ������ӵ㼴��ԭ��
	CVisDMatrix temp(3, 1);
	for(i = 0; i < M; i++)
	{
		//��任
		for(j = 0; j < 3; j++)
		{
			temp[j][0] = face1_points[i][j];
		}
		temp = rot*temp;
		for(j = 0; j < 3; j++)
		{
			face1_points[i][j] = temp[j][0];
		}
		
		//�������任
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
			//�ȿ�����򵥵��ļ���ʽ
			char c;
			//double db;
			int i, j;//, index;
			//��ʼд������
			FILE* fp;
			fp = fopen(cstrFileName, "w");
			
			//д�붥����������
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

			//д�붥�㷨��������
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

			if(textured)//�Լ�����������ͼ
			{
				double ** temp_txt;				
				if(cylindrical_map)//��������������ͼ
				{
					temp_txt = texture_map;

					//д�붥���������붥���Ӧ����ɫֵ					
					for(i = 0; i < K1; i++)
					{
						c = 'v';
						fprintf(fp, "%c", c);
						c = 'c';//ע�ⲻͬ��'t'				
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
				else//��ʹ���뵥����ͼ��Ӧ������ͼ
				{
					list_dbpt_2::iterator itrt_dbpt_2;
					//��������ͼָ�������е����һ��Ԫ�أ����뵱ǰ��ͼ��Ӧ������ͼ
					for(itrt_dbpt_2 = face1_texture.begin(); itrt_dbpt_2 != face1_texture.end(); ++itrt_dbpt_2)
					{
						temp_txt = *itrt_dbpt_2;
					}

					//д�붥����������				
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

				//д����������Ϣ
				for(i = 0; i < L1; i++)
				{
					c = 'f';				
					fprintf(fp, "%c\t", c);
					for(j = 0; j < 3; j++)//��һд�������������������Ϣ
					{
						fprintf(fp, "%d", triangles1[i][j]+1);//����ʵ����ţ����Լ�1
						c = '/';				
						fprintf(fp, "%c", c);
						fprintf(fp, "%d", triangles1[i][j]+1);//���������붥��������ϸ��Ӧ�ģ�����ʵ����ţ����Լ�1
						//c = '/';				
						fprintf(fp, "%c", c);
						if(j < 2)
							fprintf(fp, "%d\t", nml_tris[i][j]+1);//����ʵ����ţ����Լ�1
						else
							fprintf(fp, "%d\n", nml_tris[i][j]+1);
					}					
				}
			}
			else//û����������
			{
				//д����������Ϣ
				for(i = 0; i < L1; i++)
				{
					c = 'f';				
					fprintf(fp, "%c\t", c);
					for(j = 0; j < 3; j++)//��һд�������������������Ϣ
					{
						fprintf(fp, "%d", triangles1[i][j]+1);//����ʵ����ţ����Լ�1
						c = '/';				
						fprintf(fp, "%c", c);
						fprintf(fp, "%c", c);
						if(j < 2)
							fprintf(fp, "%d\t", nml_tris[i][j]+1);//�붥��������ϸ��Ӧ�ģ�����ʵ����ţ����Լ�1
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
	                 _T("txt�ļ�(*.txt)|*.txt|"));
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
	fseek(fp, 0L, SEEK_END);//��������ĩβд�룬��Ҫ��ǰ���Ѿ�д������ݻ���
	fprintf(fp, "%d\n", mdl_pt_num);
	//��ӡģ�ͱ궨�㣬Ŀǰ��Ȼ�ǹ�һ��������
	for(dbpt_itrt = pre_mdl_pts.begin(), i = 1; dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt, i++)
	{
		if(i != mdl_pt_num)
		{
			for(j = 0; j < 3; j++)//��������ά��
			{
				fprintf(fp, "%f\t", (*dbpt_itrt)[j]);//�����Ǹ���������				
			}
		}
		else//ֻ�������һ����Ż���
		{
			for(j = 0; j < 3; j++)
			{
				if(j != 2)//��������ά��
					fprintf(fp, "%f\t", (*dbpt_itrt)[j]);
				else 
					fprintf(fp, "%f\n", (*dbpt_itrt)[j]);
			}	
		}
	}
	fprintf(fp, "%d\n", num_view);
	//��ӡͼ��궨��
	for(llippt_itrt = img_pts_list.begin(), llippt_itrt_1 = img_pt_cd_list.begin(), int_itrt = img_pt_num_list.begin(); llippt_itrt != img_pts_list.end(), llippt_itrt_1 != img_pt_cd_list.end(), int_itrt != img_pt_num_list.end(); ++llippt_itrt, ++llippt_itrt_1, ++int_itrt)
	{
		//����ͼ�Ͽɼ�����
		fprintf(fp, "%d\n", (*int_itrt));

		for(intpt_itrt = (*llippt_itrt)[0].begin(), i = 1; intpt_itrt != (*llippt_itrt)[0].end(); ++intpt_itrt, i++)
		{
			if(i != mdl_pt_num)
			{
				for(j = 0; j < 2; j++)//�����Ƕ�ά��
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//�����Ǹ���������				
				}
			}
			else//ֻ�������һ����Ż���
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//�����Ƕ�ά��
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
				for(j = 0; j < 2; j++)//�����Ƕ�ά��
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//�����Ǹ���������				
				}
			}
			else//ֻ�������һ����Ż���
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//�����Ƕ�ά��
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
				for(j = 0; j < 2; j++)//�����Ƕ�ά��
				{
					fprintf(fp, "%d\t", (*intpt_itrt)[j]);//�����Ǹ���������				
				}
			}
			else//ֻ�������һ����Ż���
			{
				for(j = 0; j < 2; j++)
				{
					if(j != 1)//�����Ƕ�ά��
						fprintf(fp, "%d\t", (*intpt_itrt)[j]);
					else 
						fprintf(fp, "%d\n", (*intpt_itrt)[j]);
				}	
			}
		}
	}
	*/
	fclose(fp);

	//����ڶ��α궨Ч���ã�Ҳ���Դ洢�����Ǵ洢�ڲ�ͬ���ļ���
	if(	which_loc == 2 && obj_fp_lc && img_fp_lc)
	{
		cstrFileName;
		CFileDialog dlg_1( TRUE,NULL,NULL,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
			_T("txt�ļ�(*.txt)|*.txt|"));
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
		fseek(fp, 0L, SEEK_END);//��������ĩβд�룬��Ҫ��ǰ���Ѿ�д������ݻ���
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
					if(j != 2)//��������ά��
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
					if(j != 2)//��������ά��
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
	//���ﲻ�ؼ�����հɣ������txt_mapping��Ҫ�����أ�
	for(dbpt_itrt = pre_mdl_pts.begin(); dbpt_itrt != pre_mdl_pts.end(); ++dbpt_itrt)
	{
		delete [](*dbpt_itrt);//�ͷ�������Ԫ����ָ����ڴ�ռ�						
	}
	pre_mdl_pts.erase(pre_mdl_pts.begin(), pre_mdl_pts.end());
	*/
	
}

void COGLEventDoc::OnDataAlalysis() 
{
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt�ļ�(*.txt)|*.txt|"));
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
	//��һ�����ݲ�����Ҫ���бȽ�
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

	//��ʼ��
	num_mdl_dif_0 = num_mdl_dif_1 = num_mdl_dif_2 = num_mdl_dif_sum = 0;
	num_img_dif_0 = num_img_dif_1 = num_img_dif_sum = 0;

	for(i = 0; i < num; i++)
	{
		mdl_dif[i] = new double[3];
		img_dif[i] = new int[2];

		if(i == 0)//��ʼ��
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
	                 _T("txt�ļ�(*.txt)|*.txt|"));
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


//�����Ѽ���õ��Ķ�Ӧ�ڸ���ͼ��ӳ���������һ������Ż�ģ�����ѱ���ĸ���
void COGLEventDoc::OnPrmtOpt() 
{
	int i, j, k, cur_view;
	list_dbpt::iterator dbpt_itrt;
	CVisDMatrix K, U;
	CVisDMatrix cur_mdl_pt(3, 1);
	double cur_dis, dis_sum;	
	mdl_pts_trace = new double**[mdl_pt_num];//��¼����������ģ���ϱ�ǵ���ƶ��켣
	iter_num = 125;//��������

	double temp_mdl_pt[3];
	int temp_img_pt[2];
	double pre_err_sum, temp_err_sum;

	double pre_err_sum_2, temp_err_sum_2;//��¼����ģ�͵���������ͼ��ʵ��ӳ������ʼ��ǵ�֮�����֮��

	list_db pts_err_sum_list;//��¼��δ���µ�ģ�͵��Ӧ������������������
	
	bool* pts_update = new bool[mdl_pt_num];//���ģ�͵��Ƿ��Ѹ���
	bool update_over;//��Ǹ����Ƿ����
	
	double** temp_opt_array = new double*[mdl_pt_num];//��¼�����µ�ģ�͵�
	
	double* pts_err_sum_array = new double[mdl_pt_num];//��¼�����µ�ģ�͵��Ӧ�������
	double* pts_err_sum_array_1 = new double[mdl_pt_num];//��¼����ǰ��ģ�͵��Ӧ�������

	int update_sum = 0;//��¼�Ѹ��µ�ģ�͵���	

	//��ʼ��
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

	//��ʼ��
	for(i = 0; i < mdl_pt_num; i++)
		pts_update[i] = false;	
		
	pre_err_sum_2 = temp_err_sum_2 = 0;//��ʼ��

	for(i = 0; i < mdl_pt_num; i++)
	{
		//���ݵ�ǰ�������пɼ���ͼ��������ȷ�������С��ÿ����ͼ������������
		K.Resize(Vis_Num[i]*2, 3);
		U.Resize(Vis_Num[i]*2, 1);
		k = 0;
		for(cur_view = 0; cur_view < num_view; cur_view++)
		{
			if(Vis_Map[i][cur_view])//����ǰ���ڸ���ͼ�пɼ�
			{
				//�Ѿ���������ȷ��
				K[2*k][0] = inter_mat_list[cur_view][0][0] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][0];
				K[2*k][1] = inter_mat_list[cur_view][0][1] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][1];
				K[2*k][2] = inter_mat_list[cur_view][0][2] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][2];
				U[2*k][0] = img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][0][3];

				K[2*k+1][0] = inter_mat_list[cur_view][1][0] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][0];
				K[2*k+1][1] = inter_mat_list[cur_view][1][1] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][1];
				K[2*k+1][2] = inter_mat_list[cur_view][1][2] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][2];
				U[2*k+1][0] = img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][1][3];

				k++;//�������������
			}
		}
		cur_mdl_pt = (K.Transposed()*K).Inverted()*K.Transposed()*U;
		
		pre_err_sum = temp_err_sum = 0;//��ʼ��
		
		for(cur_view = 0; cur_view < num_view; cur_view++)
		{
			if(Vis_Map[i][cur_view])//��ǰ�������ڸ���ͼ�пɼ�
			{
				//ֻҪ����ǰ����������ֵ
				for(j = 0; j < 3; j++)
				{
					temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//��û�н�ģ���ϱ�ǵ�ת����ʵ�ʳ߶�
				}	
				
				temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				
				//ӳ�����ʵ�ʱ�ǵ�����
				pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
				
				//ֻҪ����ǰ����������ֵ
				for(j = 0; j < 3; j++)
				{
					temp_mdl_pt[j] = cur_mdl_pt[j][0];
				}
				
				temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
				
				//ӳ�����ʵ�ʱ�ǵ�����
				temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
			}
		}
		
		pre_err_sum_2 += pre_err_sum;
		temp_err_sum_2 += pre_err_sum;

		//ֻ�е�ǰ���ͱȸ���ǰ����Сʱ�Ÿ���ӳ�����
		if(temp_err_sum < pre_err_sum)
		{
			pts_update[i] = true;
			
			//���¼��������ͼ��Ӧ��ӳ�����							
			for(j = 0; j < 3; j++)
				opt_mdl_pts[i][j] = cur_mdl_pt[j][0]/Max_V;//ע��ӳ������Ǹ���ģ���ϱ�ǵ��ʵ�ʳ߶��������

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
			pts_err_sum_list.insert(pts_err_sum_list.end(), temp_err_sum-pre_err_sum);//������������뵽������
			
			temp_opt_array[i] = new double[3];
			for(j = 0; j < 3; j++)
				temp_opt_array[i][j] = cur_mdl_pt[j][0]/Max_V;//ע��ӳ������Ǹ���ģ���ϱ�ǵ��ʵ�ʳ߶��������
		}
	}

	/*CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt�ļ�(*.txt)|*.txt|"));
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
	fseek(fp, 0L, SEEK_END);//��������ĩβд�룬��Ҫ��ǰ���Ѿ�д������ݻ���
	fprintf(fp, "%f\n", pre_err_sum_2);
	fprintf(fp, "%f\n", temp_err_sum_2);*/		

	CFormCommandView1 *pView = (CFormCommandView1 *)GetFormCommandView();
	pView->m_error_sum.Format("%.3lf", pre_err_sum_2);
	pView->UpdateData(FALSE);
	
	pts_err_sum_list.sort();//��������Ԫ�ذ���������
	
	update_over = false;

	if(update_sum == mdl_pt_num)//���е���Ѹ���
		update_over = true;
	
	//���������ͼ��Ӧ��ӳ�����
	while(!update_over)
	{
		for(i = 0; i < mdl_pt_num; i++)
		{
			if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())) && temp_err_sum_2+pts_err_sum_array[i]-pts_err_sum_array_1[i] < pre_err_sum_2)//��ӳ�������δ����
			{				
				pts_update[i] = true;
				
				for(j = 0; j < 3; j++)
					opt_mdl_pts[i][j] = temp_opt_array[i][j];
				
				//temp_err_sum_2 += pts_err_sum_array[i];//��ʱֻ�ۼ����Ѹ��µ�ģ�͵��Ӧ��������
				temp_err_sum_2 += (pts_err_sum_array[i]-pts_err_sum_array_1[i]);//��ʱֻ�ۼ����Ѹ��µ�ģ�͵��Ӧ��������

				update_sum ++;
				
				pts_err_sum_list.erase(pts_err_sum_list.begin());//�����������������ɾ��	
				
				if(update_sum == mdl_pt_num)//���е���Ѹ���
				{
					update_over = true;
					break;
				}				
			}
			else
				if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())))//��ӳ�������δ����
				{
					update_over = true;//�����С���Ч����������������
					break;
				}
		}
	}
	
	mdl_pts_opted = true;//ģ���ϱ�Ǹ����ѱ��Ż�
	
	//�����Ż�ǰ��ģ���϶�Ӧ��֮�����֮��
	dis_sum = 0;//��ʼ�� 
	for(i = 0; i < mdl_pt_num; i++)
	{
		cur_dis = 0;//��ʼ��
		for(j = 0; j < 3; j++)
		{
			cur_dis += (opt_mdl_pts[i][j] - mdl_pts_array[i][j])*(opt_mdl_pts[i][j] - mdl_pts_array[i][j]);
		}
		cur_dis = sqrt(cur_dis);//��ƽ����
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
		
		if(img_pt_num == mdl_pt_num)//��ǵ����Ѿ��㹻
		{
			img_fp_lc = true;
			img_pt_num_list.insert(img_pt_num_list.end(),temp_img_pt_num);
		}
	}
}

void COGLEventDoc::OnIterative() 
{	
	int i, j, k;	
	
	temp_iter_num = 0;//��ʼ��
	iter_begin = true;//��ʼ����
	
	//�������
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
	
	int cur_view;//��¼��ǰ��ͼ�����
	double dis_sum, cur_dis;
		
	CVisDMatrix temp_mat(3, 4);
	double temp_mdl_pt[3];
	int temp_img_pt[2];
	double pre_err_sum, temp_err_sum;//��¼����ģ�͵���ĳ��ͼ��ʵ��ӳ������ʼ��ǵ�֮�����֮�ͻ��¼ĳģ�͵���������ͼ��ʵ��ӳ������ʼ��ǵ�֮�����֮��
	double pre_err_sum_2, temp_err_sum_2;//��¼����ģ�͵���������ͼ��ʵ��ӳ������ʼ��ǵ�֮�����֮��
	//double inter_err_sum_2;

	list_db views_err_sum_list;//��¼��δ���µ�ӳ������Ӧ������������������
	list_db pts_err_sum_list;//��¼��δ���µ�ģ�͵��Ӧ������������������
	
	bool* views_update = new bool[num_view];//���ӳ������Ƿ��Ѹ���
	bool* pts_update = new bool[mdl_pt_num];//���ģ�͵��Ƿ��Ѹ���
	bool update_over;//��Ǹ����Ƿ����
	
	CVisDMatrix* temp_inter_mat_array = new CVisDMatrix[num_view];//��¼�����µ�ӳ�����
	double** temp_opt_array = new double*[mdl_pt_num];//��¼�����µ�ģ�͵�
	
	double* views_err_sum_array = new double[num_view];//��¼�����µ�ӳ������Ӧ�������
	double* views_err_sum_array_1 = new double[num_view];//��¼�����µ�ӳ������Ӧ�������
	double* pts_err_sum_array = new double[mdl_pt_num];//��¼�����µ�ģ�͵��Ӧ�������
	double* pts_err_sum_array_1 = new double[mdl_pt_num];//��¼�����µ�ģ�͵��Ӧ�������

	int update_sum = 0;

	/*
	CString cstrFileName;
	CFileDialog dlg( TRUE,NULL,NULL,
	                 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	                 _T("txt�ļ�(*.txt)|*.txt|"));
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
	fseek(fp, 0L, SEEK_END);//��������ĩβд�룬��Ҫ��ǰ���Ѿ�д������ݻ���
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

		//��ʼ��
		for(i = 0; i < num_view; i++)
			views_update[i] = false;
		for(i = 0; i < mdl_pt_num; i++)
			pts_update[i] = false;	
		
		pre_err_sum_2 = temp_err_sum_2 = 0;// = inter_err_sum_2 = 0;
		update_sum = 0;

		//���������ͼ��Ӧ��ӳ�����
		for(int_itrt = img_pt_num_list.begin(), cur_view = 0; int_itrt != img_pt_num_list.end(); ++int_itrt, cur_view++)
		{
			temp_img_pt_num = (*int_itrt);
			
			//���ݵ�ǰ��ͼ��ʵ�ʱ�ǵ���ȷ�������С
			C.Resize(2*temp_img_pt_num, 9);
			D.Resize(2*temp_img_pt_num, 3);
			
			k = 0;
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//��ǰ�������ڸ���ͼ�пɼ�
				{
					for(j = 0; j < 3; j++)
					{
						D[2*k][j] = -img_pts_mat[i][cur_view][0]*mdl_pts_array[i][j]*Max_V;//-ui*Xi[j]
						//��ӡż���е�����
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
						//��ӡ�����е�����
						/*
						if(j != 2)
						fprintf(fp, "%f\t", D[2*i+1][j]);
						else 
						fprintf(fp, "%f\n", D[2*i+1][j]);
						*/
					}
					k++;//�����������
				}
			}
			
			k = 0;
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//��ǰ�������ڸ���ͼ�пɼ�
				{
					for(j = 0; j < 3; j++)
					{
						C[2*k][j] = mdl_pts_array[i][j]*Max_V;//��Ҫ�任��ʵ�ʵ�����ֵ
						//fprintf(fp, "%f\t", C[2*i][j]);
					}
					C[2*k][3] = 1;
					//fprintf(fp, "%f\t", C[2*i][3]);
					for(j = 0; j < 4; j++)
					{
						C[2*k][4+j] = 0;//�ӵ�5������8��Ԫ�ص�ȡֵ����0
						//fprintf(fp, "%f\t", C[2*i][4+j]);
					}
					C[2*k][8] = -img_pts_mat[i][cur_view][0];//-ui
					//fprintf(fp, "%f\n", C[2*i][8]);
					
					for(j = 0; j < 4; j++)
					{
						C[2*k+1][j] = 0;//�ӵ�1������4��Ԫ�ص�ȡֵ����0
						//fprintf(fp, "%f\t", C[2*i+1][j]);
					}
					for(j = 0; j < 3; j++)
					{
						C[2*k+1][4+j] = mdl_pts_array[i][j]*Max_V;//��Ҫ�任��ʵ�ʵ�����ֵ
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
			
			//���¼��������ͼ��Ӧ��ӳ�����
			for(j = 0; j < 3; j++)
			{
				temp_mat[0][j] = y[j];//��1��3��Ԫ��
				temp_mat[1][j] = y[4+j];//��5��7��Ԫ��
				temp_mat[2][j] = z[j];//��1��3��Ԫ��
			}
			temp_mat[0][3] = y[3];//��4Ԫ��
			temp_mat[1][3] = y[7];//��8��Ԫ��
			temp_mat[2][3] = y[8];//��9��Ԫ��
			
			pre_err_sum = temp_err_sum = 0;//��ʼ��
			
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(Vis_Map[i][cur_view])//��ǰ�������ڸ���ͼ�пɼ�
				{
					//ֻҪ����ǰ����������ֵ
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//��û�н�ģ���ϱ�ǵ�ת����ʵ�ʳ߶�
					}	
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//ӳ�����ʵ�ʱ�ǵ�����
					pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*temp_mat[0][0]+temp_mdl_pt[1]*temp_mat[0][1]+temp_mdl_pt[2]*temp_mat[0][2]+temp_mat[0][3])/(temp_mdl_pt[0]*temp_mat[2][0]+temp_mdl_pt[1]*temp_mat[2][1]+temp_mdl_pt[2]*temp_mat[2][2]+temp_mat[2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*temp_mat[1][0]+temp_mdl_pt[1]*temp_mat[1][1]+temp_mdl_pt[2]*temp_mat[1][2]+temp_mat[1][3])/(temp_mdl_pt[0]*temp_mat[2][0]+temp_mdl_pt[1]*temp_mat[2][1]+temp_mdl_pt[2]*temp_mat[2][2]+temp_mat[2][3]));
					
					//ӳ�����ʵ�ʱ�ǵ�����
					temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));

				}
			}
			
			pre_err_sum_2 += pre_err_sum;
			temp_err_sum_2 += pre_err_sum;
			
			//ֻ�е���ǰ���ͱȸ���ǰ����Сʱ�Ÿ���ӳ�����
			if(temp_err_sum < pre_err_sum)
			{
				views_update[cur_view] = true;
				
				//���¼��������ͼ��Ӧ��ӳ�����
				for(i = 0; i < 3; i++)
				{
					for(j = 0; j < 4; j++)
					{
						inter_mat_list[cur_view][i][j] = temp_mat[i][j];
					}
				}
				
				//temp_err_sum_2 += temp_err_sum;//��ʱֻ�ۼ����Ѹ��µ�ӳ������Ӧ��������	
				
				temp_err_sum_2 += (temp_err_sum - pre_err_sum);//�ø��º��������滻����ǰ������
				
				update_sum++;
			}
			else
			{
				views_err_sum_array[cur_view] = temp_err_sum;
				views_err_sum_array_1[cur_view] = pre_err_sum;
				views_err_sum_list.insert(views_err_sum_list.end(), temp_err_sum-pre_err_sum);//������������뵽������
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
		views_err_sum_list.sort();//��������Ԫ�ذ���������
		
		update_over = false;

		if(update_sum == num_view)
			update_over = true;

		//���������ͼ��Ӧ��ӳ�����
		while(!update_over)
		{
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(!views_update[cur_view] && (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] == (*views_err_sum_list.begin())) && temp_err_sum_2+views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] < pre_err_sum_2)//��ӳ�������δ����
				{				
					views_update[cur_view] = true;
					
					//���¼��������ͼ��Ӧ��ӳ�����
					for(i = 0; i < 3; i++)
					{
						for(j = 0; j < 4; j++)
						{
							inter_mat_list[cur_view][i][j] = temp_inter_mat_array[cur_view][i][j];
						}
					}
					
					//temp_err_sum_2 += views_err_sum_array[cur_view];//��ʱֻ�ۼ����Ѹ��µ�ӳ������Ӧ��������
					temp_err_sum_2 += (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view]);
					
					views_err_sum_list.erase(views_err_sum_list.begin());//�����������������ɾ��					
					
					update_sum++;

					if(update_sum == num_view)
					{
						update_over = true;
						break;
					}
					
				}
				else
					if(!views_update[cur_view] && (views_err_sum_array[cur_view]-views_err_sum_array_1[cur_view] == (*views_err_sum_list.begin())))//��ӳ�������δ����
					{
						update_over = true;//�����С���Ч����������������
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

		//����ǰ�����Ķ�Ӧ����ͼ��ӳ��������Ż���ģ�ͱ�ǵ�
		for(i = 0; i < mdl_pt_num; i++)
		{
			//���ݵ�ǰ�������пɼ���ͼ��������ȷ�������С��ÿ����ͼ������������
			K.Resize(Vis_Num[i]*2, 3);
			U.Resize(Vis_Num[i]*2, 1);
			k = 0;
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(Vis_Map[i][cur_view])//����ǰ���ڸ���ͼ�пɼ�
				{
					//�Ѿ���������ȷ��
					K[2*k][0] = inter_mat_list[cur_view][0][0] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][0];
					K[2*k][1] = inter_mat_list[cur_view][0][1] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][1];
					K[2*k][2] = inter_mat_list[cur_view][0][2] - img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][2];
					U[2*k][0] = img_pts_mat[i][cur_view][0]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][0][3];
					
					K[2*k+1][0] = inter_mat_list[cur_view][1][0] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][0];
					K[2*k+1][1] = inter_mat_list[cur_view][1][1] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][1];
					K[2*k+1][2] = inter_mat_list[cur_view][1][2] - img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][2];
					U[2*k+1][0] = img_pts_mat[i][cur_view][1]*inter_mat_list[cur_view][2][3] - inter_mat_list[cur_view][1][3];
					
					k++;//�������������
				}
			}
			cur_mdl_pt = (K.Transposed()*K).Inverted()*K.Transposed()*U;
			
			pre_err_sum = temp_err_sum = 0;//��ʼ��
			
			for(cur_view = 0; cur_view < num_view; cur_view++)
			{
				if(Vis_Map[i][cur_view])//��ǰ�������ڸ���ͼ�пɼ�
				{
					//ֻҪ����ǰ����������ֵ
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = mdl_pts_array[i][j]*Max_V;//��û�н�ģ���ϱ�ǵ�ת����ʵ�ʳ߶�
					}	
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//ӳ�����ʵ�ʱ�ǵ�����
					pre_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
					
					//ֻҪ����ǰ����������ֵ
					for(j = 0; j < 3; j++)
					{
						temp_mdl_pt[j] = cur_mdl_pt[j][0];
					}
					
					temp_img_pt[0] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][0][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][0][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][0][2]+inter_mat_list[cur_view][0][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					temp_img_pt[1] = int((temp_mdl_pt[0]*inter_mat_list[cur_view][1][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][1][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][1][2]+inter_mat_list[cur_view][1][3])/(temp_mdl_pt[0]*inter_mat_list[cur_view][2][0]+temp_mdl_pt[1]*inter_mat_list[cur_view][2][1]+temp_mdl_pt[2]*inter_mat_list[cur_view][2][2]+inter_mat_list[cur_view][2][3]));
					
					//ӳ�����ʵ�ʱ�ǵ�����
					temp_err_sum += sqrt((img_pts_mat[i][cur_view][0]-temp_img_pt[0])*(img_pts_mat[i][cur_view][0]-temp_img_pt[0])+(img_pts_mat[i][cur_view][1]-temp_img_pt[1])*(img_pts_mat[i][cur_view][1]-temp_img_pt[1]));
				}
			}
			
			pre_err_sum_2 += pre_err_sum;
			temp_err_sum_2 += pre_err_sum;
			
			//ֻ�е���ǰ���ͱȸ���ǰ����Сʱ�Ÿ���ӳ�����
			if(temp_err_sum < pre_err_sum)
			{
				pts_update[i] = true;
				
				//���¼��������ͼ��Ӧ��ӳ�����							
				for(j = 0; j < 3; j++)
					opt_mdl_pts[i][j] = cur_mdl_pt[j][0]/Max_V;//ע��ӳ������Ǹ���ģ���ϱ�ǵ��ʵ�ʳ߶��������

				//temp_err_sum_2 += temp_err_sum;//��ʱֻ�ۼ����Ѹ��µ�ӳ������Ӧ��������
				temp_err_sum_2 += (temp_err_sum-pre_err_sum);//��ʱֻ�ۼ����Ѹ��µ�ӳ������Ӧ��������
				update_sum++;
			}
			else
			{
				pts_err_sum_array[i] = temp_err_sum;
				pts_err_sum_array_1[i] = pre_err_sum;
				pts_err_sum_list.insert(pts_err_sum_list.end(), temp_err_sum-pre_err_sum);//������������뵽������

				temp_opt_array[i] = new double[3];
				for(j = 0; j < 3; j++)
					temp_opt_array[i][j] = cur_mdl_pt[j][0]/Max_V;//ע��ӳ������Ǹ���ģ���ϱ�ǵ��ʵ�ʳ߶��������
			}
		}
		
		//fprintf(fp, "%f\n", pre_err_sum_2);
		//fprintf(fp, "%f\n", temp_err_sum_2);
		
		pts_err_sum_list.sort();//��������Ԫ�ذ���������
		
		update_over = false;

		if(update_sum == mdl_pt_num)
			update_over = true;		

		//���������ͼ��Ӧ��ӳ�����
		while(!update_over)
		{
			for(i = 0; i < mdl_pt_num; i++)
			{
				if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())) && temp_err_sum_2+pts_err_sum_array[i]-pts_err_sum_array_1[i]<pre_err_sum_2)//��ӳ�������δ����
				{
					
					pts_update[i] = true;
					
					for(j = 0; j < 3; j++)
						opt_mdl_pts[i][j] = temp_opt_array[i][j];
					
					//temp_err_sum_2 += pts_err_sum_array[i];//��ʱֻ�ۼ����Ѹ��µ�ģ�͵��Ӧ��������
					temp_err_sum_2 += (pts_err_sum_array[i]-pts_err_sum_array_1[i]);//��ʱֻ�ۼ����Ѹ��µ�ģ�͵��Ӧ��������
					
					pts_err_sum_list.erase(pts_err_sum_list.begin());//�����������������ɾ��					

					update_sum++;
					if(update_sum == mdl_pt_num)
					{
						update_over = true;
						break;
					}					
				}
				else
					if(!pts_update[i] && (pts_err_sum_array[i]-pts_err_sum_array_1[i] == (*pts_err_sum_list.begin())))//��ӳ�������δ����
					{
						update_over = true;//�����С���Ч����������������
						break;
					}
			}
		}

		//�����Ż�ǰ��ģ���϶�Ӧ��֮�����֮��
		dis_sum = 0;//��ʼ�� 
		for(i = 0; i < mdl_pt_num; i++)
		{
			cur_dis = 0;//��ʼ��
			for(j = 0; j < 3; j++)
			{
				cur_dis += (opt_mdl_pts[i][j] - mdl_pts_array[i][j])*(opt_mdl_pts[i][j] - mdl_pts_array[i][j]);
			}
			cur_dis = sqrt(cur_dis);//��ƽ����
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

		//���ݼ����ӳ������������ӳ��
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
	//���뵱ǰǰ��ͼ��
	LPBITMAPINFOHEADER lpBitmapInfoHeader = (LPBITMAPINFOHEADER)(m_pBitmap+14);
	LPBITMAPFILEHEADER lpBitmapFileHeader = (LPBITMAPFILEHEADER)m_pBitmap;
	unsigned char *lpData = m_pBitmap + lpBitmapFileHeader->bfOffBits;
	unsigned long biHeight = lpBitmapInfoHeader->biHeight;
	unsigned long biWidth = lpBitmapInfoHeader->biWidth;
	unsigned long biAlign = (biWidth*3+3)/4 *4;

	//���뵱ǰ����ͼ��
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
					for(j = 0; j < (int)biWidth; j++)//����һ���е������������Ӧ����
					{
						cur = i*biAlign+3*j;//��ǰ������ʼλ��
						tempb = abs(lpData[cur]-lpData_1[cur]);//Ҫ�Ƚϵ��ǲ���Ĵ�С
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

//����������ͼ�и�����������ģ�ͽ��֮��Ķ�Ӧ��ϵ
void COGLEventDoc::OnMap() 
{
}

void COGLEventDoc::Mapping()
{	
	unsigned long v, u, i, j, k;
	//��¼�����ض�Ӧ�ĵ�ǰ����һ�ε����񶥵��z���꣬����ӽ����ӵ�����񶥵��ҳ�
	double* temp_z = NULL;//��¼�ڵ�ǰ��ͼ�������ѭ���õ���������ض�Ӧ�����񶥵�����ת������ֵ
	double* prev_z = NULL;//��¼�ڵ�ǰ��ͼ����ǰ��ѭ���õ���������ض�Ӧ�����񶥵�����ת������ֵ
	list_charpt::iterator itrt_charpt;//�ַ�����ı�����		
	unsigned char* temp_view_pt;//��ǰ��ͼ��ָ��	
	CVisDVector m3(3);
	double m_34;
	CVisDVector temp(3);//���ڸо�temp�������ʵ���ǲ�������
	
	//��ʼ��
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
				
	//�����ѭ������ͼָ�������
	for(k = 0; k < num_view; k++)
	{
		//Ϊ��ǰ������ͼ�ĸ�����ȷ�����Ӧ�����񶥵�
		//�����뵱ǰ��ͼ��Ӧ����ת����ĵ�����	
		for(j = 0; j < 3; j++)
		{
			m3[j] = inter_mat_list[k][2][j];//��1��3��Ԫ��
		}
		
		m_34 = inter_mat_list[k][2][3];;
		r3[k] = m3*(m_34>0?1:-1);
		
		//�����µ���ͼ��������
		if(temp_z != NULL)
			delete []temp_z;
		if(prev_z != NULL)
			delete []prev_z;
		//�ռ��С���ɸ���ͼ�ĸ߿������
		temp_z = new double[biHeight[k]*biWidth[k]];
		prev_z = new double[biHeight[k]*biWidth[k]];

		/*
		//��ʼ��
		for(i = 0; i < biHeight[k]*biWidth[k]; i++)
		{
			prev_z[i] = 10000;
		}
		*/
		
		//�ڶ���ѭ������������ģ�͵ĸ�����
		for(i = 0; i < M; i++)
		{
			for(j = 0; j < 3; j++)
			{
				temp[j] = face1_normals[i][j];//��¼��ǰ���񶥵�ķ�����
			}
			//���ں�Ȼ������ΪʲôҪ��һ�����ţ����������������ˣ�������ζ��Z�Ḻ����
			if(-r3[k]*temp>cos(PI/2))//�����ת����������񶥵�ķ����������߷���ļн�С��90��
			{
				//ͨ����ǰ��ͼ��ӳ���������������񶥵��Ӧ�����أ�����˵�Ƚϵ�׼ȷ��
				v = (face1_points[i][0]*inter_mat_list[k][0][0]+face1_points[i][1]*inter_mat_list[k][0][1]+face1_points[i][2]*inter_mat_list[k][0][2]+inter_mat_list[k][0][3])/(face1_points[i][0]*inter_mat_list[k][2][0]+face1_points[i][1]*inter_mat_list[k][2][1]+face1_points[i][2]*inter_mat_list[k][2][2]+inter_mat_list[k][2][3]);
				u = (face1_points[i][0]*inter_mat_list[k][1][0]+face1_points[i][1]*inter_mat_list[k][1][1]+face1_points[i][2]*inter_mat_list[k][1][2]+inter_mat_list[k][1][3])/(face1_points[i][0]*inter_mat_list[k][2][0]+face1_points[i][1]*inter_mat_list[k][2][1]+face1_points[i][2]*inter_mat_list[k][2][2]+inter_mat_list[k][2][3]);
				
				if(u < biWidth[k] && v < biHeight[k])//ע��߽�����
				{			
					//������ת�����񶥵��z����
					for(j = 0; j < 3; j++)
					{
						temp[j] = face1_points[i][j];
					}
					
					temp_z[v*biWidth[k]+u] = r3[k]*temp;//�����뵱ǰ���񶥵�����ת���z���겢�������Ӧ�����ص���Ӧȡֵ//����Ϊ��������û�б�Ҫ��һ�����ŵ�
					
					if(pxl_map[k][v*biWidth[k]+u] == -1 || temp_z[v*biWidth[k]+u] < prev_z[v*biWidth[k]+u])/*��������δ���κ����񶥵㽨����Ӧ��ϵ*///�жϵ�ǰ���񶥵��Ƿ�����ӵ��"��"���Ǵ�z�Ḻ�����Ͽ���
					{
						pxl_map[k][v*biWidth[k]+u] = i;//���¶�Ӧ�����񶥵�����
						//map_num[k][v*biWidth[k]+u]++;//�뵱ǰ��ͼ�и����ض�Ӧ�����񶥵��������1��
						//��̫������������ʲô����
						/*
						if(temp_z[v*biWidth+u] < prev_z[v*biWidth+u])
							prev_z[v*biWidth+u] = temp_z[v*biWidth+u];	
						*/
						prev_z[v*biWidth[k]+u] = temp_z[v*biWidth[k]+u];					
					}
				}
			}
		}
		
		//�������ǰ��ͼ��������ض�Ӧ�����񶥵�֮�󣬷�������������񶥵��ڵ�ǰ��ͼ�еĶ�Ӧ����
		//�Ե�ǰ��ͼ�����ؽ���ѭ��
		//��ʼ����
		map_pxl_num[k] = 0;
		for(v = 0; v < biHeight[k]; v++)
		{			
			for(u = 0; u < biWidth[k]; u++)
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//�������Ѿ�ȷ���˶�Ӧ�����񶥵�
				{				
					vtx_map[k][pxl_map[k][v*biWidth[k]+u]][0] = u;
					vtx_map[k][pxl_map[k][v*biWidth[k]+u]][1] = v;
					//��¼��ǰ��ͼ�д��ڶ�Ӧ���񶥵�����ص���Ŀ��֮ǰ�Ѿ������˳�ʼ��
					map_pxl_num[k]++;
				}		
			}			
		}	
		//����ͻ����һ�����⣬���������񶥵���ĳЩ��ͼ�в����ڶ�Ӧ�����أ�ʵ���ϻ���֮�������Ƿ񲻿ɼ�����ʵ���������������ش�ȻӦ���ǿ϶��ġ�
		//���ɼ��ĺ���������Ϊ���ڵ�����ôһ��������������ض�Ӧ�������ϵĿ���λ�á�
		
		/*
		//���������ͼ��ÿ�����ؿ�������ٸ����񶥵��Ӧ
		for(v = 0; v < biHeight; v++)
		{			
			for(u = 0; u < biWidth; u++)
			{
				if(map_num[k][v*biWidth+u] > 1)//�������Ѿ�ȷ���˶�Ӧ�����񶥵�
				{	
					map_num[k][v*biWidth+u] = map_num[k][v*biWidth+u];
				}		
			}			
		}	
		*/
	}
	
	//�����ڸ�����ͼ��ӵ�ж�Ӧ���񶥵�����ص���Ŀ֮�ͣ�ʵ����Ҳ���ܵ������ĸ���
	//��ʼ����Ҫ��ÿ�ε���ʱ�ͻ��ۼ���ȥ�ġ�����
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
	int gray_k[9];//������Sobel��Ĥ������ָ�����ش���ƫ��
	CVisDVector pst(3);//��¼�뵱ǰ���������ض�Ӧ�����񶥵�Ŀռ�����
	CVisDVector nml(3);//��¼�뵱ǰ���������ض�Ӧ�����񶥵�ķ�����	
	
	//ÿ�θ���ͶӰ��������󣬶�Ҫ���¼�����ص����ݣ�����Ȩ�أ�ƫ�����Ҷ���Ϣ
	for(i = 0; i < M; i++)
	{
		for(j = 0; j < 3; j++)
		{
			//pst[j] = face1_points[i][j];//����������Ȩֵʱ��һ��Ҫ�õ���
			nml[j] = face1_normals[i][j];//����������Ȩֵʱ��һ��Ҫ�õ���
		}
		
		//�����ۼ��Ҫ��ʼ��0
		deno_of_weight[i] = 0;	//Ȩ�صķ�ĸ���ֳ�ʼ��Ϊ0	
		vtx_gray[i] = 0;//���ض�Ӧ�Ҷȳ�ʼ��Ϊ0
		
		//�ڶ���ѭ���ڸ���ͼ
		for(k = 0; k < num_view; k++)
		{
			if(vtx_map[k][i][0] != -1)//�������ڸ���ͼ�д���һ����Ӧ����
			{
				//��ʼ��			
				vis_img[k][i] = true;
				if(r3[k]*nml>cos(PI/2))//�ж����񶥵�����ͼk���Ƿ�ɼ����ϸ񽲲���ȷ���ڻ�����0�Ƿ���ɼ����жϵȼۣ�
					vis_img[k][i] = false;
				else
					deno_of_weight[i] += r3[k]*nml;//�ۼ��Լ����ĸ���֣������񶥵������пɼ�����ͼ��Ӧ���ڻ�����ͣ�����Ѹ��Ŷ�����
			}
			else //�п��ܳ�ʼ���Ͷ���Ϊtrue
				vis_img[k][i] = false;
		}
		//�������񶥵��ڸ���ͼ�����������пɼ����ж�Ӧ��Ȩֵ����Ӧ���صĻҶȣ�����һ���ģ��ڶ�Ӧ���ش�����X��Y�����ƫ��
		for(k = 0; k < num_view; k++)
		{
			if(vis_img[k][i])//�������񶥵�����ͼk�пɼ�
			{
				//��������񶥵��Ӧ����ͼk��Ȩֵ
				weight[k][i] = r3[k]*nml/deno_of_weight[i];
				//��������񶥵�����ͼk�ж�Ӧ���ش����ڸ�������ɫ��ƫ��
				//����ָ�����ص����꣬��3*3���������
				//��¼���񶥵�i����ͼk�ж�Ӧ�����ص�����
				u_k = vtx_map[k][i][0];
				v_k = vtx_map[k][i][1];
				//����gray_k���༴ָ��������Χ8����ĻҶ�ֵ
				for(l = -1; l < 2; l++)//����
				{
					for(m = -1; m < 2; m++)//����
					{
						if(l == 0 && m == 0)//��Ĥ����λ�ã������񶥵��Ӧ���ش�
								vtx_map_gray[k][i] = view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)] * 0.11;							

						//�����Ѿ��ٶ�ָ������һ�����ڱ߽��ϣ����򽫿��ܳ��ַ���Խ�磡�����ڵ���Щͼ�񿴣����п��ܳ����ڱ߽��ϵģ�
						if(v_k+l > -1 && v_k+l < biHeight[k] && u_k+m > -1 && u_k+m < biWidth[k])
						{
							gray_k[(l+1)*3+(m+1)] = view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(v_k+l)*biAlign[k]+3*(u_k+m)] * 0.11;							
						}
						else//���������
						{
							if(v_k+l == -1)//�ϱ߽�
							{
								if(u_k+m == -1)//��߽�
									gray_k[(l+1)*3+(m+1)] = view_data[k][2] * 0.3 + view_data[k][1] * 0.59 + view_data[k][0] * 0.11;
								else
								{
									if(u_k+m == biWidth[k])//�ұ߽�
										gray_k[(l+1)*3+(m+1)] = view_data[k][3*(biWidth[k]-1)+2] * 0.3 + view_data[k][3*(biWidth[k]-1)+1] * 0.59 + view_data[k][3*(biWidth[k]-1)] * 0.11;
									else
										gray_k[(l+1)*3+(m+1)] = view_data[k][3*(u_k+m)+2] * 0.3 + view_data[k][3*(u_k+m)+1] * 0.59 + view_data[k][3*(u_k+m)] * 0.11;
								}												
							}	
							if(v_k+l == biHeight[k])//�±߽�
							{
								if(u_k+m == -1)//��߽�
									gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]] * 0.11;
								else
								{
									if(u_k+m == biWidth[k])//�ұ߽�
										gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(biWidth[k]-1)] * 0.11;
									else
										gray_k[(l+1)*3+(m+1)] = view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)+2] * 0.3 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)+1] * 0.59 + view_data[k][(biHeight[k]-1)*biAlign[k]+3*(u_k+m)] * 0.11;
								}												
							}
						}
					}
				}
				//�ں����������Sobel��Ĥ����ֱ�õ�u,v�����ƫ��
				//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���ƫ��
				derive_x[k][i] = gray_k[2]-gray_k[0]+2*(gray_k[5]-gray_k[3])+gray_k[8]-gray_k[6];
				derive_y[k][i] = gray_k[6]-gray_k[0]+2*(gray_k[7]-gray_k[1])+gray_k[8]-gray_k[2];	
				//�������񶥵��Ӧ�ĻҶȣ�ʵ���������ڸ���ͼ�ж�Ӧ���ػҶȵļ�Ȩƽ��
				vtx_gray[i] += weight[k][i]*vtx_map_gray[k][i];
			}
		}
	}
	
	data_cmpt = true;
}

void COGLEventDoc::Jacobian_Construction(CVisDMatrix& Jac_Mat)
{
	int i, j, k, l, m, n, u, v;
	CVisDVector pst(3);//��¼�뵱ǰ���������ض�Ӧ�����񶥵�Ŀռ�����
	CVisDVector nml(3);//��¼�뵱ǰ���������ض�Ӧ�����񶥵�ķ�����
	double cmpt[3];//��¼(x,y,z,1)��ͶӰ���������������ڻ�
	int counted_row = 0, counted_col, cur_cnt_fun;//��¼�ڱ���������ÿ�ֱ�����ʼǰ�Ѽ���������������͵�ǰ��һ�����Ѽ���������ĸ���
	
	//���ѭ������ͼָ�������
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;
		//��ʼ�����ڶ���ѭ�������ڸ���ͼ
		for(v = 0; v < biHeight[k]; v++)//������
		{
			for(u = 0; u < biWidth[k]; u++)//������
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//��һ��ӵ�ж�Ӧ���񶥵�����ع�������Jacaobian Matrix�е���ӦԪ��
				{
					//��pxl_map���Եõ���ǰ��ͼ�и������Ƿ���ڶ�Ӧ���񶥵㣬�Լ������ڶ�Ӧ���񶥵㣬�����
					//�ռ�����ͷ�������Ҫ�õ�
					for(j = 0; j < 3; j++)
					{
						pst[j] = face1_points[pxl_map[k][v*biWidth[k]+u]][j];
						nml[j] = face1_normals[pxl_map[k][v*biWidth[k]+u]][j];
					}
					//�������ĶԸ�������ģ��
					//�Ը�����ͼ����
					counted_col = 0;//��ʼ��
					for(i = 0; i < num_view; i++)
					{
						if(vtx_map[i][pxl_map[k][v*biWidth[k]+u]][0] != -1)
						{
							//�����ĸ����								
							cmpt[0] = pst[0]*inter_mat_list[i][0][0]+pst[1]*inter_mat_list[i][0][1]+pst[2]*inter_mat_list[i][0][2]+inter_mat_list[i][0][3];								
							cmpt[1] = pst[0]*inter_mat_list[i][1][0]+pst[1]*inter_mat_list[i][1][1]+pst[2]*inter_mat_list[i][1][2]+inter_mat_list[i][1][3];								
							cmpt[2] = pst[0]*inter_mat_list[i][2][0]+pst[1]*inter_mat_list[i][2][1]+pst[2]*inter_mat_list[i][2][2]+inter_mat_list[i][2][3];								
							//���кű���
							for(l = 0; l < 3; l++)
							{
								switch(l)
								{
								case 0:
									//���кű�����������4������ǰ3��
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+m] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_y[i][pxl_map[k][v*biWidth[k]+u]]*pst[m]/cmpt[2];
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+3] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_y[i][pxl_map[k][v*biWidth[k]+u]]/cmpt[2];
									break;
								case 1:
									//���кű�����������4������ǰ3��
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+4+m] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_x[i][pxl_map[k][v*biWidth[k]+u]]*pst[m]/cmpt[2];
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+4+3] = weight[i][pxl_map[k][v*biWidth[k]+u]]*derive_x[i][pxl_map[k][v*biWidth[k]+u]]/cmpt[2];
									break;
								case 2:
									//���кű�����������4������ǰ3��
									//��ʼ��
									for(m = 0; m < 3; m++)
										Jac_Mat[counted_row+cur_cnt_fun][counted_col+8+m] = -pst[m]*weight[i][pxl_map[k][v*biWidth[k]+u]]*(derive_x[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[1]+derive_y[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[0])/(cmpt[2]*cmpt[2]);
									Jac_Mat[counted_row+cur_cnt_fun][counted_col+8+3] = -weight[i][pxl_map[k][v*biWidth[k]+u]]*(derive_x[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[1]+derive_y[i][pxl_map[k][v*biWidth[k]+u]]*cmpt[0])/(cmpt[2]*cmpt[2]);
									break;
								default:
									{}
								}																
							}							
						}
						else//�����k����ͼ�еĵ�(u,v)���ض�Ӧ������ģ�Ͷ����ڵ�i����ͼ�в��ɼ��أ���Ȼ��������󵼶���0��
						{
							for(m = 0; m < 12; m++)
							{
								Jac_Mat[counted_row+cur_cnt_fun][counted_col+m] = 0;
							}
								
						}
						counted_col += 12;//��ɶ��뵱ǰ������ͼ��Ӧ��ͶӰ�����12����������
					}					
					cur_cnt_fun++;//��¼�Ѽ�����뵱ǰ��ͼ��ص�����
				}
			}
		}
		counted_row += map_pxl_num[k];//��ɶ��뵱ǰ������ͼ��Ӧ��������������
	}
}

void COGLEventDoc::Err_Fun_Computation(CVisDMatrix& cur_fun_vct)
{
	int k, u, v;
	int counted_row = 0, cur_cnt_fun;//, cur_sum_fun;//��¼�ڱ���������ÿ�ֱ�����ʼǰ�Ѽ���������������͵�ǰ��һ�����Ѽ���������ĸ���
	
	//�ڹ����Jacobian����Ļ����ϣ����Ա�дLM�㷨�Ĵ���
	double cur_gray;//��ǰ�������صĻҶ�ֵ
	//������������
	//��ʼ��
	counted_row = 0;
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;//��ʼ��
		for(v = 0; v < biHeight[k]; v++)//������
		{
			for(u = 0; u < biWidth[k]; u++)//������
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//��һ��ӵ�ж�Ӧ���񶥵�����ع�������Jacaobian Matrix�е���ӦԪ��
				{
					//��������ػҶ������Ӧ�����񶥵����ɫ��Ϣ֮���ƽ��
					cur_gray = view_data[k][v*biAlign[k]+3*u+2] * 0.3 + view_data[k][v*biAlign[k]+3*u+1] * 0.59 + view_data[k][v*biAlign[k]+3*u] * 0.11;										
					//������������ֵ��ע��ƽ��
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
	
	//�ڹ����Jacobian����Ļ����ϣ����Ա�дLM�㷨�Ĵ���
	double cur_gray;//��ǰ�������صĻҶ�ֵ
	//������������
	//��ʼ��
	cur_sum_fun = 0;
	for(k = 0; k < num_view; k++)
	{
		cur_cnt_fun = 0;//��ʼ��
		for(v = 0; v < biHeight[k]; v++)//������
		{
			for(u = 0; u < biWidth[k]; u++)//������
			{
				if(pxl_map[k][v*biWidth[k]+u] != -1)//��һ��ӵ�ж�Ӧ���񶥵�����ع�������Jacaobian Matrix�е���ӦԪ��
				{
					//��������ػҶ������Ӧ�����񶥵����ɫ��Ϣ֮���ƽ��
					cur_gray = view_data[k][v*biAlign[k]+3*u+2] * 0.3 + view_data[k][v*biAlign[k]+3*u+1] * 0.59 + view_data[k][v*biAlign[k]+3*u] * 0.11;										
					//���㵱ǰ���ͺ���
					cur_sum_fun += (cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]) * (cur_gray - vtx_gray[pxl_map[k][v*biWidth[k]+u]]);					
				}
			}
		}
	}
}

void COGLEventDoc::OnPxlComp() 
{
	int i, j, k, u, v, cnt = 0;//, l, m, n;//, u_k, v_k;
	
	//int** map_num = new int*[num_view];//��¼ÿ�����ض�Ӧ�����񶥵�ĸ���
	r3 = new CVisDVector[num_view];//��¼��Ӧ�ڸ���ͼ����ת����ĵ�����
	pxl_map = new int*[num_view];//��¼�ڸ���ͼ��������ض�Ӧ�����񶥵����ţ����޶�Ӧ���¼Ϊ-1
	vtx_map = new int**[num_view];//��¼�����񶥵��ڸ���ͼ�ж�Ӧ�����ص�����
	map_pxl_num = new unsigned long[num_view];//��¼�ڸ���ͼ���ж�Ӧ���񶥵�����صĸ���
		
	//�����ڴ�ռ估��ʼ��
	for(k = 0; k < num_view; k++)
	{
		r3[k].Resize(3);//����Ϊ��Ԫ������
		map_pxl_num[k] = 0;//��ʼ����һ��Ҫע���
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
		
		//��ʼ��
		for(v = 0; v < biHeight[k]; v++)
		{
			for(u = 0; u < biWidth[k]; u++)
			{
				pxl_map[k][v*biWidth[k]+u] = -1;
				//map_num[k][v*biWidth[k]+u] = 0;
			}			
		}
	}
	
	//����Jacobian Matrix
	CVisDMatrix Jac_Mat(1, 12*num_view);//�����������ĸ����������Ǳ����ĸ������༴�����ͼ��Ӧ��ͶӰ����ĸ�Ԫ��
	
	//�ڹ����Jacobian����Ļ����ϣ����Ա�дLM�㷨�Ĵ���
	double para, incr_fct, err, pre_sum_fun, cur_sum_fun;//�������������������ӱ��������������ͺ���
	//CVisDMatrix pre_fun_vct(1, 1);//�������ɵ�����
	CVisDMatrix cur_fun_vct(1, 1);//�������ɵ�����
	CVisDMatrix proj_para(12*num_view, 1);//ͶӰ����������ɵ�����
	CVisDMatrix cur_proj_para(12*num_view, 1);//ͶӰ����������ɵ�����
	CVisDMatrix drt_vct(12*num_view, 1);//����������ע����ά��������ͶӰ�����������֮��
	CVisDMatrix unit_mat(12*num_view, 12*num_view);//��λ����
	CVisDMatrix grad_vct(12*num_view, 1);//1/2�ݶ�����
	//double cur_gray;//��ǰ�������صĻҶ�ֵ
	double vct_lg;//��¼������ģ��
	bool stop = FALSE;//��ǲ��������Ƿ����

	FILE* fp;
			
	deno_of_weight = new double[M];//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ�Ĺ�����ĸ
	weight = new double*[num_view];//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ
	for(k = 0; k < num_view; k++)
	{
		weight[k] = new double[M];
	}
	vtx_map_gray = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش��ĻҶ�
	for(k = 0; k < num_view; k++)
	{
		vtx_map_gray[k] = new double[M];
	}
	derive_x = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���X�����ƫ��
	for(k = 0; k < num_view; k++)
	{
		derive_x[k] = new double[M];
	}
	derive_y = new double*[num_view];//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���Y�����ƫ��
	for(k = 0; k < num_view; k++)
	{
		derive_y[k] = new double[M];
	}
	vtx_gray = new double[M];//��¼���񶥵��Ӧ�ĻҶ���Ϣ
	vis_img = new bool*[num_view];//��¼�뵱ǰ���������ض�Ӧ�����񶥵�����Щ��ͼ�пɼ����м�ÿ�α�����Ҫ���³�ʼ����
	for(k = 0; k < num_view; k++)
	{
		vis_img[k] = new bool[M];
	}

	//pst.Resize(3);
	//nml.Resize(3);
	
	//��ʼ��
	//����ͶӰ�����������
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
	
	//�ϴ����ȵ�Ҫ����������ݰ�����ô���ⲽ���ӵ���:(
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
			//�ȿ�����򵥵��ļ���ʽ
			//��ʼд������
			FILE* fp;
			fp = fopen(cstrFileName, "w");
			//д�붥����������
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
	
	//�������ͺ���
	Sum_Fun_Computation(cur_sum_fun);

	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
	
	fseek(fp, 0, SEEK_END);
	
	//д�붥����������
	fprintf(fp, "%lf\n", cur_sum_fun);				
	
	fclose(fp);	
	
	pre_sum_fun = cur_sum_fun;
	
	para /= incr_fct;
	
	//������������
	cur_fun_vct.Resize(map_pxl_num_sum, 1);
	Err_Fun_Computation(cur_fun_vct);
	
	Jac_Mat.Resize(map_pxl_num_sum, 12*num_view);
	Jacobian_Construction(Jac_Mat);//����Jacobian����
	
	//VisSetToIdentity(unit_mat);//��Ϊ��λ����
	
	for(i = 0; i < 12*num_view; i++)
	{
		for(j = 0; j < 12*num_view; j++)
		{
			if(i == j)	
				unit_mat[i][i] = para;//�Խ�����Ԫ������Ϊpara���൱��para*unit_mat
			else
				unit_mat[i][i] = 0;
		}
	}
	
	drt_vct = -(Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);

	//����ͶӰ�������
	cur_proj_para += drt_vct;

	grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//�����ݶ�����

	//�����ݶ�������ģ��
	vct_lg = 0;//��ʼ��
	for(i = 0; i < 12*num_view; i++)
	{		
		vct_lg += grad_vct[i][0]*grad_vct[i][0];
	} 
	vct_lg = sqrt(vct_lg);

	//��Ҫ����������º�����ͺ���
	//�ø��µ�ͶӰ�������cur_proj_para������ͶӰ����inter_mat_list,����������Ϊ�˼���cur_sum_fun
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

	Mapping();//��ȻӦ���ȸ��¶�Ӧ��ϵ���ٸ�����һ��Ӧ��ϵ��������ز���
	Data_Computation();
	
	//�������ͺ���
	Sum_Fun_Computation(cur_sum_fun);
	
	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
	fseek(fp, 0, SEEK_END);
	
	//д�붥����������
	fprintf(fp, "%lf\n", cur_sum_fun);
	
	fprintf(fp, "%d\n", map_pxl_num_sum);
			
	fclose(fp);
				
	cnt++;
		
	fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
	fseek(fp, 0, SEEK_END);		
	//д�붥����������
	fprintf(fp, "%d\n", cnt);	
	fclose(fp);
	
	while(!stop)//cnt < 1000)
	{
		while(cur_sum_fun < pre_sum_fun)//���ͼ�С
		{
			//��ȷ��Ҫ����proj_para
			proj_para = cur_proj_para;
				
			if(vct_lg <= err)//�ݶ�С��������ͶӰ������������
			{
				stop = TRUE;				
				break;
			}
			else
			{
				pre_sum_fun = cur_sum_fun;
				
				para /= incr_fct;
				
				//������������
				cur_fun_vct.Resize(map_pxl_num_sum, 1);
				Err_Fun_Computation(cur_fun_vct);
				
				Jac_Mat.Resize(map_pxl_num_sum, 12*num_view);
				Jacobian_Construction(Jac_Mat);//����Jacobian����
				
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
							//�ȿ�����򵥵��ļ���ʽ
							//��ʼд������
							FILE* fp;
							fp = fopen(cstrFileName, "w");
							//д�붥����������
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
				//VisSetToIdentity(unit_mat);//��Ϊ��λ����
				
				for(i = 0; i < 12*num_view; i++)
				{
					for(j = 0; j < 12*num_view; j++)
					{
						if(i == j)	
							unit_mat[i][i] = para;//�Խ�����Ԫ������Ϊpara���൱��para*unit_mat
						else
							unit_mat[i][i] = 0;
					}
				}

				if(cnt == 93)
				{
					//�ⷽ��
					CVisDMatrix itmd(12*num_view, 12*num_view);
					itmd = Jac_Mat.Transposed()*Jac_Mat;
					
					fp = fopen("G:\\Data\\research\\exhibition\\skull\\output.txt", "r+");
					
					fseek(fp, 0, SEEK_END);					
					
					//д�붥����������
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
				
				//�ⷽ��
				drt_vct = -(Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);
				
				//����ͶӰ�������
				cur_proj_para += drt_vct;
				
				grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//�����ݶ�����

				//�����ݶ�������ģ��	
				vct_lg = 0;//��ʼ��
				for(i = 0; i < 12*num_view; i++)
				{		
					vct_lg += grad_vct[i][0]*grad_vct[i][0];
				} 
				vct_lg = sqrt(vct_lg);

				//��Ҫ����������º�����ͺ���
				//�ø��µ�ͶӰ�������proj_para������ͶӰ����inter_mat_list
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
				Mapping();//��ȻӦ���ȸ��¶�Ӧ��ϵ���ٸ�����һ��Ӧ��ϵ��������ز���
				Data_Computation();
				//�������ͺ���
				Sum_Fun_Computation(cur_sum_fun);

				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
				fseek(fp, 0, SEEK_END);
	
				//д�붥����������
				fprintf(fp, "%lf\n", cur_sum_fun);				
				
				fprintf(fp, "%d\n", map_pxl_num_sum);
			
				fclose(fp);

				cnt++;
				
				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
				fseek(fp, 0, SEEK_END);		
				//д�붥����������
				fprintf(fp, "%d\n", cnt);	
				fclose(fp);	

				if(cnt == 5 || cnt == 13)
				{
					cnt = cnt;
				}
			}
		}

		while(cur_sum_fun >= pre_sum_fun)//����δ��С
		{
			//���ǲ��������κ�����,����ͶӰ����ҲҪ��ԭ
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

			if(vct_lg <= err)//�ݶ�С��������ͶӰ������������
			{
				stop = TRUE;				
				break;
			}
			else
			{
				//���ﲢ����Ҫ����
				//pre_sum_fun = cur_sum_fun;

				para *= incr_fct;

				//VisSetToIdentity(unit_mat);//��Ϊ��λ����
				
				for(i = 0; i < 12*num_view; i++)
				{
					for(j = 0; j < 12*num_view; j++)
					{
						if(i == j)	
							unit_mat[i][i] = para;//�Խ�����Ԫ������Ϊpara���൱��para*unit_mat
						else
							unit_mat[i][i] = 0;
					}
				}				
				
				if(cnt == 93)
				{
					//�ⷽ��
					CVisDMatrix itmd(12*num_view, 12*num_view);
					itmd = Jac_Mat.Transposed()*Jac_Mat;
					
					fp = fopen("G:\\Data\\research\\exhibition\\skull\\output.txt", "r+");
					
					fseek(fp, 0, SEEK_END);					
					
					//д�붥����������
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

				//�ⷽ��
				else drt_vct = (Jac_Mat.Transposed()*Jac_Mat+unit_mat).Inverted()*(Jac_Mat.Transposed()*cur_fun_vct);
				
				//����ͶӰ�������
				cur_proj_para += drt_vct;
				
				/*
				//����Jac_Mat��cur_fun_vct��û�и��£����Բ������¼���
				grad_vct = Jac_Mat.Transposed()*cur_fun_vct;//�����ݶ�����

				//�����ݶ�������ģ��	
				vct_lg = 0;//��ʼ��
				for(i = 0; i < 12*num_view; i++)
				{		
					vct_lg += grad_vct[i][0]*grad_vct[i][0];
				} 
				vct_lg = sqrt(vct_lg);
				*/

				//��Ҫ����������º�����ͺ���
				//�ø��µ�ͶӰ�������proj_para������ͶӰ����inter_mat_list
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
				
				Mapping();//��ȻӦ���ȸ��¶�Ӧ��ϵ���ٸ�����һ��Ӧ��ϵ��������ز���
				Data_Computation();
				//�������ͺ���
				Sum_Fun_Computation(cur_sum_fun);

				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");
				
				fseek(fp, 0, SEEK_END);
	
				//д�붥����������
				fprintf(fp, "%lf\n", cur_sum_fun);				
				
				fprintf(fp, "%d\n", map_pxl_num_sum);
			
				fclose(fp);
				
				cnt++;
				
				fp = fopen("G:\\Data\\research\\exhibition\\skull\\mat_output.txt", "r+");		
				fseek(fp, 0, SEEK_END);		
				//д�붥����������
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
		//���������ε��������㼰�䷨����					
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
		//���������ε��������㼰�䷨����					
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
