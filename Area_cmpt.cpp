// Area_cmpt.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Area_cmpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Area_cmpt dialog


Area_cmpt::Area_cmpt(CWnd* pParent /*=NULL*/)
	: CDialog(Area_cmpt::IDD, pParent)
{
	//{{AFX_DATA_INIT(Area_cmpt)
	m_area_value = _T("");
	//}}AFX_DATA_INIT
}


void Area_cmpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Area_cmpt)
	DDX_Text(pDX, IDC_AREA_VALUE, m_area_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Area_cmpt, CDialog)
	//{{AFX_MSG_MAP(Area_cmpt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Area_cmpt message handlers
