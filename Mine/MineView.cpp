
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
        ON_COMMAND(ID_GAME_EASY, OnEasy)
        ON_COMMAND(ID_GAME_MEDIUM, OnMedium)
        ON_COMMAND(ID_GAME_HARD, OnHard)
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
        {
            return;
        }

        // TODO: 在此处为本机数据添加绘制代码
        //pDoc->DrawTimer(pDC);

        this->SetWindowSize();

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

        Game* pGame = &(pDoc->_game);

        UINT num = pGame->GetElapsedTime();

        UINT num1 = num / 100;
        UINT num2 = num / 10 - num1 * 10;
        UINT num3 = num - num1 * 100 - num2 * 10;

        CRect clientRect;
        GetClientRect(&clientRect);

        memoryDC->StretchBlt(0, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
        memoryDC->StretchBlt(16, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        memoryDC->StretchBlt(32, 0, 16, 23, &tempDC, 0, (11 - num3) * 23, 16, 23, SRCCOPY);

        if (pGame->GetNumberOfMines() >= 0)
        {
            num1 = pGame->GetNumberOfMines() / 10;
            num2 = pGame->GetNumberOfMines() - num1 * 10;

            memoryDC->StretchBlt(76, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(92, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        }
        else
        {
            INT absLeftMines = abs((int)pGame->GetNumberOfMines());
            num1 = absLeftMines / 10;
            num2 = absLeftMines - num1 * 10;

            memoryDC->StretchBlt(76, 0, 16, 23, &tempDC, 0, 0, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(92, 0, 16, 23, &tempDC, 0, (11 - num1) * 23, 16, 23, SRCCOPY);
            memoryDC->StretchBlt(108, 0, 16, 23, &tempDC, 0, (11 - num2) * 23, 16, 23, SRCCOPY);
        }

        CBitmap facesBitmap;
        facesBitmap.LoadBitmapW(IDB_FACES);
        tempDC.SelectObject(facesBitmap);

        memoryDC->StretchBlt(50, 0, 24, 24, &tempDC, 0, 24 * (pGame->GetStarted() ? 0 : 4), 24, 24, SRCCOPY);
    }

    void CMineView::DrawMineArea(CDC* memoryDC, CMineDoc* pDoc)
    {
        CDC tempDC;
        tempDC.CreateCompatibleDC(memoryDC);
        CBitmap mineBitmap;
        mineBitmap.LoadBitmapW(IDB_MINES);
        tempDC.SelectObject(mineBitmap);

        Game* pGame = &(this->GetDocument()->_game);

        for (UINT y = 0; y < pGame->GetNumberOfBlockInY(); y++)
        {
            for (UINT x = 0; x < pGame->GetNumberOfBlockInX(); x++)
            {
                MyMine mine = pGame->GetMine(x, y);

                if (pGame->GetFailed())
                {
                    if (mine.m_status == MineStatus::Normal && mine.m_isMine)
                    {
                        memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * (UINT)MineStatus::Bomb, 16, 16, SRCCOPY);
                    }
                    else if (mine.m_status == MineStatus::Unknown && mine.m_isMine)
                    {
                        memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * (UINT)MineStatus::Unknown, 16, 16, SRCCOPY);
                    }
                    else if (mine.m_status == MineStatus::Flag && mine.m_isMine)
                    {
                        memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * (UINT)MineStatus::Mine, 16, 16, SRCCOPY);
                    }
                    else if (mine.m_status == MineStatus::Flag && !mine.m_isMine)
                    {
                        memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * (UINT)MineStatus::Error, 16, 16, SRCCOPY);
                    }
                    else
                    {
                        memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * mine.m_status, 16, 16, SRCCOPY);
                    }
                }
                else
                {
                    memoryDC->StretchBlt(x * 16, 30 + y * 16, 16, 16, &tempDC, 0, mine.m_height * mine.m_status, 16, 16, SRCCOPY);
                }
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
        CMineDoc* pDoc = this->GetDocument();
        pDoc->_game.Elapse();
        InvalidateRect(NULL);
    }

    void CMineView::OnLButtonUp(UINT nFlags, CPoint point)
    {
        this->SetWindowSize();

        CMineDoc* pDoc = this->GetDocument();
        Game* pGame = &(pDoc->_game);

        CRect faceRect(50, 0, 74, 24);
        CRect mineAreaRect(0, 30, pGame->GetNumberOfBlockInX() * 16, pGame->GetNumberOfBlockInY() * 16 + 30);

        if (faceRect.PtInRect(point) && !pGame->GetStarted())
        {
            this->Start();;
            InvalidateRect(NULL);
            return;
        }

        if (mineAreaRect.PtInRect(point))
        {	 
            if (!pGame->GetStarted())
            {
                return;
            }

            UINT x = point.x / 16;
            UINT y = (point.y - 30) / 16;

            pGame->LeftClickInMineArea(x, y);

            if (pGame->Fail(x, y))
            {
                KillTimer(1);
                InvalidateRect(NULL);
                MessageBox(L"你挂了", L"游戏结束", MB_ICONSTOP);
                return;
            }

            if (pGame->Success())
            {
                KillTimer(1);
                InvalidateRect(NULL);
                MessageBox(L"你赢了", L"游戏结束", MB_OK);
                return;
            }
        }

        InvalidateRect(NULL);
    }

    void CMineView::OnRButtonUp(UINT nFlags, CPoint point)
    {
        CMineDoc* pDoc = this->GetDocument();

        Game* pGame = &(pDoc->_game);

        if (!pGame->GetStarted())
        {
            return;
        }

        CRect mineAreaRect(0, 30, pGame->GetNumberOfBlockInX() * 16, pGame->GetNumberOfBlockInY() * 16 + 30);

        if (mineAreaRect.PtInRect(point))
        {
            UINT x = point.x / 16;
            UINT y = (point.y - 30) / 16;

            pGame->RightClickInMineArea(x, y);

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
        Game* pGame = &(pDoc->_game);
        CWnd* pFrame = this->GetTopLevelFrame();

        CRect clientRect;
        this->GetClientRect(&clientRect);

        CRect windowRect;
        pFrame->GetWindowRect(&windowRect);

        ClientToScreen(&clientRect);

        UINT border = (clientRect.left - windowRect.left);
        UINT width = border * 2 + pGame->GetMine(0, 0).m_width * pGame->GetNumberOfBlockInX();
        UINT height = clientRect.top - windowRect.top + pGame->GetMine(0, 0).m_height * pGame->GetNumberOfBlockInY() + 40;

        this->GetTopLevelFrame()->SetWindowPos(&wndTopMost, windowRect.left, windowRect.top, width, height, SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);
    }

    void CMineView::Start()
    {
        Game* pGame = &(this->GetDocument()->_game);
        pGame->Init();
        pGame->Start();
        this->SetTimer(1, 1000, NULL);
        InvalidateRect(NULL);
    }

    void CMineView::OnEasy()
    {
        Game* pGame = &(this->GetDocument()->_game);
        pGame->SetEasyLevel();
        this->Start();
    }

    void CMineView::OnMedium()
    {
        Game* pGame = &(this->GetDocument()->_game);
        pGame->SetMediumLevel();
        this->Start();
    }

    void CMineView::OnHard()
    {
        Game* pGame = &(this->GetDocument()->_game);
        pGame->SetHardLevel();
        this->Start();
    }

    void GetStatusLeftSpan()
    {

    }