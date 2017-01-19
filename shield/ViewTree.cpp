
#include "stdafx.h"
#include "ViewTree.h"
#include "FileView.h"
#include "ClassView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
	m_tree = this;
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnLButtonDblClk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 消息处理程序

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}
void CViewTree::OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (pNMHDR && pNMHDR->code == NM_DBLCLK)
	{
		Click(pNMHDR, pResult, NM_DBLCLK);
	}
}
void CViewTree::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (pNMHDR && pNMHDR->code == NM_CLICK)
	{
		Click(pNMHDR, pResult,NM_CLICK);
	}
	
}
void CViewTree::Click(NMHDR* pNMHDR, LRESULT* pResult,UINT uFlags)
{
	CPoint pt;
	GetCursorPos(&pt);
	m_tree->ScreenToClient(&pt);
	bool bFileView = true;
	CFileView* fileView = (CFileView*)m_tree->GetParent();
	CClassView* classView = NULL;
	if (fileView->GetViewTree() != m_tree)
	{
		classView = (CClassView*)fileView;
		bFileView = false;
	}
	UINT uFlag = 0;
	HTREEITEM hCurSel = m_tree->GetSelectedItem();
	HTREEITEM hItem = m_tree->HitTest(pt, &uFlag);
	if (hItem)
	{
		if (hCurSel != hItem)
		{
			m_tree->SelectItem(hItem);
		}

		CString strText(_T(""));
		if (TVHT_ONITEMBUTTON & uFlag&&(uFlags==NM_CLICK|| uFlags == NM_DBLCLK))
		{
			//strText.Format(_T("On Button"));
			CString strPath = m_tree->GetItemText(hItem);
			bool bHasChild = m_tree->GetItemData(hItem);
			if (bFileView&&!bHasChild)
			{
				HTREEITEM hChild = m_tree->GetChildItem(hItem);
				if (hChild)
					m_tree->DeleteItem(hChild);
				strPath = fileView->GetFilePath(hItem, strPath);
				fileView->GetFileNode(hItem, strPath);
			}
			Expand(hItem, TVE_TOGGLE);
			*pResult = 1;
			return;
		}
		else if (TVHT_ONITEMLABEL & uFlag&&uFlags==NM_DBLCLK)
		{
			CString strPath = m_tree->GetItemText(hItem);
			bool bHasChild = m_tree->GetItemData(hItem);
			if (bFileView&&!bHasChild)
			{
				HTREEITEM hChild = m_tree->GetChildItem(hItem);
				if (hChild)
					m_tree->DeleteItem(hChild);
				strPath = fileView->GetFilePath(hItem, strPath);
				fileView->GetFileNode(hItem, strPath);
			}
			Expand(hItem, TVE_TOGGLE);
			*pResult = 1;
			return;
		}
		else
		{
			strText.Format(_T("On Others"));
		}
		//AfxMessageBox(strText);
	}

	*pResult = 0;
}