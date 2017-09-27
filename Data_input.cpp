// Data_input.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Data_input.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Data_input dialog


Data_input::Data_input(CWnd* pParent /*=NULL*/)
	: CDialog(Data_input::IDD, pParent)
{
	//{{AFX_DATA_INIT(Data_input)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Data_input::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Data_input)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Data_input, CDialog)
	//{{AFX_MSG_MAP(Data_input)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Data_input message handlers
