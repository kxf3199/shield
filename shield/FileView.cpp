
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "shield.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define ILI_CDDRV 5
//#define ILI_CLSDFLD 1
#define ILI_FOLDER 1
#define ILI_DRIVE 3
#define ILI_FLOPPYDRV 6
#define ILI_MYCOMP 0
#define ILI_OPENFLD 2
#define ILI_FILE 4
/////////////////////////////////////////////////////////////////////////////
// CFileView

unsigned int job(WPARAM wParam, LPARAM lParam)
{
	int nCount = 0;
	STR_ViewNode* stu_viewNode = (STR_ViewNode*)wParam;
	HTREEITEM hItem = stu_viewNode->m_hItem;
	CString nodePath = stu_viewNode->m_nodePath;	
	CFileView* tmp_fileView = (CFileView*)lParam;
	if (nodePath.Right(1).Compare(_T("\\")) == 0)
		nodePath = nodePath.Left(nodePath.GetLength() - 1);
	nodePath += "\\*.*";
	CString  fdPath, fdName;
	CFileFind find;
	BOOL bf = find.FindFile(nodePath);
	while (bf)
	{
		bf = find.FindNextFile();
		if (!find.IsDots())
		{
			fdPath = find.GetFilePath();
			fdName = find.GetFileName();
			if (find.IsDirectory())
			{
				HTREEITEM hSonItem = tmp_fileView->InsertItemFunc(fdName, ILI_FOLDER, ILI_FOLDER, hItem);				
				WPARAM wParanTmp;
				STR_ViewNode stu_tmp;				
				stu_tmp.m_hItem = hSonItem;
				stu_tmp.m_nodePath = fdPath;
				wParanTmp =(WPARAM)&stu_tmp;
				wParam=job(wParam, (LPARAM)tmp_fileView);
			}
			else
			{
				//������ļ�				
				tmp_fileView->InsertItemFunc(fdName, ILI_FOLDER, ILI_FOLDER, hItem);
				nCount++;
			}
		}
	}
	find.Close();
	return nCount;
}
void cb(unsigned int pResult)
{
	CString msg;
	msg.Format(_T("�������,�ܹ��ļ�����Ϊ%d"), pResult);
	AfxMessageBox(msg);
}
CFileView::CFileView()
{
	
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ������ͼ: 
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("δ�ܴ����ļ���ͼ\n");
		return -1;      // δ�ܴ���
	}

	// ������ͼͼ��: 
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
	//FillFileView();
	GetFileSystem();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}
