
// MineView.h : CMineView 类的接口
//

#pragma once


class CMineView : public CView
{
protected: // 仅从序列化创建
    CMineView();
    DECLARE_DYNCREATE(CMineView)

    // 特性
public:
    CMineDoc* GetDocument() const;

protected:

    // 操作
public:

    // 重写
public:
    virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void SetWindowSize();

    // 实现
public:
    virtual ~CMineView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    void DrawStatusArea(CDC* memoryDC, CMineDoc* pDoc);
    void DrawMineArea(CDC* memoryDC, CMineDoc* pDoc);
    void Start();
    void Fail();
    void Succeed();

    // 生成的消息映射函数
protected:
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnEasy();
    afx_msg void OnMedium();
    afx_msg void OnHard();

    DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // MineView.cpp 中的调试版本
inline CMineDoc* CMineView::GetDocument() const
{ return reinterpret_cast<CMineDoc*>(m_pDocument); }
#endif

