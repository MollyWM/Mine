
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

	this->m_numX = this->m_numY = 9;
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