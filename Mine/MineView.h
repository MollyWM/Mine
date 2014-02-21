
// MineView.h : CMineView ��Ľӿ�
//

#pragma once


class CMineView : public CView
{
protected: // �������л�����
    CMineView();
    DECLARE_DYNCREATE(CMineView)

    // ����
public:
    CMineDoc* GetDocument() const;

protected:

    // ����
public:

    // ��д
public:
    virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
    virtual void SetWindowSize();

    // ʵ��
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

    // ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // MineView.cpp �еĵ��԰汾
inline CMineDoc* CMineView::GetDocument() const
{ return reinterpret_cast<CMineDoc*>(m_pDocument); }
#endif

