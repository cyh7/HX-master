#pragma once
#include "CmodbusDlg.h"
#include "CLayout.h"
#include "CMyButton.h"

#include "CamCon.h"//相机控制，必须第一引入，避免前面出现 using namespace cv;
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
extern int rect_height;


extern int left_baoguang_time;
extern int right_baoguang_time;
//左右相机智能指针
extern shared_ptr<SingleCam> leftCam;
extern shared_ptr<SingleCam> rightCam;

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
	CListCtrl m_vs_list_location_data;

	CString m_vs_edit_type;
	DWORD m_vs_edit_batch;
	

	//void OnShowList();
	HICON m_vs_hIconRed;
	HICON m_vs_hIconGreen;
	HICON m_vs_hIconYellow;
	HICON m_vs_hIconGray;
	
	void OnInitLocateData();
	void OnAllLeftLocate();
	void OnAllRightLocate();
	
	CStatic m_vs_pic_camstatus;
	CStatic m_vs_pic_locate;
	// 状态改变监测
	void StatusChange();
	CStatic m_vs_pic_plc;
	CStatic m_vs_pic_glue;
	CStatic m_vs_pic_stop;
};
