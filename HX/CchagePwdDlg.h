#pragma once
#include "CMyButton.h"

// CchagePwdDlg 对话框

class CchagePwdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CchagePwdDlg)

public:
	CchagePwdDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CchagePwdDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGEPWD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_oldPwd;
	CString m_newPwd;
	CString m_surePwd;
	afx_msg void OnBnClickedButton1();
	BOOL PreTranslateMessage(MSG* pMsg);
	CMyButton m_chg_btn_sure;
	CMyButton m_chg_btn_cancel;
	CBrush m_chg_Brush;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
};
