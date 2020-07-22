
// SetupView.cpp : implementation of the CSetupView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Setup.h"
#endif

#include "SetupDoc.h"
#include "SetupView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSetupView

IMPLEMENT_DYNCREATE(CSetupView, CView)

BEGIN_MESSAGE_MAP(CSetupView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSetupView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CSetupView construction/destruction

CSetupView::CSetupView() noexcept
{
	// TODO: add construction code here

}

CSetupView::~CSetupView()
{
}

BOOL CSetupView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSetupView drawing

void CSetupView::OnDraw(CDC* /*pDC*/)
{
	CSetupDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSetupView printing


void CSetupView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSetupView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSetupView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSetupView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSetupView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSetupView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSetupView diagnostics

#ifdef _DEBUG
void CSetupView::AssertValid() const
{
	CView::AssertValid();
}

void CSetupView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSetupDoc* CSetupView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSetupDoc)));
	return (CSetupDoc*)m_pDocument;
}
#endif //_DEBUG


// CSetupView message handlers
