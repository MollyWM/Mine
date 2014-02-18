
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
	UINT m_increase;
	UINT m_max;
	UINT m_index;

	UINT m_numX;
	UINT m_numY;
// 操作
public:
   void DrawTimer(CDC* pDC);
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
