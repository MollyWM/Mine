
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
	UINT m_increase;
	UINT m_max;
	UINT m_index;

	UINT m_numX;
	UINT m_numY;
// ����
public:
   void DrawTimer(CDC* pDC);
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
