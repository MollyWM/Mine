
// MineView.cpp : CMineView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CMineView ����/����

CMineView::CMineView()
{
	// TODO: �ڴ˴���ӹ������
}

CMineView::~CMineView()
{
}

BOOL CMineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMineView ����

void CMineView::OnDraw(CDC* pDC)
{
	CMineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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



// CMineView ���

#ifdef _DEBUG
void CMineView::AssertValid() const
{
	CView::AssertValid();
}

void CMineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMineDoc* CMineView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMineDoc)));
	return (CMineDoc*)m_pDocument;
}
#endif //_DEBUG


// CMineView ��Ϣ�������
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