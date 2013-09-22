
// NoteBookView.h : CNoteBookView ��Ľӿ�
//

#pragma once
#include "atltypes.h"
#include "afxcoll.h"


class CNoteBookView : public CView
{
protected: // �������л�����
	CNoteBookView();
	DECLARE_DYNCREATE(CNoteBookView)

// ����
public:
	CNoteBookDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CNoteBookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	// �������Ƿ����ѡ��
	int m_iMouseSelected;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// ������λ����Ϣ
	CPoint m_ptCaret;
	// �����ַ���
	CString m_strLine;
	// д�����ֵ����λ��
	CPoint m_ptWriteCaret;
	// ��һ��Ϊ��λԪ�ر����ַ���
	CStringArray m_strAarray;
	// �ϴ�����Ĺ��λ��
	CPoint m_ptOrgCaret;
	// ԭ����ߵ����
	CString m_strLeft;
	// ԭ���ұߵ����
	CString m_strRight;
	// �жϲ���ģʽ�еĻس�
	int m_IsCaretEnter;
};

#ifndef _DEBUG  // NoteBookView.cpp �еĵ��԰汾
inline CNoteBookDoc* CNoteBookView::GetDocument() const
   { return reinterpret_cast<CNoteBookDoc*>(m_pDocument); }
#endif

