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
typedef list<list_intpt*> list_llippt;//以list_intpt指针为节点元素的链表
typedef list<CVisDMatrix*> list_matpt;//以CVisDMatrix指针为节点元素的链表

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
	list_dbpt_2 face1_texture;//用于存放与各视图对应的纹理坐标
	list_dbpt view_inner;//用与存放与各视图对应的表面法向量与视线的内积值
	double **texture_map;
	double **txt_map;
	bool m_read;
	int m_style;
	int which_face;
	bool textured;//判断是否生成与某个视图对应的纹理图，这是生成柱体纹理图的前提
	bool img_loaded;
	
	bool if_rot, obj_fp_lc, img_fp_lc;
	
	CPoint img_ft_pt[2];//图像特征点的绝对坐标平均值
	CPoint img_temp_pt[8];//图像特征点的绝对坐标
	int img_pt_num;

	double mdl_ft_pt[2][3];
	double pre_mdl_ft_pt[2][3];	
	double mdl_temp_pt[8][3];
	int mdl_pt_num;
	int pre_pt_num;

	list_dbpt mdl_pts;
	list_dbpt pre_mdl_pts;//记录归一化的标定点
	list_dbpt act_mdl_pts;//记录实际映射后与图像上特征点所对应的模型点的坐标值
	list_intpt img_pts, img_pt_cd;
	list_intpt act_img_pts;//记录实际映射后与模型上特征点所对应的图像点的坐标值
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

	bool transformed;//判断是否进行了位姿调整

	bool pose_adj;
	float x_rot;
	float y_rot;
	float z_rot;

	int which_view;//标识是哪一个视图	
	int num_view;//标识视图的数目
	int which_loc;//标识是哪一次标定

	int width;
	int height;

	bool cylindrical_map;
	bool txt_mapped;
	bool mesh_projed;

	bool with_txt;//判断obj文件中是否已附带纹理信息
	bool txt_type;//判断obj文件中纹理信息的携带方式

	CVisDMatrix rot_matrix;
	CVisDMatrix inter_matrix;//用于记录包含中间参数的矩阵

	list_llippt img_pts_list;//记录与各视图对应的标点链表
	list_llippt img_pt_cd_list;//记录与各视图对应的标点链表
	//list_matpt inter_mat_list;//记录与各视图对应的的映射矩阵
	CVisDMatrix* inter_mat_list;//记录与各视图对应的的映射矩阵
	list_intpt* temp_img_pts;//记录与当前视图对应的标点链表的指针
	list_intpt* temp_img_pt_cd;//记录与当前视图对应的标点链表的指针
	CVisDMatrix* temp_inter_mat;//记录与当前视图对应的的映射矩阵的指针	

	int temp_img_pt_num;//记录当前视图中真正标记的点数
	list_int img_pt_num_list;//记录各视图中真正标记的点数

	bool** Vis_Map;//标记模型上已标出的各点在各视图中的可见性
	int* Vis_Num;//标记模型上已标出的各点在其中可见的视图的数目
	int*** img_pts_mat;//标记模型上已标出的各点在各视图中对应标记的像素坐标

	double** opt_mdl_pts;//记录优化后模型上已标出的各点的坐标
	double** mdl_pts_array;//记录优化前(相对而言)已标出的各点的坐标
	bool mdl_pts_opted;//标志模型上已标记各点是否被优化
	bool optimized;//标志与各视图对应的映射矩阵是否被优化

	int iter_index;//用于标记视图的序号

	unsigned char* temp_view;//标记当前视图数据内存的指针

	double** temp_txt;//标记模型对应于当前视图的纹理坐标

	int*** act_img_pts_list;//标记对应于各视图的实际的图像上映射点

	double*** mdl_pts_trace;//记录迭代过程中模型上标记点的移动轨迹
	int iter_num, temp_iter_num;//迭代次数
	bool iter_begin;
	bool iter_over;

	CVisDVector* r3;//记录与各视图对应的旋转矩阵的第三行，是一个向量数组的指针
	int** pxl_map;//记录与当前视图的各像素对应的网格顶点的序号
	int*** vtx_map;//记录与网格顶点对应的当前视图的各像素的坐标

	bool mapped;//确定各视图像素与网格顶点间的对应关系是否已经建立
	bool data_cmpt;//确定建立对应关系之后是否进行了相关数据的计算
	unsigned long* map_pxl_num;//记录在各幅视图中确定对应网格顶点的像素的数目
	unsigned long map_pxl_num_sum;//记录在各幅视图中确定对应网格顶点的像素的数目之和，实际上也是总的误差函数的个数

	unsigned char** view_data;//存储各视图图像数据的指针，使用数组更易于操作？
	unsigned long* biHeight;//存储各视图图像的高度
	unsigned long* biWidth;//存储各视图图像的宽度
	unsigned long* biAlign;

	//double* vtx_clr;//记录网格顶点的灰度信息

	double* deno_of_weight;//记录网格顶点对应于各视图的权值部分的公共分母
	double** weight;//记录网格顶点对应于各视图的权值
	double** vtx_map_gray;//记录网格顶点在各视图中对应像素处的灰度
	double** derive_x;//记录网格顶点在各视图中对应像素处沿X方向的偏导
	double** derive_y;//记录网格顶点在各视图中对应像素处沿Y方向的偏导
	double* vtx_gray;//记录网格顶点对应的灰度信息

	bool** vis_img;//记录与当前所遍历像素对应的网格顶点在哪些视图中可见，切记每次便利都要重新初始化！

	//CVisDVector pst;//记录与当前所遍历像素对应的网格顶点的空间坐标
	//CVisDVector nml;//记录与当前所遍历像素对应的网格顶点的法向量
	
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
	unsigned char* m_pBkgd;//指向存储背景图像的内存空间
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
