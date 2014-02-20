
// MineDoc.h : CMineDoc ��Ľӿ�
//


#pragma once

class CMineDoc : public CDocument
{
protected: // �������л�����
    CMineDoc();
    DECLARE_DYNCREATE(CMineDoc)

    // ����
public:
    MyMine m_mines[100][100];

	Game _game;
    // ����
public:
    void DrawTimer(CDC* pDC);
    void ExpandMines(UINT row, UINT col);
    void InitMines();
    void LayMines(UINT row, UINT col);
    BOOL IsInMineArea(UINT row, UINT col);
    BOOL CheckWin();
    void Failed();
    void Start(CWnd* wnd);
private:
    UINT GetAroundNum(UINT row, UINT col);
    UINT GetAroundFlags(UINT row, UINT col);
    
    // ��д
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // ʵ��
public:
    virtual ~CMineDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // ���ɵ���Ϣӳ�亯��
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // ����Ϊ����������������������ݵ� Helper ����
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
