#pragma once
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"

extern bool SendDone;
//把时间全局给插入数据库用
extern CString LastTime;
extern double vs_x;
extern double vs_y;
extern double vs_theta;

extern int hv_Threshold_8;
extern int hv_Filter_block_radius_8;
extern int m_startPos_right_8_y;
extern int m_startPos_right_8_x;
extern int m_startPos_left_8_y;
extern int m_startPos_left_8_x;
extern int rect_width;
extern int rect_height;

extern int scale;


// CvisionDlg 对话框

class CvisionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CvisionDlg)

public:
	CvisionDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CvisionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VISION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//通信功能
public:
	unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);
//程序功能
public:
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CLayout m_layoutVision;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnPaint();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
//按键点击
public:
	afx_msg void OnBnClickedVisBtnOpmon();
	afx_msg void OnBnClickedVisBtnOpdata();
	afx_msg void OnBnClickedVisBtnOpcad();
	afx_msg void OnBnClickedVisBtnOpmod();
	afx_msg void OnBnClickedVsBtnResend();
//按键重绘
public:
	CBrush m_Brush;
	CMyButton m_vs_btn_opmon;
	CMyButton m_vs_btn_opcad;
	CMyButton m_vs_btn_opdata;
	CMyButton m_vs_btn_opmod;
	CMyButton m_vs_btn_opvs;
	CMyButton m_vs_btn_resend;
	//图片
	CStatic m_vs_pic_logo;
	HBITMAP m_vs_hBitmap_logo;
	//字体
	CFont f_vs_font;
	CFont f_vs_name;
	// 停止定时器1
	void KillTime1();
	// 激活定时器1
	void ReSetTime();
	//用以外界调用vision里的函数
	static CvisionDlg *pVisiondlg;
	
	
	void OnShowLeftPic();
	void OnShowRightPic();
	void OnLeftCollectAndCompress();
	void OnRightCollectAndCompress();
	CStatic m_vs_pic_8_left;
	CStatic m_vs_pic_8_right;
	void OnLeftLocate();
	void OnRightLocate();

	std::shared_ptr<CClientDC> dc_left_8_ptr;

	/*CWnd* pwnd_right_8;
	CClientDC dc_right_8;*/
	std::shared_ptr<CClientDC> dc_right_8_ptr;
	CPen pen_left_8;
	afx_msg void OnBnClickedButton1();
	CListCtrl m_vs_list_location_data;

	CString m_vs_edit_type;
	DWORD m_vs_edit_batch;
	

	void OnShowList();
	int m_test_locate_num;
	int m_locate_test_wrong;
	void OnInitLocateData();
	void OnAllLeftLocate();
	void OnAllRightLocate();
	afx_msg void OnBnClickedButton2();
};
