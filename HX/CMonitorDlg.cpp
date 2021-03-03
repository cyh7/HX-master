// CMonitorDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CMonitorDlg.h"
#include "afxdialogex.h"
#include "layoutinitMonitor.h"
#include "HXDlg.h"

// CMonitorDlg 对话框

IMPLEMENT_DYNAMIC(CMonitorDlg, CDialogEx)

CMonitorDlg::CMonitorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MONITOR, pParent)
	, m_mon_edit_type(_T(""))
	, m_mon_edit_batch(0)
{

}

CMonitorDlg::~CMonitorDlg()
{
}

void CMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MON_PIC_PLC, m_mon_pic_plc);
	DDX_Control(pDX, IDC_MON_PIC_GLUE, m_mon_pic_glue);
	DDX_Control(pDX, IDC_MON_PIC_STOP, m_mon_pic_stop);
	DDX_Control(pDX, IDC_MON_BTN_OPVS, m_mon_btn_opvs);
	DDX_Control(pDX, IDC_MON_BTN_OPCAD, m_mon_btn_opcad);
	DDX_Control(pDX, IDC_MON_BTN_OPDATA, m_mon_btn_opdata);
	DDX_Control(pDX, IDC_MON_BTN_OPMOD, m_mon_btn_opmod);
	DDX_Text(pDX, IDC_MON_EDIT_TYPE, m_mon_edit_type);
	DDX_Text(pDX, IDC_MON_EDIT_BATCH, m_mon_edit_batch);
	DDX_Text(pDX, IDC_MON_EDIT_X, m_mon_edit_x);
	DDX_Text(pDX, IDC_MON_EDIT_Y, m_mon_edit_y);
	DDX_Text(pDX, IDC_MON_EDIT_THETA, m_mon_edit_theta);
	DDX_Control(pDX, IDC_MON_BTN_OPMON, m_mon_btn_opmon);
	DDX_Control(pDX, IDC_MON_PIC_MAIN, m_mon_pic_main);
	DDX_Control(pDX, IDC_MON_PIC_LOGO, m_mon_pic_logo);
}


BEGIN_MESSAGE_MAP(CMonitorDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_MON_BTN_OPVS, &CMonitorDlg::OnBnClickedMonBtnOpvs)
	ON_BN_CLICKED(IDC_MON_BTN_OPCAD, &CMonitorDlg::OnBnClickedMonBtnOpcad)
	ON_BN_CLICKED(IDC_MON_BTN_OPDATA, &CMonitorDlg::OnBnClickedMonBtnOpdata)
	ON_BN_CLICKED(IDC_MON_BTN_OPMOD, &CMonitorDlg::OnBnClickedMonBtnOpmod)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMonitorDlg 消息处理程序


BOOL CMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_hIconRed = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_RED), IMAGE_ICON, 40, 40, LR_DEFAULTCOLOR);
	m_hIconBlue = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GREEN), IMAGE_ICON, 40, 40, LR_DEFAULTCOLOR);
	m_hIconYellow = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_YELLOW), IMAGE_ICON, 40, 40, LR_DEFAULTCOLOR);
	m_hIconGray = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GRAY), IMAGE_ICON, 40, 40, LR_DEFAULTCOLOR);

	//m_hBit = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 100, 100, LR_DEFAULTCOLOR);
	//没启动的时候
	//m_mon_pic_plc.SetBitmap(m_hBit);
	m_mon_pic_plc.SetIcon(m_hIconGray);
	m_mon_pic_glue.SetIcon(m_hIconGray);
	m_mon_pic_stop.SetIcon(m_hIconGray);

	m_Brush.CreateSolidBrush(RGB(240, 240, 220));
	
	//按钮绘制
	{
		GetDlgItem(IDC_MON_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mon_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mon_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mon_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mon_btn_opmon.setWordSize(200);

		GetDlgItem(IDC_MON_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mon_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mon_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mon_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mon_btn_opvs.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MON_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mon_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mon_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mon_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mon_btn_opcad.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MON_BTN_OPDATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mon_btn_opdata.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mon_btn_opdata.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mon_btn_opdata.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mon_btn_opdata.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MON_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mon_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mon_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mon_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mon_btn_opmod.setWordSize(200);
	}

	//字体
	{
		
		f_mon_font.CreateFontW(18,      // nHeight，文字大小
			0,          // nWidth
			0,          // nEscapement
			0,          // nOrientation
			FW_BOLD,    // nWeight，加粗
			FALSE,      // bItalic
			FALSE,      // bUnderline
			0,          // cStrikeOut
			ANSI_CHARSET,               // nCharSet
			OUT_DEFAULT_PRECIS,         // nOutPrecision
			CLIP_DEFAULT_PRECIS,        // nClipPrecision
			DEFAULT_QUALITY,            // nQuality
			DEFAULT_PITCH | FF_SWISS,   // nPitchAndFamily
			_T("微软雅黑"));       // lpszFac，字体
		//静态文本
		GetDlgItem(IDC_MON_STATIC_PLC)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_GLUE)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_STOP)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_TYPE)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_BATCH)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_COORD)->SetFont(&f_mon_font, false);
	
		GetDlgItem(IDC_MON_STATIC_X)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_Y)->SetFont(&f_mon_font, false);
		GetDlgItem(IDC_MON_STATIC_THETA)->SetFont(&f_mon_font, false);
		
		f_mon_name.CreateFontW(50,      // nHeight，文字大小
			0,          // nWidth
			0,          // nEscapement
			0,          // nOrientation
			FW_BOLD,    // nWeight，加粗
			FALSE,      // bItalic
			FALSE,      // bUnderline
			0,          // cStrikeOut
			ANSI_CHARSET,               // nCharSet
			OUT_DEFAULT_PRECIS,         // nOutPrecision
			CLIP_DEFAULT_PRECIS,        // nClipPrecision
			DEFAULT_QUALITY,            // nQuality
			DEFAULT_PITCH | FF_SWISS,   // nPitchAndFamily
			_T("楷体"));       // lpszFac，字体
		GetDlgItem(IDC_MON_STATIC_NAME)->SetFont(&f_mon_name, false);

	}
	InitLayoutMonitor(m_layoutMonitor, this);
	//全屏
	{
		WINDOWPLACEMENT m_struOldWndpl;
		//get current system resolution
		int g_iCurScreenWidth = GetSystemMetrics(SM_CXSCREEN); //1920
		int g_iCurScreenHeight = GetSystemMetrics(SM_CYSCREEN); //1080

		//for full screen while backplay
		GetWindowPlacement(&m_struOldWndpl);

		CRect rectWholeDlg;//entire client(including title bar)
		CRect rectClient;//client area(not including title bar)
		CRect rectFullScreen;
		//用于接收左上角和右下角的屏幕坐标
		GetWindowRect(&rectWholeDlg);
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);
		//将显示器上给定点或矩形的客户去坐标转换为屏幕坐标 新的坐标是相对于屏幕左上角的
		ClientToScreen(&rectClient);

		//-8 = 0 - 8
		rectFullScreen.left = rectWholeDlg.left - rectClient.left;
		//-28 = 0 - 28
		rectFullScreen.top = rectWholeDlg.top;
		// = 1088 + 1920 - 1080
		rectFullScreen.right = rectWholeDlg.right + g_iCurScreenWidth - rectClient.right;
		// = 639 + 1080 - 609
		rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom - 20;


		//enter into full screen;
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = rectFullScreen;
		SetWindowPlacement(&struWndpl);
	}
	
	m_mon_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
	m_mon_pic_logo.SetBitmap(m_mon_hBitmap_logo);
	//更换图片
	m_mon_hBitmapMain = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 700, 500, LR_DEFAULTCOLOR);
	m_mon_pic_main.SetBitmap(m_mon_hBitmapMain);
	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





