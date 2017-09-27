// OGLEventDoc.h : interface of the COGLEventDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGLEVENTDOC_H__0854AB6B_1433_4D85_A31B_C6F36BB7D34B__INCLUDED_)
#define AFX_OGLEVENTDOC_H__0854AB6B_1433_4D85_A31B_C6F36BB7D34B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VisCore.h"
#include "VisMatrix.h"

#include <list>

using namespace std;

typedef list<double> list_db;
typedef list<int> list_int;
typedef list<double*> list_dbpt;
typedef list<int*> list_intpt;
typedef list<unsigned char*> list_charpt;

typedef list<double**> list_dbpt_2;
typedef list<list_intpt*> list_llippt;//��list_intptָ��Ϊ�ڵ�Ԫ�ص�����
typedef list<CVisDMatrix*> list_matpt;//��CVisDMatrixָ��Ϊ�ڵ�Ԫ�ص�����

#define PI 3.14159265

class COGLEventDoc : public CDocument
{
protected: // create from serialization only
	COGLEventDoc();
	DECLARE_DYNCREATE(COGLEventDoc)

// Attributes
public:

// Operations
public:
	double **face1_points;
	double **face1_normals;

	int **triangles1;
	int **nml_tris;
	int **txt_tris;

	int M;
	int M1;
	int L1;
	int K1;

	/*
	double **face1_texture;
	double **face1_texture_r;
	double **face1_texture_l;
	*/
	//double* view_inner[3]; 
	list_dbpt_2 face1_texture;//���ڴ�������ͼ��Ӧ����������
	list_dbpt view_inner;//�����������ͼ��Ӧ�ı��淨���������ߵ��ڻ�ֵ
	double **texture_map;
	double **txt_map;
	bool m_read;
	int m_style;
	int which_face;
	bool textured;//�ж��Ƿ�������ĳ����ͼ��Ӧ������ͼ������������������ͼ��ǰ��
	bool img_loaded;
	
	bool if_rot, obj_fp_lc, img_fp_lc;
	
	CPoint img_ft_pt[2];//ͼ��������ľ�������ƽ��ֵ
	CPoint img_temp_pt[8];//ͼ��������ľ�������
	int img_pt_num;

	double mdl_ft_pt[2][3];
	double pre_mdl_ft_pt[2][3];	
	double mdl_temp_pt[8][3];
	int mdl_pt_num;
	int pre_pt_num;

	list_dbpt mdl_pts;
	list_dbpt pre_mdl_pts;//��¼��һ���ı궨��
	list_dbpt act_mdl_pts;//��¼ʵ��ӳ�����ͼ��������������Ӧ��ģ�͵������ֵ
	list_intpt img_pts, img_pt_cd;
	list_intpt act_img_pts;//��¼ʵ��ӳ�����ģ��������������Ӧ��ͼ��������ֵ
	//list_int img_pt_num_list;

	double D_E;
	int I_D_E;
	double D_M;	
	
	double r_x[3];
	double r_y[3];
	double r_z[3];

	double Max_V;
	
	CVisDVector N_E;
	CVisDVector N_M;
	CVisDVector N_S;
	CVisDVector N_F;

	bool transformed;//�ж��Ƿ������λ�˵���

	bool pose_adj;
	float x_rot;
	float y_rot;
	float z_rot;

	int which_view;//��ʶ����һ����ͼ	
	int num_view;//��ʶ��ͼ����Ŀ
	int which_loc;//��ʶ����һ�α궨

	int width;
	int height;

	bool cylindrical_map;
	bool txt_mapped;
	bool mesh_projed;

	bool with_txt;//�ж�obj�ļ����Ƿ��Ѹ���������Ϣ
	bool txt_type;//�ж�obj�ļ���������Ϣ��Я����ʽ

	CVisDMatrix rot_matrix;
	CVisDMatrix inter_matrix;//���ڼ�¼�����м�����ľ���

	list_llippt img_pts_list;//��¼�����ͼ��Ӧ�ı������
	list_llippt img_pt_cd_list;//��¼�����ͼ��Ӧ�ı������
	//list_matpt inter_mat_list;//��¼�����ͼ��Ӧ�ĵ�ӳ�����
	CVisDMatrix* inter_mat_list;//��¼�����ͼ��Ӧ�ĵ�ӳ�����
	list_intpt* temp_img_pts;//��¼�뵱ǰ��ͼ��Ӧ�ı�������ָ��
	list_intpt* temp_img_pt_cd;//��¼�뵱ǰ��ͼ��Ӧ�ı�������ָ��
	CVisDMatrix* temp_inter_mat;//��¼�뵱ǰ��ͼ��Ӧ�ĵ�ӳ������ָ��	

	int temp_img_pt_num;//��¼��ǰ��ͼ��������ǵĵ���
	list_int img_pt_num_list;//��¼����ͼ��������ǵĵ���

	bool** Vis_Map;//���ģ�����ѱ���ĸ����ڸ���ͼ�еĿɼ���
	int* Vis_Num;//���ģ�����ѱ���ĸ��������пɼ�����ͼ����Ŀ
	int*** img_pts_mat;//���ģ�����ѱ���ĸ����ڸ���ͼ�ж�Ӧ��ǵ���������

	double** opt_mdl_pts;//��¼�Ż���ģ�����ѱ���ĸ��������
	double** mdl_pts_array;//��¼�Ż�ǰ(��Զ���)�ѱ���ĸ��������
	bool mdl_pts_opted;//��־ģ�����ѱ�Ǹ����Ƿ��Ż�
	bool optimized;//��־�����ͼ��Ӧ��ӳ������Ƿ��Ż�

