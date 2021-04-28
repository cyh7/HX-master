// CdataDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CdataDlg.h"
#include "afxdialogex.h"
#include "layoutinitData.h"
#include "HXDlg.h"
 
//正在插入数据标志位
bool IsInsert = false;
//查询失败标志位
bool IsConnOpen = false;
//连接状态标志位
bool ConnectSucces = false;
CdataDlg *CdataDlg::pDatadlg = NULL;
// CdataDlg 对话框
// 存放数据库记录，最大为40000条
std::vector<std::string> m_dat_data[40000];
//查询日期没有超过七天
bool QueryDayFlag = true;


IMPLEMENT_DYNAMIC(CdataDlg, CDialogEx)

CdataDlg::CdataDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATABASE, pParent)
{

}

CdataDlg::~CdataDlg()
{
}

void CdataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DAT_LIST, m_dat_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dat_year);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dat_time);
	DDX_Control(pDX, IDC_DAT_BTN_QUERY, m_dat_btn_query);
	DDX_Control(pDX, IDC_DAT_BTN_OPMON, m_dat_btn_opmon);
	DDX_Control(pDX, IDC_DAT_BTN_OPVS, m_dat_btn_opvs);
	DDX_Control(pDX, IDC_DAT_BTN_OPCAD, m_dat_btn_opcad);
	DDX_Control(pDX, IDC_DAT_BTN_OPMOD, m_dat_btn_opmod);
	DDX_Control(pDX, IDC_DAT_BTN_DELETE, m_dat_btn_delete);
	DDX_Control(pDX, IDC_DAT_BTN_CLEAR, m_dat_btn_clear);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_dat_yearEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_dat_timeEnd);
	DDX_Control(pDX, IDC_DAT_BTN_OPDAT, m_dat_btn_opdat);
	DDX_Control(pDX, IDC_DATA_PIC_LOGO, m_data_pic_logo);
}


BEGIN_MESSAGE_MAP(CdataDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DAT_BTN_OPVS, &CdataDlg::OnBnClickedDatBtnOpvs)
	ON_BN_CLICKED(IDC_DAT_BTN_OPCAD, &CdataDlg::OnBnClickedDatBtnOpcad)
	ON_BN_CLICKED(IDC_DAT_BTN_OPMOD, &CdataDlg::OnBnClickedDatBtnOpmod)
	ON_BN_CLICKED(IDC_DAT_BTN_QUERY, &CdataDlg::OnBnClickedDatBtnQuery)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_DAT_BTN_OPMON, &CdataDlg::OnBnClickedDatBtnOpmon)
	ON_BN_CLICKED(IDC_DAT_BTN_DELETE, &CdataDlg::OnBnClickedDatBtnDelete)
	ON_BN_CLICKED(IDC_DAT_BTN_CLEAR, &CdataDlg::OnBnClickedDatBtnClear)
END_MESSAGE_MAP()


// CdataDlg 消息处理程序


