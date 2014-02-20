
// MineDoc.cpp : CMineDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Mine.h"
#endif

#include "MineDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMineDoc

IMPLEMENT_DYNCREATE(CMineDoc, CDocument)

    BEGIN_MESSAGE_MAP(CMineDoc, CDocument)
    END_MESSAGE_MAP()


    // CMineDoc ����/����

    CMineDoc::CMineDoc()
    {
        // TODO: �ڴ����һ���Թ������
        this->m_increase = 23;
        this->m_max = 12;
        this->m_index = 2;

        this->m_start = FALSE;
        this->m_setWindowSize = FALSE;

        this->InitMines();
    }

    CMineDoc::~CMineDoc()
    {
    }

    BOOL CMineDoc::OnNewDocument()
    {
        if (!CDocument::OnNewDocument())
            return FALSE;

        // TODO: �ڴ�������³�ʼ������
        // (SDI �ĵ������ø��ĵ�)

        return TRUE;
    }


    // CMineDoc ���л�

    void CMineDoc::Serialize(CArchive& ar)
    {
        if (ar.IsStoring())
        {
            // TODO: �ڴ���Ӵ洢����
        }
        else
        {
            // TODO: �ڴ���Ӽ��ش���
        }
    }

#ifdef SHARED_HANDLERS

    // ����ͼ��֧��
    void CMineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
    {
        // �޸Ĵ˴����Ի����ĵ�����
        dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

        CString strText = _T("TODO: implement thumbnail drawing here");
        LOGFONT lf;

        CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
        pDefaultGUIFont->GetLogFont(&lf);
        lf.lfHeight = 36;

        CFont fontDraw;
        fontDraw.CreateFontIndirect(&lf);

        CFont* pOldFont = dc.SelectObject(&fontDraw);
        dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
        dc.SelectObject(pOldFont);
    }

    // ������������֧��
    void CMineDoc::InitializeSearchContent()
    {
        CString strSearchContent;
        // ���ĵ����������������ݡ�
        // ���ݲ���Ӧ�ɡ�;���ָ�

        // ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
        SetSearchContent(strSearchContent);
    }

    void CMineDoc::SetSearchContent(const CString& value)
    {
        if (value.IsEmpty())
        {
            RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
        }
        else
        {
            CMFCFilterChunkValueImpl *pChunk = NULL;
            ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
            if (pChunk != NULL)
            {
                pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
                SetChunkValue(pChunk);
            }
        }
    }

#endif // SHARED_HANDLERS

    // CMineDoc ���

#ifdef _DEBUG
    void CMineDoc::AssertValid() const
    {
        CDocument::AssertValid();
    }

    void CMineDoc::Dump(CDumpContext& dc) const
    {
        CDocument::Dump(dc);
    }
