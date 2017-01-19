
// shieldView.h : CshieldView 类的接口
//

#pragma once


class CshieldView : public CView
{
protected: // 仅从序列化创建
	CshieldView();
	DECLARE_DYNCREATE(CshieldView)

// 特性
public:
	CshieldDoc* GetDocument() const;

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
	virtual ~CshieldView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // shieldView.cpp 中的调试版本
inline CshieldDoc* CshieldView::GetDocument() const
   { return reinterpret_cast<CshieldDoc*>(m_pDocument); }
#endif

