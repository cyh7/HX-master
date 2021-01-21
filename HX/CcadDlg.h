#pragma once
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"


// CcadDlg 对话框

class CcadDlg : public CDialogEx, public has_slots<>
{
	DECLARE_DYNAMIC(CcadDlg)

public:
	CcadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CcadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CLayout m_layoutCad;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnPaint();
	CListCtrl m_CadGlueList;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	BOOL PreTranslateMessage(MSG* pMsg);
	CBrush m_Brush;
	static CcadDlg* pCaddlg;
//通信功能
public:
	unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);
//按钮重绘
public:
	CMyButton m_cad_btn_open;
	CMyButton m_cad_btn_draw;
	CMyButton m_cad_btn_send;
	afx_msg void OnBnClickedButtonCadOpen();
	afx_msg void OnBnClickedButtonCadDraw();
	afx_msg void OnBnClickedCadBtnOpvs();
	afx_msg void OnBnClickedCadBtnOpmod();
	afx_msg void OnBnClickedButtonCadSend();
	afx_msg void OnBnClickedCadBtnOpdata();
	afx_msg void OnBnClickedCadBtnOpmon();
	CMyButton m_cad_btn_opmon;
	CMyButton m_cad_btn_opvs;
	CMyButton m_cad_btn_opdata;
	CMyButton m_cad_btn_opmod;
	CMyButton m_cad_btn_opcad;
//logo
public:
	CStatic m_cad_pic_logo;
	HBITMAP m_cad_hBitmap_logo;
	//字体
	CFont f_cad_font;
	CFont f_cad_name;
	// 禁用发送图纸按钮
	void BanBtnSend();
	// 使能图纸发送按钮
	void EnableBtnSend();
};