HBRUSH CMonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor == CTLCOLOR_STATIC)//如果当前控件属于静态文本
	{
		//pDC->SetTextColor(RGB(255, 0, 0)); //字体颜色
		pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		pDC->SetBkMode(TRANSPARENT); //设置字体背景为透明
		//pDC->SetFont(p_font);//设置字体

		//pDC-> SetBkColor(RGB(0, 0, 255));  //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_MON_EDIT_TYPE || IDC_MON_EDIT_BATCH || IDC_MON_EDIT_X || IDC_MON_EDIT_Y || IDC_MON_EDIT_THETA)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	return hbr;
}


void CMonitorDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB(240, 240, 220));
	//dc.FillSolidRect(rect, RGB(125, 125, 255));

	CDialogEx::OnPaint();
					   // 不为绘图消息调用 CDialogEx::OnPaint()
}


void CMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	m_layoutMonitor.OnSize(cx, cy);
	// TODO: 在此处添加消息处理程序代码
}


BOOL CMonitorDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			OnBnClickedMonBtnOpdata();
		}
		if (pMsg->wParam == VK_F2)
		{
			OnBnClickedMonBtnOpvs();
		}
		if (pMsg->wParam == VK_F3)
		{
			OnBnClickedMonBtnOpcad();
		}
		if (pMsg->wParam == VK_F5)
		{
			OnBnClickedMonBtnOpmod();
		}
	}
	
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CMonitorDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnHelpInfo(pHelpInfo);
	return true;
}


void CMonitorDlg::OnBnClickedMonBtnOpvs()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowVision();
}


void CMonitorDlg::OnBnClickedMonBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CMonitorDlg::OnBnClickedMonBtnOpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowData();
}


void CMonitorDlg::OnBnClickedMonBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowModbus();
}


void CMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case 1:
		{
			
			//SprayBatch喷涂批次
			m_mon_edit_batch = SprayBatch;
			//backboard背板型号
			m_mon_edit_type = backboard;
			//
			
			m_mon_edit_x = vs_x;
			m_mon_edit_y = vs_y;
			m_mon_edit_theta = vs_theta;
			
			//PLC状态  胶机状态  急停 同步到控件里
			if (SprayFlag == true)
			{
				m_mon_pic_glue.SetIcon(m_hIconBlue);
			}
			else if (SprayFlag == false)
			{
				m_mon_pic_glue.SetIcon(m_hIconRed);
			}
			if (PlcFlag == true)
			{
				m_mon_pic_plc.SetIcon(m_hIconBlue);
			}
			else if (PlcFlag == false)
			{
				m_mon_pic_plc.SetIcon(m_hIconRed);
			}
			if (StopFlag == true)
			{
				m_mon_pic_stop.SetIcon(m_hIconRed);
			}
			else if (StopFlag == false)
			{
				m_mon_pic_stop.SetIcon(m_hIconBlue);
			}
			
			UpdateData(FALSE);

			
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