CViewTree* CFileView::GetViewTree()
{
	return &m_wndFileView;
}
HTREEITEM CFileView::InsertItemFunc(_In_z_ LPCTSTR lpszItem, _In_ int nImage, _In_ int nSelectedImage,
	_In_ HTREEITEM hParent , _In_ HTREEITEM hInsertAfter )
{
	EnterCriticalSection(&theApp.g_cs);
	HTREEITEM hItem=m_wndFileView.InsertItem(lpszItem,nImage,nSelectedImage,hParent);
	LeaveCriticalSection(&theApp.g_cs);
	if (!hItem)	
		return NULL;	
	return hItem;
}
CString CFileView::GetFilePath(HTREEITEM hItem, CString filePath)
{
	HTREEITEM hParent = m_wndFileView.GetParentItem(hItem);
	if (hParent&&hParent!=m_wndFileView.GetRootItem())
	{
		CString parentPath = m_wndFileView.GetItemText(hParent);
		if (parentPath.Right(1).Compare(_T("\\")) == 0)
			parentPath = parentPath.Left(parentPath.GetLength() - 1);

		filePath = parentPath+"\\"+filePath;
		filePath=GetFilePath(hParent, filePath);
	}
	return filePath;
}
void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("FakeApp �ļ�"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("FakeApp Դ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	m_wndFileView.InsertItem(_T("StdAfx.cpp"), 1, 1, hSrc);

	HTREEITEM hInc = m_wndFileView.InsertItem(_T("FakeApp ͷ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("Resource.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	m_wndFileView.InsertItem(_T("StdAfx.h"), 2, 2, hInc);

	HTREEITEM hRes = m_wndFileView.InsertItem(_T("FakeApp ��Դ�ļ�"), 0, 0, hRoot);

	m_wndFileView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	m_wndFileView.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
	m_wndFileView.Expand(hInc, TVE_EXPAND);
}
void CFileView::GetFileSystem()
{
		this;
		m_wndFileView.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
		int nPos = 0;
		UINT nCount = 0;
		CString strDrive = _T("?:\\");
		DWORD dwDriveList = ::GetLogicalDrives();
		CString cTmp;
		HTREEITEM hParent = m_wndFileView.InsertItem(_T("MyComputer"), 0, 0);
		m_wndFileView.SetItemState(hParent, TVIS_BOLD, TVIS_BOLD);
		while (dwDriveList) {
			if (dwDriveList & 1) {
				cTmp = strDrive;
				strDrive.SetAt(0, 0x41 + nPos);
				if (AddDrives(strDrive, hParent))
					nCount++;
			}
			dwDriveList >>= 1;
			nPos++;
		}
		return;
}
	
BOOL CFileView::AddDrives(CString strDrive, HTREEITEM hParent)
{
	HTREEITEM hItem;
	UINT nType = ::GetDriveType((LPCTSTR)strDrive);
	UINT nDrive = (UINT)strDrive[0] - 0x41;
	switch (nType) {
	case DRIVE_REMOVABLE:
		//hItem = InsertItemFunc(strDrive, ILI_FLOPPYDRV, ILI_FLOPPYDRV, hParent);
		hItem = m_wndFileView.InsertItem(strDrive, ILI_FLOPPYDRV, ILI_FLOPPYDRV, hParent);
		break;
	case DRIVE_FIXED:
		//hItem = InsertItemFunc(strDrive, ILI_DRIVE, ILI_DRIVE, hParent);
		hItem = m_wndFileView.InsertItem(strDrive, ILI_DRIVE, ILI_DRIVE, hParent);
		break;
	case DRIVE_REMOTE:
		//hItem = InsertItemFunc(strDrive, ILI_DRIVE, ILI_DRIVE, hParent);
		hItem = m_wndFileView.InsertItem(strDrive, ILI_DRIVE, ILI_DRIVE, hParent);
		break;
	case DRIVE_CDROM:
		//hItem = InsertItemFunc(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		hItem = m_wndFileView.InsertItem(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		break;
	case DRIVE_RAMDISK:
		//hItem = InsertItemFunc(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		hItem = m_wndFileView.InsertItem(strDrive, ILI_CDDRV, ILI_CDDRV, hParent);
		break;
	default:
		return FALSE;
	}
	bool bHasChild = true;
	m_wndFileView.SetItemData(hParent, bHasChild);
// 	STR_ViewNode stu_viewNode;	
// 	stu_viewNode.m_hItem = hItem;
// 	stu_viewNode.m_nodePath = strDrive;
// 	WPARAM wParam = (WPARAM)&stu_viewNode;	
// 	theApp.m_pThreadManagerInterface->pushJob(job, wParam, (LPARAM)this, cb);
	GetFileNode(hItem, strDrive);
	return true;
}
void CFileView::GetFileNode(HTREEITEM hItem, CString  nodePath,_In_ int nDeep)
{	
	if (nodePath.Right(1).Compare(_T("\\")) == 0)
		nodePath = nodePath.Left(nodePath.GetLength() - 1);
	nodePath += "\\*.*";
	CString  fdPath, fdName;
	CFileFind find;
	BOOL bf = find.FindFile(nodePath);
	while (bf)
	{
		bf = find.FindNextFile();
		if (!find.IsDots())
		{
			fdPath = find.GetFilePath();
			fdName = find.GetFileName();
			if (find.IsDirectory())
			{
				HTREEITEM hSonItem = m_wndFileView.InsertItem(fdName, ILI_FOLDER, ILI_FOLDER, hItem);
				if (hSonItem)
				{
					bool bHasChild = true;
					m_wndFileView.SetItemData(hItem, bHasChild);
// 					nDeep++;
// 					if (nDeep < 2)
// 					{
// 						GetFileNode(hSonItem, fdPath, nDeep);
// 						nDeep = 0;
// 					}
					m_wndFileView.InsertItem(_T("tmp"), 1, 1, hSonItem);
				}
				
				//
			}
			else
			{
				//������ļ�,
				HTREEITEM hSonItem=m_wndFileView.InsertItem(fdName, ILI_FILE, ILI_FILE, hItem);
				bool bHasChild =true;
				m_wndFileView.SetItemData(hItem, bHasChild);
			}
		}
		
	}
	find.Close();
	
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// ѡ���ѵ�������: 
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("����...."));

}

void CFileView::OnFileOpen()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnFileOpenWith()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnDummyCompile()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCut()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditCopy()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnEditClear()
{
	// TODO: �ڴ˴���������������
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_NEW : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("�޷�����λͼ: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


