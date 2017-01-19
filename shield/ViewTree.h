
#pragma once
/////////////////////////////////////////////////////////////////////////////
// CViewTree ����
#define FILE_VIEW 1
#define CLASS_VIEW 2
struct STR_treeNode
{
	bool m_bFolder;
	bool m_bFile;
	bool m_bHasChild;
	STR_treeNode()
	{
		m_bFolder = false;
		m_bHasChild = false;
	}
};
class CViewTree : public CTreeCtrl
{
// ����
public:
	CViewTree();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(NMHDR* pNMHDR, LRESULT* pResult);
// ��д
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CViewTree* m_tree;
	void Click(NMHDR* pNMHDR, LRESULT* pResult,UINT uFlags);
// ʵ��
public:
	virtual ~CViewTree();
protected:
	DECLARE_MESSAGE_MAP()
};