	int iter_index;//���ڱ����ͼ�����

	unsigned char* temp_view;//��ǵ�ǰ��ͼ�����ڴ��ָ��

	double** temp_txt;//���ģ�Ͷ�Ӧ�ڵ�ǰ��ͼ����������

	int*** act_img_pts_list;//��Ƕ�Ӧ�ڸ���ͼ��ʵ�ʵ�ͼ����ӳ���

	double*** mdl_pts_trace;//��¼����������ģ���ϱ�ǵ���ƶ��켣
	int iter_num, temp_iter_num;//��������
	bool iter_begin;
	bool iter_over;

	CVisDVector* r3;//��¼�����ͼ��Ӧ����ת����ĵ����У���һ�����������ָ��
	int** pxl_map;//��¼�뵱ǰ��ͼ�ĸ����ض�Ӧ�����񶥵�����
	int*** vtx_map;//��¼�����񶥵��Ӧ�ĵ�ǰ��ͼ�ĸ����ص�����

	bool mapped;//ȷ������ͼ���������񶥵��Ķ�Ӧ��ϵ�Ƿ��Ѿ�����
	bool data_cmpt;//ȷ��������Ӧ��ϵ֮���Ƿ������������ݵļ���
	unsigned long* map_pxl_num;//��¼�ڸ�����ͼ��ȷ����Ӧ���񶥵�����ص���Ŀ
	unsigned long map_pxl_num_sum;//��¼�ڸ�����ͼ��ȷ����Ӧ���񶥵�����ص���Ŀ֮�ͣ�ʵ����Ҳ���ܵ������ĸ���

	unsigned char** view_data;//�洢����ͼͼ�����ݵ�ָ�룬ʹ����������ڲ�����
	unsigned long* biHeight;//�洢����ͼͼ��ĸ߶�
	unsigned long* biWidth;//�洢����ͼͼ��Ŀ��
	unsigned long* biAlign;

	//double* vtx_clr;//��¼���񶥵�ĻҶ���Ϣ

	double* deno_of_weight;//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ���ֵĹ�����ĸ
	double** weight;//��¼���񶥵��Ӧ�ڸ���ͼ��Ȩֵ
	double** vtx_map_gray;//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش��ĻҶ�
	double** derive_x;//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���X�����ƫ��
	double** derive_y;//��¼���񶥵��ڸ���ͼ�ж�Ӧ���ش���Y�����ƫ��
	double* vtx_gray;//��¼���񶥵��Ӧ�ĻҶ���Ϣ

	bool** vis_img;//��¼�뵱ǰ���������ض�Ӧ�����񶥵�����Щ��ͼ�пɼ����м�ÿ�α�����Ҫ���³�ʼ����

	//CVisDVector pst;//��¼�뵱ǰ���������ض�Ӧ�����񶥵�Ŀռ�����
	//CVisDVector nml;//��¼�뵱ǰ���������ض�Ӧ�����񶥵�ķ�����
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLEventDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COGLEventDoc();
	CView *GetOGLEventView(); 
	CView *GetTextureView();
	CView *GetFormCommandView();
	void TxtMapping();
	void Tran_To_Center();	
	void Mapping();
	void Data_Computation();
	void Jacobian_Construction(CVisDMatrix& Jac_Mat);
	void Sum_Fun_Computation(double& cur_sum_fun);
	void COGLEventDoc::Err_Fun_Computation(CVisDMatrix& cur_fun_vct);

	unsigned int current_size;
	unsigned char* m_pBitmap;
	unsigned char* m_pBkgd;//ָ��洢����ͼ����ڴ�ռ�
	unsigned char* m_pTransfered;
	list_charpt view_pt;
	CString m_strFileName;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL ReadBitmap(LPCTSTR lpszPathName, unsigned char*& buffer);
	void ReadObj(double **& data_points, double **& data_normals, int **& triangles, int& num_points, int& num_normals, int& num_tris);
	bool ReadObj_Extd(double **& data_points, double **& data_normals, double **& data_texture, int **& triangles, int **& nml_tris, int **& txt_tris, int& num_points, int& num_normals, int& num_txt, int& num_tris, bool& with_txt);
	void Normalize(CVisDVector& vec);
	double Length(CVisDVector vec);
	//void Texture_Mapping();

// Generated message map functions
protected:
	//{{AFX_MSG(COGLEventDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnReadModel();
	afx_msg void OnPtLoc1();
	afx_msg void OnPtLoc2();
	afx_msg void OnProjMap();
	afx_msg void OnTxtBlend();
	afx_msg void OnReadImage();
	afx_msg void OnPoseAdj();
	afx_msg void OnRightView();
	afx_msg void OnLeftView();
	afx_msg void OnSave();
	afx_msg void OnSaveData();
	afx_msg void OnDataAlalysis();
	afx_msg void OnPrmtOpt();
	afx_msg void OnInvisible();
	afx_msg void OnIterative();
	afx_msg void OnNextview();
	afx_msg void OnSaveimage();
	afx_msg void OnReadBkgd();
	afx_msg void OnReadFgd();
	afx_msg void OnImgDiff();
	afx_msg void OnMap();
	afx_msg void OnPxlComp();
	afx_msg void OnAreaCmpt();
	afx_msg void OnVlmnCmpt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGLEVENTDOC_H__0854AB6B_1433_4D85_A31B_C6F36BB7D34B__INCLUDED_)
