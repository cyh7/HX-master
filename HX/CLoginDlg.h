#pragma once
#include "CMyButton.h"
extern bool LoginFlag;
// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoginDlg();

	

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_user;
	CString m_pwd;
	afx_msg void OnBnClickedButtonLogin();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSetpwd();
	BOOL PreTranslateMessage(MSG* pMsg);
	CMyButton m_log_btn_login;
	CMyButton m_log_btn_cancel;
	CMyButton m_log_btn_setpwd;
	CBrush m_log_Brush;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
