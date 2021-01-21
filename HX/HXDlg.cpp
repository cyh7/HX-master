
// HXDlg.cpp: 实现文件


#include "pch.h"
#include "framework.h"
#include "HX.h"
#include "HXDlg.h"
#include "afxdialogex.h"
#include "layoutinit.h"
#include "CLoginDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//状态栏时间
CTime StatusTime;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHXDlg 对话框



CHXDlg::CHXDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_GDUT);
}

void CHXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HX_BTN_VISION, m_hx_btn_vision);
	DDX_Control(pDX, IDC_HX_BTN_CAD, m_hx_btn_cad);
	DDX_Control(pDX, IDC_HX_BTN_MODBUS, m_hx_btn_modbus);
	DDX_Control(pDX, IDC_HX_BTN_MONITOR, m_hx_btn_monitor);
	DDX_Control(pDX, IDC_HX_BTN_DATA, m_hx_btn_data);
	DDX_Control(pDX, IDC_HX_PIC_MAIN, m_hx_pic_main);
}

BEGIN_MESSAGE_MAP(CHXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_HX_BTN_VISION, &CHXDlg::OnBnClickedHxBtnVision)
	ON_BN_CLICKED(IDC_HX_BTN_CAD, &CHXDlg::OnBnClickedHxBtnCad)
	ON_BN_CLICKED(IDC_HX_BTN_MODBUS, &CHXDlg::OnBnClickedHxBtnModbus)
	ON_BN_CLICKED(IDC_HX_BTN_DATA, &CHXDlg::OnBnClickedHxBtnData)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_MENU_F1, &CHXDlg::OnMenuF1)
	ON_COMMAND(ID_MENU_F2, &CHXDlg::OnMenuF2)
	ON_COMMAND(ID_MENU_F3, &CHXDlg::OnMenuF3)
	ON_COMMAND(ID_MENU_F4, &CHXDlg::OnMenuF4)
	ON_COMMAND(ID_MENU_F5, &CHXDlg::OnMenuF5)
	ON_COMMAND(ID_MENU_LOGIN, &CHXDlg::OnMenuLogin)
	ON_COMMAND(ID_MENU_EXIT, &CHXDlg::OnMenuExit)
	ON_BN_CLICKED(IDC_HX_BTN_MONITOR, &CHXDlg::OnBnClickedHxBtnMonitor)
	ON_COMMAND(ID_32772, &CHXDlg::OnAbout)
	ON_COMMAND(ID_32794, &CHXDlg::OnExitAll)
END_MESSAGE_MAP()


// CHXDlg 消息处理程序

