
#pragma once

#include "ViewTree.h"
struct STR_ViewNode
{	
	HTREEITEM m_hItem;
	CString m_nodePath;
};
class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// 构造
public:
	CFileView();
	CViewTree* GetViewTree();
	CString GetFilePath(HTREEITEM hItem,CString filePath);
	void AdjustLayout();
	void OnChangeVisualStyle();
	void GetFileNode(HTREEITEM hItem, CString  nodePath,_In_ int nDeep=0 );
	HTREEITEM InsertItemFunc(_In_z_ LPCTSTR lpszItem, _In_ int nImage, _In_ int nSelectedImage,
		_In_ HTREEITEM hParent = TVI_ROOT, _In_ HTREEITEM hInsertAfter = TVI_LAST);
	int getType() { return FILE_VIEW; };
// 特性
protected:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

protected:
	void FillFileView();
	void GetFileSystem();
	//void GetFileNode(HTREEITEM hItem, CString  nodePath);
	BOOL AddDrives(CString strDrive, HTREEITEM hParent);

// 实现
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

