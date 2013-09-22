
// NoteBookView.h : CNoteBookView 类的接口
//

#pragma once
#include "atltypes.h"
#include "afxcoll.h"


class CNoteBookView : public CView
{
protected: // 仅从序列化创建
	CNoteBookView();
	DECLARE_DYNCREATE(CNoteBookView)

// 特性
public:
	CNoteBookDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CNoteBookView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	// 标记鼠标是否被左键选中
	int m_iMouseSelected;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 保存光标位置信息
	CPoint m_ptCaret;
	// 保存字符串
	CString m_strLine;
	// 写入文字的最后位置
	CPoint m_ptWriteCaret;
	// 以一行为单位元素保存字符串
	CStringArray m_strAarray;
	// 上次输入的光标位置
	CPoint m_ptOrgCaret;
	// 原有左边的输出
	CString m_strLeft;
	// 原有右边的输出
	CString m_strRight;
	// 判断插入模式中的回车
	int m_IsCaretEnter;
};

#ifndef _DEBUG  // NoteBookView.cpp 中的调试版本
inline CNoteBookDoc* CNoteBookView::GetDocument() const
   { return reinterpret_cast<CNoteBookDoc*>(m_pDocument); }
#endif

