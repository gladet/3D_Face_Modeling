// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OGLEvent.h"

#include "MainFrm.h"
#include "FormCommandView1.h"
#include "OGLEventView.h"
#include "Texture.h"
#include "Operation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

//IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

/*
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// First splitter pane
	if (!m_wndSplitter.CreateView(0, 0,
		RUNTIME_CLASS(CFormCommandView1), CSize(260,0), pContext))
	{
		TRACE("Failed to create command view pane\n");
		return FALSE;
	}

	if (!m_wndSplitter1.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1)))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// Second splitter pane
	if (!m_wndSplitter1.CreateView(0, 0,
		RUNTIME_CLASS(Operation), CSize(0,75), pContext))
	{
		TRACE("Failed to create command view pane\n");
		return FALSE;
	}
	
	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter1, 1, 2, WS_CHILD | WS_VISIBLE, m_wndSplitter1.IdFromRowCol(1, 0)))
	{
		TRACE("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// Third splitter pane
	if (!m_wndSplitter2.CreateView(0, 0,
		RUNTIME_CLASS(COGLEventView), CSize(480,0), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}

	// Fourth splitter pane
	if (!m_wndSplitter2.CreateView(0, 1,
		RUNTIME_CLASS(CTexture), CSize(0,0), pContext))
	{
		TRACE("Failed to create preview pane\n");
		return FALSE;
	}
	return TRUE;
}