BOOL CdataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pDatadlg = this;
	// TODO:  在此添加额外的初始化
	m_dat_list.InsertColumn(0, TEXT("日期"), 0, 210);
	m_dat_list.InsertColumn(1, TEXT("背板型号"), 0, 120);
	m_dat_list.InsertColumn(2, TEXT("喷涂批次"), 0, 80);
	m_dat_list.InsertColumn(3, TEXT("X左侧坐标"), 0, 100);
	m_dat_list.InsertColumn(4, TEXT("Y左侧坐标"), 0, 100);
	m_dat_list.InsertColumn(5, TEXT("X右侧坐标"), 0, 100);
	m_dat_list.InsertColumn(6, TEXT("Y右侧坐标"), 0, 100);
	m_dat_list.InsertColumn(7, TEXT("偏转角"), 0, 100);
	m_dat_list.InsertColumn(8, TEXT("定位状态"), 0, 120);
	m_dat_list.InsertColumn(9, TEXT("PLC状态"), 0, 120);
	m_dat_list.InsertColumn(10, TEXT("胶机状态"), 0, 120);
	m_dat_list.InsertColumn(11, TEXT("急停"), 0, 120);

	

	
	//按钮绘制
	{
		GetDlgItem(IDC_DAT_BTN_QUERY)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_dat_btn_query.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_query.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_query.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_query.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_DELETE)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_dat_btn_delete.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_delete.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_delete.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_delete.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_CLEAR)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_dat_btn_clear.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_clear.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_clear.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_clear.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		m_dat_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_opmon.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		m_dat_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_opvs.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		m_dat_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_opcad.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_OPDAT)->ModifyStyle(0, BS_OWNERDRAW, 0);
		m_dat_btn_opdat.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_opdat.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_opdat.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_opdat.setWordSize(200);

		GetDlgItem(IDC_DAT_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		m_dat_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_dat_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_dat_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_dat_btn_opmod.setWordSize(200);
	}
	//字体改变
	
	f_data_name.CreateFontW(50,      // nHeight，文字大小
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
	GetDlgItem(IDC_DAT_STATIC_NAME)->SetFont(&f_data_name, false);

	f_data_font.CreateFontW(18,      // nHeight，文字大小
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
	GetDlgItem(IDC_DAT_STATIC_TIME)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DAT_STATIC_TIMEEND)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DATETIMEPICKER1)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DATETIMEPICKER2)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DATETIMEPICKER3)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DATETIMEPICKER4)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DAT_STATIC1)->SetFont(&f_data_font, false);
	GetDlgItem(IDC_DAT_STATIC2)->SetFont(&f_data_font, false);
	m_Brush.CreateSolidBrush(RGB(240, 240, 220));
	InitLayoutData(m_layoutData, this);

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

	m_data_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
	m_data_pic_logo.SetBitmap(m_data_hBitmap_logo);

	ConnectDB();
	//if (ConnectSucces == true)
	//{
	//	//查询数据
	//	SelectDB();
	//	//获取数据;
	//	GetDataFromDB();
	//	//显示数据
	//	ShowInfo();
	//}

	////查询数据
	//SelectDB();
	////获取数据;
	//GetDataFromDB();
	////显示数据
	//ShowInfo();

	SetTimer(1, 600 * 1000, NULL);//每隔10min判断重连一次数据库
	//SetTimer(2, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CdataDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_layoutData.OnSize(cx, cy);
}


HBRUSH CdataDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	return hbr;
}


void CdataDlg::OnPaint()
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


void CdataDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			if (ConnectSucces == false)
			{
				ConnectDB();
			}
		}
		break;
	case 2:
	{
		/*CTime StatusTime;
		StatusTime = StatusTime.GetCurrentTime();
		CString sTime = StatusTime.Format("%Y-%m-%d %H:%M:%S");
		CString test_Type = _T("A型");
		int n = 1;
		double test_x_loc = 1.12;
		double test_y_loc = 1.13;
		double test_theta_loc = 1.14;
		CString test_Good = _T("良品");
		CString test_PLc = _T("PLC正常");
		CString test_Spray = _T("胶机正常");
		CString test_Stop = _T("无急停");
		

		InsertDB(sTime, test_Type, n, test_x_loc, test_y_loc, test_theta_loc, test_Good, test_PLc, test_Spray, test_Stop);*/
	}
	}
	CDialogEx::OnTimer(nIDEvent);
}


// 连接数据库
BOOL CdataDlg::ConnectDB()
{
	// TODO: 在此处添加实现代码.
	mysql_init(&m_sqlCon);
	//mysqlCon 数据库结构 localhost 主机名 root 用户名 "" 密码为空   test 数据库 3306 端口
	mysql_options(&m_sqlCon, MYSQL_SET_CHARSET_NAME, "gbk");

	/*if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123", "test", 3306, NULL, 0))
		return FALSE;
	return TRUE;*/
	try
	{
		if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "1234", "test", 3306, NULL, 0))
			return false;
		ConnectSucces = true;
	}
	catch (_com_error)
	{
		ConnectSucces = false;
	}
	return TRUE;
}

//初步查询所有数据
BOOL CdataDlg::SelectDB()
{
	// TODO: 在此处添加实现代码.
	UpdateData(TRUE);
	ClearData();
	CString cquery;
	//条件全部为空则查询所有书籍

	cquery.Format(_T("select * from table2 where DATE_SUB(CURDATE(), INTERVAL 6 DAY) <= 日期"));
	//CString转const char*
	//const char* query = CString(cquery);
	//cstring转 const char*
	const char* query;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)cquery);
	query = temp;
	

	try
	{
		//查询数据
		if (mysql_query(&m_sqlCon, query))//执行指定为一个空结尾的字符串的SQL查询。
			return false;
		//获取结果集
		m_dat_res = mysql_store_result(&m_sqlCon); //检索一个完整的结果集合给客户

	}
	catch(_com_error)
	{
		IsConnOpen = false;
	}
	return TRUE;
}


// 清空数据
void CdataDlg::ClearData()
{
	// TODO: 在此处添加实现代码.
	int i = 0;
	while (!m_dat_data[i].empty())
	{
		m_dat_data[i++].clear();
	}
}

