#pragma once


// Dlg_log_in �Ի���

class Dlg_log_in : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_log_in)

public:
	Dlg_log_in(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Dlg_log_in();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_strUserName;
	CString m_strPswd;
private:
	void OnClose();
	void OnOK();
public:
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnCancel();
};
