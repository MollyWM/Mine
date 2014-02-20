
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
        ON_COMMAND(ID_FILE_NEW, OnStart)
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
        {
            return;
        }

        // TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
        //pDoc->DrawTimer(pDC);

        if (!pDoc->m_setWindowSize)
        {
            this->SetWindowSize();
        }

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
    }

    void CMineView::DrawStatusArea(CDC* memoryDC, CMineDoc* pDoc)
    {
        CDC tempDC;
        tempDC.CreateCompatibleDC(memoryDC);

        CBitmap numbersBitmap;
        numbersBitmap.LoadBitmapW(IDB_NUMBERS);
        tempDC.SelectObject(numbersBitmap);

        UINT num = pDoc->m_ticks;

        if (pDoc->m_ticks > pDoc->m_maxTicks)
        {
            num = pDoc->m_maxTicks; 
        }

        UINT num1 = num / 100;
        UINT num2 = num / 10 - num1 * 10;
        UINT num3 = num - num1 * 100 - num2 * 10;

        memoryDC->StretchBlt(0, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
        memoryDC->StretchBlt(16, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        memoryDC->StretchBlt(32, 0, 16, 23, &tempDC, 0, (11 - num3) * 23, 16, 23, SRCCOPY);

        if (pDoc->m_leftMines >= 0)
        {
            num1 = pDoc->m_leftMines / 10;
            num2 = pDoc->m_leftMines - num1 * 10;

            memoryDC->StretchBlt(76, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(92, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        }
        else
        {
            INT absLeftMines = abs((int)pDoc->m_leftMines);
            num1 = absLeftMines / 10;
            num2 = absLeftMines - num1 * 10;

            memoryDC->StretchBlt(76, 0, 16, 23, &tempDC, 0, 0, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(92, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(108, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        }

        CBitmap facesBitmap;
        facesBitmap.LoadBitmapW(IDB_FACES);
        tempDC.SelectObject(facesBitmap);

        memoryDC->StretchBlt(50, 0, 24, 24, &tempDC, 0, 24 * (pDoc->m_start ? 0 : 4), 24, 24, SRCCOPY);
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
                MyMine mine = pDoc->m_mines[x][y];
                memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_width * mine.m_status, 16, 16, SRCCOPY);
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
        CMineDoc* pDoc = this->GetDocument();

        if (!pDoc->m_start)
        {
            return;
        }

        if (pDoc->m_timerId == nIDEvent)
        {
            CMineDoc* pDoc = GetDocument();
            pDoc->m_ticks++;

            InvalidateRect(NULL);
        }
    }

    void CMineView::OnLButtonUp(UINT nFlags, CPoint point)
    {
        this->SetWindowSize();

        CMineDoc* pDoc = this->GetDocument();
        CRect faceRect(50, 0, 74, 24);
        CRect mineAreaRect(0, 30, pDoc->m_numX * 16, pDoc->m_numY * 16 + 30);

        if (faceRect.PtInRect(point) && !pDoc->m_start)
        {
            pDoc->Start(this);
            InvalidateRect(NULL);
            return;
        }

        if (mineAreaRect.PtInRect(point))
        {
            if (!pDoc->m_start)
            {
                return;
            }

            UINT x = point.x / 16;
            UINT y = (point.y - 30) / 16;

            MyMine* mine = &(pDoc->m_mines[x][y]);

            if (mine->m_status != MineStatus::Flag)
            {
                if (mine->m_isMine)
                {
                    mine->m_status = MineStatus::Bomb;
                    pDoc->Failed();
                    MessageBox(L"�����");
                }
                else
                {
                    pDoc->ExpandMines(x, y);
                }
            }
        }

        InvalidateRect(NULL);

        pDoc->CheckWin();
    }

    void CMineView::OnRButtonUp(UINT nFlags, CPoint point)
    {
        CMineDoc* pDoc = this->GetDocument();

        if (!pDoc->m_start)
        {
            return;
        }

        CRect mineAreaRect(0, 30, pDoc->m_numX * 16, pDoc->m_numY * 16 + 30);

        if (mineAreaRect.PtInRect(point))
        {
            UINT x = point.x / 16;
            UINT y = (point.y - 30) / 16;

            MyMine* mine = &(pDoc->m_mines[x][y]);

            if (mine->m_status == MineStatus::Normal)
            {
                pDoc->m_leftMines--;
            }
            else if (mine->m_status == MineStatus::Flag)
            {
                pDoc->m_leftMines++;
            }

            mine->ChangeStatus();
        }

        InvalidateRect(NULL);
    }

    BOOL CMineView::OnEraseBkgnd(CDC* pDC)
    {
        return TRUE;
    }

    void CMineView::SetWindowSize()
    {
        CMineDoc* pDoc = this->GetDocument();
        CWnd* pFrame = this->GetTopLevelFrame();

        CRect clientRect;
        this->GetClientRect(&clientRect);

        CRect windowRect;
        pFrame->GetWindowRect(&windowRect);

        ClientToScreen(&clientRect);

        UINT border = (clientRect.left - windowRect.left);
        UINT width = border * 2 + pDoc->m_mines[0][0].m_width * pDoc->m_numX;
        UINT height = clientRect.top - windowRect.top + pDoc->m_mines[0][0].m_height * pDoc->m_numY + 40;

        this->GetTopLevelFrame()->SetWindowPos(&wndTopMost, windowRect.left, windowRect.top, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
    }

    void CMineView::OnStart()
    {
        GetDocument()->Start(this);
        InvalidateRect(NULL);
    }