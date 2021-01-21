
// HXDlg.h: 头文件
//
#include "CvisionDlg.h"
#include "CmodbusDlg.h"
#include "CcadDlg.h"
#include "CLayout.h"
#include "CdataDlg.h"
#include "CMonitorDlg.h"
#pragma once


// CHXDlg 对话框
class CHXDlg : public CDialogEx
{
// 构造
public:
	CHXDlg(CWnd* pParent = nullptr);	// 标准构造函数
	//主监控窗口声明
	CMonitorDlg m_Dlg4;
	//视觉窗口声明
	CvisionDlg m_Dlg1;
	//图纸窗口声明
	CcadDlg m_Dlg2;
	//系统设置窗口声明
	CmodbusDlg m_Dlg3;
	//数据库窗口声明
	CdataDlg m_Dlg5;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HICON m_myhIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedHxBtnVision();
	afx_msg void OnBnClickedHxBtnCad();
	afx_msg void OnBnClickedHxBtnModbus();
	afx_msg void OnBnClickedHxBtnMonitor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedHxBtnData();
	//注释了帮助，按F1帮助不会弹出
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//界面
public:
	//用以全屏
	CLayout m_layout;
	POINT old_HX;
	//状态栏
	CStatusBar m_Statusbar;
	//快捷键以及屏蔽按键设置
	BOOL PreTranslateMessage(MSG* pMsg);
	//改变控件颜色的画刷
	CBrush m_HX_Brush;
	//自绘按钮变量
	CMyButton m_hx_btn_vision;
	CMyButton m_hx_btn_cad;
	CMyButton m_hx_btn_modbus;
	CMyButton m_hx_btn_monitor;
	CMyButton m_hx_btn_data;

	void ShowVision();
	void ShowCad();
	void ShowModbus();
	void ShowData();
	void ShowMonitor();
	//主窗口图片
	CStatic m_hx_pic_main;
	HBITMAP m_hBitmapMain;
	//字体绘制
	CFont f_hx_word;
	CFont f_hx_title;
//菜单栏
public:
	//快捷键声明
	HACCEL hAccel;
	afx_msg void OnMenuF1();
	afx_msg void OnMenuF2();
	afx_msg void OnMenuF3();
	afx_msg void OnMenuF4();
	afx_msg void OnMenuF5();
	afx_msg void OnMenuLogin();
	afx_msg void OnMenuExit();
	afx_msg void OnAbout();
	afx_msg void OnExitAll();
};
