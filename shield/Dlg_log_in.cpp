// Dlg_log_in.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "shield.h"
#include "Dlg_log_in.h"
#include "afxdialogex.h"


// Dlg_log_in �Ի���

IMPLEMENT_DYNAMIC(Dlg_log_in, CDialogEx)

Dlg_log_in::Dlg_log_in(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strUserName(_T(""))
	, m_strPswd(_T(""))
{

}

Dlg_log_in::~Dlg_log_in()
{
}

void Dlg_log_in::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PSWD, m_strPswd);
}


BEGIN_MESSAGE_MAP(Dlg_log_in, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &Dlg_log_in::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// Dlg_log_in ��Ϣ�������

void Dlg_log_in::OnClose()
{
	CDialogEx::OnClose();
}
void Dlg_log_in::OnOK()
{
	CDialogEx::OnOK();
}
void Dlg_log_in::OnBnClickedBtnLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if (m_strUserName.Compare(_T("develop"))!=0)
	{
		AfxMessageBox(_T("�û������ԣ�"));
		return;
	}
	if (m_strPswd.Compare(_T("kxf3099"))!= 0)
	{
		AfxMessageBox(_T("���벻�ԣ�"));
		return;
	}
	OnOK();
}



