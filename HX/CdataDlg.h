#pragma once
#include <vector>
#include <string>
#include "CLayout.h"
#include "CMyButton.h"

// CdataDlg 对话框

class CdataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CdataDlg)

public:
	CdataDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CdataDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATABASE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//控件变量
	CListCtrl m_dat_list;
	CDateTimeCtrl m_dat_year;
	CDateTimeCtrl m_dat_time;
	CDateTimeCtrl m_dat_yearEnd;
	CDateTimeCtrl m_dat_timeEnd;
//程序功能
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CLayout m_layoutData;
//数据库函数
public:
	//让别的窗口调用本窗口函数
	static CdataDlg *pDatadlg;
	// 连接数据库
	BOOL ConnectDB();
	MYSQL m_sqlCon;
	// 查询的返回结果集
	MYSQL_RES *m_dat_res;
	// 获取每一条记录
	MYSQL_ROW m_dat_row;
	BOOL SelectDB();
	// 清空数据
	void ClearData();
	BOOL SelectDateDB();
	// 存放数据库记录，最大为100条
	//static std::vector<std::string> m_dat_data; //30240
	void GetDataFromDB();
	// 显示数据
	void ShowInfo();
	BOOL DeleteDB();
	BOOL ClearDB();
	BOOL InsertDB(CString time, CString type, DWORD batch, double x, double y, double theta, 
		CString good, CString plc, CString spray, CString stop);
//按钮重绘
public:
	CBrush m_Brush;
	afx_msg void OnBnClickedDatBtnOpvs();
	afx_msg void OnBnClickedDatBtnOpcad();
	afx_msg void OnBnClickedDatBtnOpmod();
	afx_msg void OnBnClickedDatBtnQuery();
	afx_msg void OnBnClickedDatBtnDelete();
	afx_msg void OnBnClickedDatBtnOpmon();
	afx_msg void OnBnClickedDatBtnClear();
	CMyButton m_dat_btn_query;
	CMyButton m_dat_btn_opmon;
	CMyButton m_dat_btn_opvs;
	CMyButton m_dat_btn_opcad;
	CMyButton m_dat_btn_opmod;
	CMyButton m_dat_btn_delete;
	CMyButton m_dat_btn_clear;
	CMyButton m_dat_btn_opdat;
//按钮和字体
public:
	CStatic m_data_pic_logo;
	HBITMAP m_data_hBitmap_logo;
	CFont f_data_font;
	CFont f_data_name;
};