//获取数据
void CdataDlg::GetDataFromDB()
{
	// TODO: 在此处添加实现代码.
	//容器是二维数组
	int i = 0;
	while (m_dat_row = mysql_fetch_row(m_dat_res))//从结果集合中取得下一行
	{
		m_dat_data[i].push_back(m_dat_row[0]);
		m_dat_data[i].push_back(m_dat_row[1]);
		m_dat_data[i].push_back(m_dat_row[2]);
		m_dat_data[i].push_back(m_dat_row[3]);
		m_dat_data[i].push_back(m_dat_row[4]);
		m_dat_data[i].push_back(m_dat_row[5]);
		m_dat_data[i].push_back(m_dat_row[6]);
		m_dat_data[i].push_back(m_dat_row[7]);
		m_dat_data[i].push_back(m_dat_row[8]);
		m_dat_data[i].push_back(m_dat_row[9]);
		m_dat_data[i].push_back(m_dat_row[10]);
		m_dat_data[i].push_back(m_dat_row[11]);
		m_dat_data[i++].push_back(m_dat_row[12]);
		//m_dat_data[i++].push_back(m_dat_row[11]);
	}
}


// 显示数据
void CdataDlg::ShowInfo()
{
	// TODO: 在此处添加实现代码.
	m_dat_list.DeleteAllItems();
	for (int i = 0; i < m_dat_res->row_count; i++)
	{
		int iCount = m_dat_list.GetItemCount();//行数
		CString temp;
		//string转cstring
		temp = m_dat_data[i][0].c_str();
		m_dat_list.InsertItem(iCount, temp);//先把第0行第0列插入进去了
		//插入后续的9列 还有默认的一个空格
		for (int j = 0; j < m_sqlCon.field_count - 1; j++)//m_sqlCon.field_count = 11
		{
			temp = m_dat_data[i][j + 1].c_str();
			m_dat_list.SetItemText(iCount, j + 1, temp);
		}

	}
}


void CdataDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	mysql_free_result(m_dat_res);
	//关闭数据库
	mysql_close(&m_sqlCon);
}

void CdataDlg::OnBnClickedDatBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CdataDlg::OnBnClickedDatBtnOpvs()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowVision();
}


void CdataDlg::OnBnClickedDatBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CdataDlg::OnBnClickedDatBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowModbus();
}


BOOL CdataDlg::SelectDateDB()
{
	// TODO: 在此处添加实现代码.
	UpdateData(TRUE);
	ClearData();
	CString cquery;
	//条件全部为空则查询所有书籍
	CTime year;
	m_dat_year.GetTime(year);
	CString sYear = year.Format("%Y-%m-%d");
	CTime time;
	m_dat_time.GetTime(time);
	CString sTime = time.Format("%H:%M:%S");
	CTime yearEnd;
	m_dat_yearEnd.GetTime(yearEnd);
	CString sYearEnd = yearEnd.Format("%Y-%m-%d");
	CTime timeEnd;
	m_dat_timeEnd.GetTime(timeEnd);
	CString sTimeEnd = timeEnd.Format("%H:%M:%S");

	int s_day = year.GetDay();
	int e_day = yearEnd.GetDay();
	if ((e_day - s_day) >= 7)
	{
		QueryDayFlag = false;
		AfxMessageBox(_T("起始日期和终止日期不能超过7天"));
		return FALSE;
	}
	else
	{
		QueryDayFlag = true;
		cquery.Format(_T("select * from table2 where 日期 >= CONCAT('%s',' ','%s') and 日期 <= CONCAT('%s',' ','%s')"), sYear, sTime, sYearEnd, sTimeEnd);
		//CString转const char*
		//const char* query = CString(cquery);
		//cstring转 const char*
		const char* query;
		char temp[1024];
		::wsprintfA(temp, "%ls", (LPCTSTR)cquery);
		query = temp;
		//查询数据
		//if (mysql_query(&m_sqlCon, query)) //执行指定为一个空结尾的字符串的SQL查询。
		//	return FALSE;
		////获取结果集
		//m_dat_res = mysql_store_result(&m_sqlCon); //检索一个完整的结果集合给客户
		try
		{
			if (mysql_query(&m_sqlCon, query))
				return false;
			m_dat_res = mysql_store_result(&m_sqlCon);
		}
		catch (_com_error)
		{
			IsConnOpen = false;
		}
		return TRUE;
	}
	
}

