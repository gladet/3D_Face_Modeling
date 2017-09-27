// Operation.cpp : implementation file
//

#include "stdafx.h"
#include "OGLEvent.h"
#include "Operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Operation

IMPLEMENT_DYNCREATE(Operation, CFormView)

Operation::Operation()
	: CFormView(Operation::IDD)
{
	//{{AFX_DATA_INIT(Operation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

Operation::~Operation()
{
}

void Operation::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Operation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Operation, CFormView)
	//{{AFX_MSG_MAP(Operation)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Operation diagnostics

#ifdef _DEBUG
void Operation::AssertValid() const
{
	CFormView::AssertValid();
}

void Operation::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Operation message handlers