BOOL CHXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	//SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_myhIcon = LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(IDI_GDUT));

	SetIcon(m_myhIcon, TRUE);
	SetIcon(m_myhIcon, FALSE);

	// TODO: 在此添加额外的初始化代码
	//菜单栏设置
	CMenu menu;
	menu.LoadMenuW(IDR_MENU1);
	SetMenu(&menu);

	hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MENU1));

	CRect rect;
	GetClientRect(&rect);
	old_HX.x = rect.right - rect.left;
	old_HX.y = rect.bottom - rect.top;


	//给状态栏添加时间更新
	SetTimer(1, 1000, NULL);

	//添加状态栏
	//获得系统当前时间
	//CTime StatusTime;
	StatusTime = StatusTime.GetCurrentTime();
	CString InitTime = StatusTime.Format("%Y-%m-%d %H:%M:%S");


	//创建状态栏
	UINT array[2] = { 12301,12302 };    //注：这里是ID号，应保证不与resource.h中的其他ID号重复
	m_Statusbar.Create(this);
	m_Statusbar.SetIndicators(array, sizeof(array) / sizeof(UINT));

	//显示状态栏
	CRect StatusRect;
	GetWindowRect(StatusRect);
	m_Statusbar.SetPaneInfo(0, array[0], 0, StatusRect.Width() / 3);
	m_Statusbar.SetPaneInfo(1, array[1], 0, StatusRect.Width() / 3 * 2);
	m_Statusbar.SetPaneText(0, InitTime);
	m_Statusbar.SetPaneText(1, _T("通信状态"));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);


	
	
	//这个也是无法准确获得分辨率
	/*int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);

	CRect rcTemp;
	rcTemp.BottomRight() = CPoint(cx, cy);
	rcTemp.TopLeft() = CPoint(0, 0);
	
	MoveWindow(&rcTemp);*/

	//全屏操作 换肤之后彻底不行
	//ModifyStyle(WS_CAPTION, 0, 0);  // 如果只是要普通的全屏，不想去掉标题栏，就不用第一个语句
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	//全屏幕操作
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
		rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom - 10;

		/*rectFullScreen.right = g_iCurScreenWidth;
		rectFullScreen.bottom = g_iCurScreenHeight;*/
		//CString tt;
		//tt.Format(_T("%d"), rectFullScreen.right);//前后之差即程序运行时间  
		//MessageBox(tt);

		//enter into full screen;
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = rectFullScreen;
		SetWindowPlacement(&struWndpl);
	}
	

	//分别创建三个非模态窗口，并隐藏
	m_Dlg5.Create(IDD_DATABASE);
	m_Dlg5.ShowWindow(SW_HIDE);
	
	m_Dlg2.Create(IDD_CAD);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.Create(IDD_MODBUS);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.Create(IDD_MONITOR);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg1.Create(IDD_VISION);
	m_Dlg1.ShowWindow(SW_HIDE);
	//背景颜色画刷
	m_HX_Brush.CreateSolidBrush(RGB(240, 240, 220));

	//按钮重绘
	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	GetDlgItem(IDC_HX_BTN_VISION)->ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	m_hx_btn_vision.SetDownColor(RGB(102, 139, 139));
	//设置Button Up的背景色
	m_hx_btn_vision.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_hx_btn_vision.setWordColor(RGB(255, 250, 250));
	//设置字体大小
	m_hx_btn_vision.setWordSize(200);

	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	GetDlgItem(IDC_HX_BTN_CAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	m_hx_btn_cad.SetDownColor(RGB(102, 139, 139));
	//设置Button Up的背景色
	m_hx_btn_cad.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_hx_btn_cad.setWordColor(RGB(255, 250, 250));
	//设置字体大小
	m_hx_btn_cad.setWordSize(200);

	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	GetDlgItem(IDC_HX_BTN_MODBUS)->ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	m_hx_btn_modbus.SetDownColor(RGB(102, 139, 139));
	//设置Button Up的背景色
	m_hx_btn_modbus.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_hx_btn_modbus.setWordColor(RGB(255, 250, 250));
	//设置字体大小
	m_hx_btn_modbus.setWordSize(200);

	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	GetDlgItem(IDC_HX_BTN_MONITOR)->ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	m_hx_btn_monitor.SetDownColor(RGB(102, 139, 139));
	//设置Button Up的背景色
	m_hx_btn_monitor.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_hx_btn_monitor.setWordColor(RGB(255, 250, 250));
	//设置字体大小
	m_hx_btn_monitor.setWordSize(200);

	//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
	GetDlgItem(IDC_HX_BTN_DATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
	//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
	m_hx_btn_data.SetDownColor(RGB(102, 139, 139));
	//设置Button Up的背景色
	m_hx_btn_data.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_hx_btn_data.setWordColor(RGB(255, 250, 250));
	//设置字体大小
	m_hx_btn_data.setWordSize(200);

	//静态文本字体改变
	f_hx_title.CreateFontW(70,      // nHeight，文字大小
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

	GetDlgItem(IDC_HX_STATIC_TITLE)->SetFont(&f_hx_title, false);

	f_hx_word.CreateFontW(50,      // nHeight，文字大小
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
	GetDlgItem(IDC_HX_STATIC_GDUT)->SetFont(&f_hx_word, false);
	GetDlgItem(IDC_HX_STATIC_GDUT2)->SetFont(&f_hx_word, false);
	//InitLayout(m_layout, this);
	m_hBitmapMain = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 600, 500, LR_DEFAULTCOLOR);
	m_hx_pic_main.SetBitmap(m_hBitmapMain);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHXDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_MOVE || nID == 0xF012)
		return;
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CHXDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (1)
	{
		float fsp[2];
		POINT Newp; //获取现在对话框的大小
		CRect recta;
		GetClientRect(&recta);     //取客户区大小  
		Newp.x = recta.right - recta.left;
		Newp.y = recta.bottom - recta.top;
		fsp[0] = (float)Newp.x / old_HX.x;
		fsp[1] = (float)Newp.y / old_HX.y;
		CRect Rect;
		int woc;
		CPoint OldTLPoint, TLPoint; //左上角
		CPoint OldBRPoint, BRPoint; //右下角
		HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件  
		while (hwndChild)
		{
			woc = ::GetDlgCtrlID(hwndChild);//取得ID
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			OldBRPoint = Rect.BottomRight();
			BRPoint.x = long(OldBRPoint.x *fsp[0]);
			BRPoint.y = long(OldBRPoint.y *fsp[1]);
			Rect.SetRect(TLPoint, BRPoint);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
		}
		old_HX = Newp;
	}

	// TODO: 在此处添加消息处理程序代码
	//m_layout.OnSize(cx, cy);

}


void CHXDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}


void CHXDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		StatusTime = StatusTime.GetCurrentTime();
		CString sTime = StatusTime.Format("%Y-%m-%d %H:%M:%S");
		m_Statusbar.SetPaneText(0, sTime);
		
		if (ConnectClose == true)
		{
			m_Statusbar.SetPaneText(1, _T("通信状态: 通信关闭"));
		}
		else
		{
			if (DisconnectFlag == false)
			{
				m_Statusbar.SetPaneText(1, _T("通信状态: 连接正常"));
			}
			else if (DisconnectFlag == true)
			{
				m_Statusbar.SetPaneText(1, _T("通信状态: 连接中断"));
			}
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CHXDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (::TranslateAccelerator(GetSafeHwnd(), hAccel, pMsg))
			return TRUE;
	}
	
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CHXDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (pWnd->GetDlgCtrlID() == IDC_HX_STATIC_TITLE)//如果当前控件属于静态文本
	{
		//pDC->SetTextColor(RGB(255, 0, 0)); //字体颜色
		pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		pDC->SetBkMode(TRANSPARENT); //设置字体背景为透明
		
		return (HBRUSH)m_HX_Brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_HX_STATIC_GDUT || IDC_HX_STATIC_GDUT2)//如果当前控件属于静态文本
	{
		//pDC->SetTextColor(RGB(255, 0, 0)); //字体颜色
		pDC->SetTextColor(RGB(238, 44, 44));  //字体颜色
		pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		pDC->SetBkMode(TRANSPARENT); //设置字体背景为透明

		return (HBRUSH)m_HX_Brush.GetSafeHandle();
	}

	return (HBRUSH)m_HX_Brush.GetSafeHandle();;
	//return hbr;
}

void CHXDlg::OnBnClickedHxBtnMonitor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dlg4.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("喷胶主监控");
	this->SetWindowText(title);
}
void CHXDlg::OnBnClickedHxBtnVision()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dlg1.ShowWindow(SW_SHOW);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);

	CString title;
	title = _T("视觉处理");
	this->SetWindowText(title);
}

void CHXDlg::OnBnClickedHxBtnCad()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dlg2.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("CAD文件读取");
	this->SetWindowText(title);
}

void CHXDlg::OnBnClickedHxBtnModbus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (LoginFlag == false)
	{
		m_Dlg4.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg3.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("喷胶主监控");
		this->SetWindowText(title);
		MessageBox(_T("请登录管理员账户"));
	}
	else
	{
		m_Dlg3.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg4.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("系统设置");
		this->SetWindowText(title);
	}
}

void CHXDlg::OnBnClickedHxBtnData()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dlg5.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	CString title;
	title = _T("数据库");
	this->SetWindowText(title);
}

