// Vlmn_cmpt.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Vlmn_cmpt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Vlmn_cmpt dialog


Vlmn_cmpt::Vlmn_cmpt(CWnd* pParent /*=NULL*/)
	: CDialog(Vlmn_cmpt::IDD, pParent)
{
	//{{AFX_DATA_INIT(Vlmn_cmpt)
	m_vlmn_value = _T("");
	//}}AFX_DATA_INIT
}


void Vlmn_cmpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Vlmn_cmpt)
	DDX_Text(pDX, IDC_VLMN_VALUE, m_vlmn_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Vlmn_cmpt, CDialog)
	//{{AFX_MSG_MAP(Vlmn_cmpt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Vlmn_cmpt message handlers
