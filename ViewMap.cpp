// ViewMap.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "ViewMap.h"
#include "OGLEventDoc.h"
#include "OGLEventView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewMap dialog


ViewMap::ViewMap(CWnd* pParent /*=NULL*/)
	: CDialog(ViewMap::IDD, pParent)
{
	ASSERT(pParent);
	m_pParent=pParent;
	//{{AFX_DATA_INIT(ViewMap)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void ViewMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ViewMap)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ViewMap, CDialog)
	//{{AFX_MSG_MAP(ViewMap)
		ON_WM_DESTROY()
	ON_BN_CLICKED(ID_POSE_ADJ, OnPoseAdj)
	ON_BN_CLICKED(ID_PROJ_MAPPING, OnProjMapping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewMap message handlers
void ViewMap::OnOK()
{
	CDialog::OnOK();
	DestroyWindow();
}

void ViewMap::OnDestroy() 
{
	CDialog::OnDestroy();
	//AfxMessageBox("call delete this");
	delete this;
}

void ViewMap::OnPoseAdj() 
{
	COGLEventDoc* pDoc = ((COGLEventView *)m_pParent)->GetDocument();
	ASSERT_VALID(pDoc);		

	pDoc->pose_adj = true;
}



void ViewMap::OnProjMapping() 
{
	COGLEventDoc* pDoc = ((COGLEventView *)m_pParent)->GetDocument();
	ASSERT_VALID(pDoc);		

	pDoc->pose_adj = false;

	pDoc->textured = true;		
	pDoc->TxtMapping();
	pDoc->UpdateAllViews(NULL);
}
