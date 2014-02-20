
// MineDoc.h : CMineDoc 类的接口
//


#pragma once

class CMineDoc : public CDocument
{
protected: // 仅从序列化创建
    CMineDoc();
    DECLARE_DYNCREATE(CMineDoc)

    // 特性
public:
    MyMine m_mines[100][100];

	Game _game;
    // 操作
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
    
    // 重写
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // 实现
public:
    virtual ~CMineDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // 生成的消息映射函数
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // 用于为搜索处理程序设置搜索内容的 Helper 函数
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
