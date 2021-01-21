#pragma once
#include "CMyButton.h"

// CMonitorDlg 对话框

class CMonitorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorDlg)

public:
	CMonitorDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMonitorDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONITOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//大小适配
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//快捷键
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//取消F1弹出帮助
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//大小同步类
	CLayout m_layoutMonitor;
//指示灯图标
public:
	CStatic m_mon_pic_plc;
	CStatic m_mon_pic_glue;
	CStatic m_mon_pic_stop;
	HICON m_hIconRed;
	HICON m_hIconBlue;
	HICON m_hIconYellow;
	HICON m_hIconGray;
	//背景颜色画刷
	CBrush m_Brush;
	//HBITMAP m_hBit;
	//主图片和logo
	HBITMAP m_mon_hBitmapMain;
	CStatic m_mon_pic_main;
	CStatic m_mon_pic_logo;
	HBITMAP m_mon_hBitmap_logo;
//按键
public:
	//按键功能
	afx_msg void OnBnClickedMonBtnOpvs();
	afx_msg void OnBnClickedMonBtnOpcad();
	afx_msg void OnBnClickedMonBtnOpdata();
	afx_msg void OnBnClickedMonBtnOpmod();
	//按键重绘
	CMyButton m_mon_btn_opvs;
	CMyButton m_mon_btn_opcad;
	CMyButton m_mon_btn_opdata;
	CMyButton m_mon_btn_opmod;
	CMyButton m_mon_btn_opmon;
//监视区变量
public:
	// 背板型号
	CString m_mon_edit_type;
	//批次
	DWORD m_mon_edit_batch;
	//x坐标
	DOUBLE m_mon_edit_x;
	//y坐标
	DOUBLE m_mon_edit_y;
	//偏转角
	DOUBLE m_mon_edit_theta;
//字体
public:
	CFont f_mon_name;
	CFont f_mon_font;
};
