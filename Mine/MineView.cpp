
// MineView.cpp : CMineView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Mine.h"
#endif

#include "MineDoc.h"
#include "MineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMineView

IMPLEMENT_DYNCREATE(CMineView, CView)

BEGIN_MESSAGE_MAP(CMineView, CView)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMineView 构造/析构

CMineView::CMineView()
{
	// TODO: 在此处添加构造代码
}

CMineView::~CMineView()
{
}

BOOL CMineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMineView 绘制

void CMineView::OnDraw(CDC* pDC)
{
	CMineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//pDoc->DrawTimer(pDC);

	CRect clientRect;
	GetClientRect(&clientRect);

	CDC memoryDC;
	memoryDC.CreateCompatibleDC(pDC);

	CBitmap memoryBitmap;
	memoryBitmap.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());

	memoryDC.SelectObject(memoryBitmap);

	this->DrawStatusArea(&memoryDC, pDoc);
	this->DrawMineArea(&memoryDC, pDoc);
	
	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &memoryDC, 0, 0, SRCCOPY);

	CRect windowRect;
	GetWindowRect(&windowRect);
}

void CMineView::DrawStatusArea(CDC* memoryDC, CMineDoc* pDoc)
{
	CDC tempDC;
	tempDC.CreateCompatibleDC(memoryDC);

	CBitmap numbersBitmap;
	numbersBitmap.LoadBitmapW(IDB_NUMBERS);
	tempDC.SelectObject(numbersBitmap);

	memoryDC->StretchBlt(0, 0, 16, 23, &tempDC, 0, pDoc->m_index * 23, 16, 23, SRCCOPY);

	CBitmap facesBitmap;
	facesBitmap.LoadBitmapW(IDB_FACES);
	tempDC.SelectObject(facesBitmap);

	memoryDC->StretchBlt(20, 0, 24, 24, &tempDC, 0, 0, 24, 24, SRCCOPY);
}

void CMineView::DrawMineArea(CDC* memoryDC, CMineDoc* pDoc)
{
	CDC tempDC;
	tempDC.CreateCompatibleDC(memoryDC);
	CBitmap mineBitmap;
	mineBitmap.LoadBitmapW(IDB_MINES);
	tempDC.SelectObject(mineBitmap);

	for (UINT y = 0; y < pDoc->m_numY; y++)
	{
		for (UINT x = 0; x < pDoc->m_numX; x++)
		{
			memoryDC->StretchBlt(x * 16, 100 + y * 16, 16, 16, &tempDC, 0, 0, 16, 16, SRCCOPY);
		}
	}
}



// CMineView 诊断

#ifdef _DEBUG
void CMineView::AssertValid() const
{
	CView::AssertValid();
}

void CMineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMineDoc* CMineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMineDoc)));
	return (CMineDoc*)m_pDocument;
}
#endif //_DEBUG


// CMineView 消息处理程序
void CMineView::OnTimer(UINT nIDEvent)
{
	if (1 == nIDEvent)
	{
		CMineDoc* pDoc = GetDocument();
		pDoc->m_index++;
		if (pDoc->m_index >= pDoc->m_max)
		{
			pDoc->m_index = 2;
		}

		InvalidateRect(NULL);
	}
}

void CMineView::OnLButtonUp(UINT nFlags, CPoint point)
{
	SetTimer(1, 1000, NULL);
}

void CMineView::OnRButtonUp(UINT nFlags, CPoint point)
{
	KillTimer(1);
}

BOOL CMineView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}