BOOL CdataDlg::DeleteDB()
{
	// TODO: 在此处添加实现代码.
	UpdateData(TRUE);
	//ClearData();
	CTime year;
	m_dat_year.GetTime(year);
	CString sYear = year.Format("%Y-%m-%d");
	CTime time;
	m_dat_time.GetTime(time);
	CString sTime = time.Format("%H:%M:%S");
	CTime yearEnd;
	m_dat_yearEnd.GetTime(yearEnd);
	CString sYearEnd = yearEnd.Format("%Y-%m-%d");
	CTime timeEnd;
	m_dat_timeEnd.GetTime(timeEnd);
	CString sTimeEnd = timeEnd.Format("%H:%M:%S");

	//可以多选然后删除
	CString cquery;

	//删除所有DELETE FROM table_name
	//truncate table table1

	cquery.Format(TEXT("delete from table2 where 日期 >= CONCAT('%s',' ','%s') and 日期 <= CONCAT('%s',' ','%s')"), sYear, sTime, sYearEnd, sTimeEnd);

	const char* query;
	char temp[1024];
	::wsprintfA(temp, "%ls", (LPCTSTR)cquery);
	query = temp;

	/*if (mysql_query(&m_sqlCon, query))
	{
		AfxMessageBox(TEXT("删除数据失败！"));
		return FALSE;
	}*/
	try
	{
		if (mysql_query(&m_sqlCon, query))
			return false;
		//mysql_query(&m_sqlCon, query);
	}
	catch (_com_error )
	{
		IsConnOpen = false;
	}

	return TRUE;
}

void CdataDlg::OnBnClickedDatBtnQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsInsert == true)
		Sleep(50);
	SelectDateDB();
	//如果查询超过七天,return
	if (QueryDayFlag == false)
	{
		return;
	}
	GetDataFromDB();
	//显示数据
	ShowInfo();
}





BOOL CdataDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnHelpInfo(pHelpInfo);
}


BOOL CdataDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			OnBnClickedDatBtnOpmon();
		}
		if (pMsg->wParam == VK_F2)
		{
			OnBnClickedDatBtnOpvs();
		}
		if (pMsg->wParam == VK_F3)
		{
			OnBnClickedDatBtnOpcad();
		}
		if (pMsg->wParam == VK_F5)
		{
			OnBnClickedDatBtnOpmod();
		}
	}
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}





void CdataDlg::OnBnClickedDatBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsInsert == true)
		Sleep(50);
	DeleteDB();
	m_dat_list.DeleteAllItems(); //清空所有表项
	//查询数据
	SelectDB();
	//获取数据;
	GetDataFromDB();
	//显示数据
	ShowInfo();
}


BOOL CdataDlg::ClearDB()
{
	// TODO: 在此处添加实现代码.
	CString cquery;

	//删除所有DELETE FROM table_name
	//truncate table table1

	cquery.Format(TEXT("truncate table table2"));

	const char* query;
	char temp[100];
	::wsprintfA(temp, "%ls", (LPCTSTR)cquery);
	query = temp;

	if (mysql_query(&m_sqlCon, query))
	{
		AfxMessageBox(TEXT("清空数据失败！"));
		return FALSE;
	}

	return TRUE;
}


void CdataDlg::OnBnClickedDatBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsInsert == true)
		Sleep(50);
	ClearDB(); 
	m_dat_list.DeleteAllItems();
	UpdateData(FALSE);
	//GetDataFromDB();
	//显示数据
	//ShowInfo();
}


BOOL CdataDlg::InsertDB(CString time, CString type, DWORD batch, double x, double y,double x_right,double y_right, double theta, CString good, CString plc, CString spray, CString stop)
{
	// TODO: 在此处添加实现代码.
	CString cquery;

	CString test;
	test = _T("a");
	cquery.Format(_T("insert into table2 values('%s','%s','%d',%.3f ,%.3f ,%.3f,%.3f,%.3f ,'%s','%s','%s','%s');"), time, type, batch, x, y,x_right,y_right, theta, good, plc, spray, stop);

	const char* query;
	char temp[1024];
	::wsprintfA(temp, "%ls", (LPCTSTR)cquery);
	query = temp;

	/*if (mysql_query(&m_sqlCon, query))
	{
		MessageBox(TEXT("插入数据失败！"));
		return FALSE;
	}*/
	IsInsert = true;
	try
	{
		if (mysql_query(&m_sqlCon, query))
			return false;
	}
	catch (_com_error)
	{
		IsConnOpen = false;
	}
	IsInsert = false;
	return TRUE;
}
