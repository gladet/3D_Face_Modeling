// Txt_map.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Txt_map.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Txt_map dialog


Txt_map::Txt_map(CWnd* pParent /*=NULL*/)
	: CDialog(Txt_map::IDD, pParent)
{
	//{{AFX_DATA_INIT(Txt_map)
	m_width = 0;
	//}}AFX_DATA_INIT
}


void Txt_map::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Txt_map)
	DDX_Text(pDX, IDC_W, m_width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Txt_map, CDialog)
	//{{AFX_MSG_MAP(Txt_map)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Txt_map message handlers