#endif //_DEBUG


    // CMineDoc ����
    void CMineDoc::DrawTimer(CDC* pDC)
    {
        CBitmap bm;
        bm.LoadBitmapW(IDB_TIMER);

        BITMAP b;
        bm.GetBitmap(&b);

        CDC memDC;
        memDC.CreateCompatibleDC(NULL);
        memDC.SelectObject(bm);
        pDC->BitBlt(0, 0, b.bmWidth, this->m_increase, &memDC, 0, this->m_increase * this->m_index, SRCCOPY);
    }

    void CMineDoc::InitMines()
    {
        this->m_numX = this->m_numY = 9;

        this->m_ticks = 0;
        this->m_maxTicks = 999;
        this->m_timerId = 9999;

        this->m_leftMines = 10;

        for (UINT y = 0; y < this->m_numY; y++)
        {
            for (UINT x = 0; x < this->m_numX; x++)
            {
                MyMine* mine = (&this->m_mines[x][y]);

                mine->m_width = 16;
                mine->m_height = 16;
                mine->m_status = MineStatus::Normal;
                mine->m_isMine = FALSE;
                mine->m_aroundMines = 0;
                mine->m_x = x;
                mine->m_y = y;
            }
        }
    }

    void CMineDoc::LayMines(UINT row, UINT col)
    {
        //�����������
        srand( (unsigned)time( NULL ) );
        UINT i, j;

        for(UINT index = 0; index < this->m_leftMines;) 
        {	
            //ȡ�漴��
            i = rand() % this->m_numY;
            j = rand() % this->m_numX;

            if (i == row && j == col) continue;

            MyMine* mine = &((this->m_mines)[i][j]);

            if(!mine->m_isMine) 
            {
                mine->m_isMine = TRUE;//�޸�����Ϊ��
                index++;
            }
        }	
    }

    //
    //�׷�����չ(������Χ���׵Ŀհ�����)
    //
    void CMineDoc::ExpandMines(UINT row, UINT col)
    {
        UINT i, j;
        UINT minRow = (row == 0) ? 0 : row - 1;
        UINT maxRow = row + 2;
        UINT minCol = (col == 0) ? 0 : col - 1;
        UINT maxCol = col + 2;
        UINT around = GetAroundNum(row, col);


        this->m_mines[row][col].m_status = (MineStatus)(15 - around);

        // ���򿪡��������ػ�
        //DrawSpecialMine(row, col);

        //����Χһ���׶�û�еĿհ�����
        if (around == 0)
        {
            for (i = minRow; i < maxRow; i++) 
            {
                for (j = minCol; j < maxCol; j++) 
                {//������Χ������չ��������еĹ���չ		
                    MyMine* mine = &(this->m_mines[i][j]);
                    if (!(i == row && j == col) && mine->m_status == MineStatus::Normal && !mine->m_isMine)
                    {
                        if (!IsInMineArea(i, j)) continue;
                        ExpandMines(i, j);
                    }
                }
            }
        }
    }

    UINT CMineDoc::GetAroundNum(UINT row, UINT col)
    {
        UINT i, j;
        UINT around = 0;
        UINT minRow = (row == 0) ? 0 : row - 1;
        UINT maxRow = row + 2;
        UINT minCol = (col == 0) ? 0 : col - 1;
        UINT maxCol = col + 2;

        for (i = minRow; i < maxRow; i++) 
        {
            for (j = minCol; j < maxCol; j++)
            {
                if (!IsInMineArea(i, j)) continue;
                if (this->m_mines[i][j].m_isMine) around++;
            }
        }
        return around;
    }

    UINT CMineDoc::GetAroundFlags(UINT row, UINT col)
    {
        UINT i, j;
        UINT flags = 0;
        UINT minRow = (row == 0) ? 0 : row - 1;
        UINT maxRow = row + 2;
        UINT minCol = (col == 0) ? 0 : col - 1;
        UINT maxCol = col + 2;

        for (i = minRow; i < maxRow; i++) 
        {
            for (j = minCol; j < maxCol; j++)
            {
                if (!IsInMineArea(i, j)) continue;
                if (this->m_mines[i][j].m_status == MineStatus::Flag) flags++;
            }
        }
        return flags;
    }

    //�ж��Ƿ���������
    BOOL CMineDoc::IsInMineArea(UINT row, UINT col)
    {
        return (row >= 0 && row < this->m_numX && col >= 0 && col < this->m_numY);
    }

    BOOL CMineDoc::CheckWin()
    {
        for (UINT i = 0; i < this->m_numY; i++)
        {
            for (UINT j = 0; j < this->m_numX; j++) 
            {
                MyMine* mine = &(this->m_mines[i][j]);
                if (mine->m_status == MineStatus::Normal || mine->m_status == MineStatus::Unknown)
                {
                    return FALSE;
                }
            }
        }

        m_start = FALSE;

        return TRUE;
    }

    void CMineDoc::Failed()
    {
        m_start = FALSE;
    }

    void CMineDoc::Start(CWnd* wnd)
    {
        KillTimer(NULL, this->m_timerId);

        this->InitMines();
        this->LayMines(this->m_numX, this->m_numY);
        this->m_start = TRUE;
        
        SetTimer(*wnd, this->m_timerId, 1000, NULL);
    }