void CHXDlg::ShowVision()
{
	// TODO: 在此处添加实现代码.
	m_Dlg1.ShowWindow(SW_SHOW);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("视觉处理");
	this->SetWindowText(title);
}


void CHXDlg::ShowCad()
{
	// TODO: 在此处添加实现代码.
	m_Dlg2.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("CAD文件读取");
	this->SetWindowText(title);
}


void CHXDlg::ShowModbus()
{
	// TODO: 在此处添加实现代码.
	if (LoginFlag == false)
	{
		m_Dlg4.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg3.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("喷胶主监控");
		this->SetWindowText(title);
		MessageBox(_T("请登录管理员账户"));
	}
	else
	{
		m_Dlg3.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg4.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("系统设置");
		this->SetWindowText(title);
	}
	/*m_Dlg3.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("系统设置");
	this->SetWindowText(title);*/
}

void CHXDlg::ShowData()
{
	// TODO: 在此处添加实现代码.
	m_Dlg5.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	CString title;
	title = _T("数据库");
	this->SetWindowText(title);
}

void CHXDlg::ShowMonitor()
{
	// TODO: 在此处添加实现代码.
	m_Dlg4.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("喷胶主监控");
	this->SetWindowText(title);
}

BOOL CHXDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnHelpInfo(pHelpInfo);
	return true;
}


void CHXDlg::OnMenuF1()
{
	// TODO: 在此添加命令处理程序代码
	m_Dlg4.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("喷胶主监控");
	this->SetWindowText(title);
}


void CHXDlg::OnMenuF2()
{
	// TODO: 在此添加命令处理程序代码
	m_Dlg1.ShowWindow(SW_SHOW);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);

	CString title;
	title = _T("视觉处理");
	this->SetWindowText(title);
}


void CHXDlg::OnMenuF3()
{
	// TODO: 在此添加命令处理程序代码
	m_Dlg2.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("CAD文件读取");
	this->SetWindowText(title);
}


void CHXDlg::OnMenuF4()
{
	// TODO: 在此添加命令处理程序代码
	m_Dlg5.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg3.ShowWindow(SW_HIDE);
	CString title;
	title = _T("数据库");
	this->SetWindowText(title);
}


void CHXDlg::OnMenuF5()
{
	// TODO: 在此添加命令处理程序代码
	if (LoginFlag == false)
	{
		m_Dlg4.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg3.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("喷胶主监控");
		this->SetWindowText(title);
		MessageBox(_T("请登录管理员账户"));
	}
	else
	{
		m_Dlg3.ShowWindow(SW_SHOW);
		m_Dlg1.ShowWindow(SW_HIDE);
		m_Dlg2.ShowWindow(SW_HIDE);
		m_Dlg4.ShowWindow(SW_HIDE);
		m_Dlg5.ShowWindow(SW_HIDE);
		CString title;
		title = _T("系统设置");
		this->SetWindowText(title);
	}
	/*m_Dlg3.ShowWindow(SW_SHOW);
	m_Dlg1.ShowWindow(SW_HIDE);
	m_Dlg4.ShowWindow(SW_HIDE);
	m_Dlg2.ShowWindow(SW_HIDE);
	m_Dlg5.ShowWindow(SW_HIDE);
	CString title;
	title = _T("系统设置");
	this->SetWindowText(title);*/
}


void CHXDlg::OnMenuLogin()
{
	// TODO: 在此添加命令处理程序代码
	if (LoginFlag == true)
	{
		MessageBox(_T("管理员已登录"));
		return;
	}
	if (LoginFlag == false)
	{
		CLoginDlg loginDlg;
		loginDlg.DoModal();
	}
	
}


void CHXDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	LoginFlag = false;
	MessageBox(_T("管理员已退出！"));
}








void CHXDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CHXDlg::OnExitAll()
{
	// TODO: 在此添加命令处理程序代码
	
	exitFlag = true;
	Sleep(100);
	CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;
	pdlg->OnClose();
	Sleep(100);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
