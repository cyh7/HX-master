// CmodbusDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CmodbusDlg.h"
#include "afxdialogex.h"
#include <vector>
#include "layoutinitModbus.h"
#include "HXDlg.h"
#include "InfoFile.h"

//波特率
int BaudRateArray[] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
//奇偶校验
std::string ParityArray[] = { "None", "Odd", "Even", "Mark", "Space" };
//数据位
std::string DataBitsArray[] = { "5", "6", "7","8" };
//停止位
std::string StopArray[] = { "1", "1.5", "2" };
//声明一个位操作的容器
vector<bool> bit_manipul(16, 0);
CmodbusDlg *CmodbusDlg::pModbusdlg = NULL;

//判断从机发送回复信息的对错 可以进入第一次发送
bool RecMsgFlag = true;
//接收超时
bool OverTime_Vision = false;
bool OverTime = false;
//T1为开关按下时刻，T2为数据接收时刻
long m_CadT1;
//给T2赋一个初值，防止首次执行时发生误判，之后的时候若T2值为0则说明通信断线，我们没有收到接收函数
long m_CadT2 = 1;
//视觉计时
long m_Vision_T1;
long m_Vision_T2 = 1;
//循环发送计时
long m_Status_T1;
long m_Status_T2 = 1;
//读状态标志位
bool ReadStatus = false;
//判断背板是否到达
bool ArriveFlag = false;
//声明一个当前是否是单次发送的flag
bool SendOnce = true;
bool SendOnce_Vision = true;
//是否是每200s询问一次，不是的话就属于发送视觉识别的程序，计算发送定位数据是否超时
int DisconnectNum = 0;
//设置界面
//X上下限 Y上下限 THETA上下限
double x_floor;
double x_ceil;
double y_floor;
double y_ceil;
double theta_floor;
double theta_ceil;
//背板型号
CString backboard;
//胶机状态 false没有停机 true停机
bool SprayFlag = false;
//喷涂批次
DWORD SprayBatch = 0;
//是否良品
bool GoodFlag = true;
//PLC正常
bool PlcFlag = true;
//通信状态
bool DisconnectFlag = true;
bool ConnectClose = true;
//急停标志位
bool StopFlag = false;
//读cad图纸是否接收完毕
bool PlcCadRecFlag = false;
//PLC请求连接测试标志位
bool PlcAskFlag = false;
//插入数据库所需变量
//防止识别完成后重复插入，识别完置0；插入完成置1
int insertdata = 0;
//良品
CString data_good;
//胶机状态
CString data_spray;
//PLC状态
CString data_plc;
//急停
CString data_stop;
//这个数据暂时不用发完就清空，因为每一次都会把对应的值覆盖进去
//WORD GlueTemp[200];//把胶条数据从函数里边提取出来变成全局的，用以发送
bool exitFlag = false;
// CmodbusDlg 对话框

IMPLEMENT_DYNAMIC(CmodbusDlg, CDialogEx)

CmodbusDlg::CmodbusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODBUS, pParent)
	, m_EditSend(_T(""))
	, m_EditReceive(_T(""))
	, m_mod_type(_T(""))
	, m_mod_edit_xfloor(0)
	, m_mod_edit_yfloor(0)
	, m_mod_edit_thetafloor(0)
	, m_mod_edit_xceil(0)
	, m_mod_edit_yceil(0)
	, m_mod_edit_thetaceil(0)
	, m_mod_edit_threshold(0)
	, m_mod_edit_filter(0)
	, m_mod_edit_rect_width(0)
	, m_mod_edit_height(0)
	, m_mod_edit_rect_topleft_x(0)
	, m_mod_edit_rect_topleft_y(0)
	, m_mod_edit_right_rect_topleft_x(0)
	, m_mod_edit_right_rect_topleft_y(0)
	, m_mod_edit_left_baoguang(0)
	, m_mod_edit_right_baoguang(0)
{

}

CmodbusDlg::~CmodbusDlg()
{
}

void CmodbusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_EditSend);
	DDX_Control(pDX, IDC_COMBO1, m_comb1);
	DDX_Control(pDX, IDC_COMBO2, m_comb2);
	DDX_Control(pDX, IDC_COMBO3, m_comb3);
	DDX_Control(pDX, IDC_COMBO4, m_comb4);
	DDX_Control(pDX, IDC_COMBO5, m_comb5);

	DDX_Text(pDX, IDC_EDIT2, m_EditReceive);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_OpenCloseCtrl);


	DDX_Control(pDX, IDC_EDIT2, m_EditReceiveCtrl);

	DDX_Control(pDX, IDC_BUTTON_SEND_ONCE, m_btn_SendOnce);
	DDX_Control(pDX, IDC_BUTTON_CLEAN, m_btn_Clean);
	DDX_Control(pDX, IDC_MOD_BTN_OPMON, m_mod_btn_opmon);
	DDX_Control(pDX, IDC_MOD_BTN_OPVS, m_mod_btn_opvs);
	DDX_Control(pDX, IDC_MOD_BTN_OPCAD, m_mod_btn_opcad);
	DDX_Control(pDX, IDC_MOD_BTN_OPDATA, m_mod_btn_opdata);
	DDX_Text(pDX, IDC_EDIT_TYPE, m_mod_type);
	DDX_Control(pDX, IDC_MOD_BTN_CHANGE, m_mod_btn_change);
	DDX_Control(pDX, IDC_MOD_BTN_OPMOD, m_mod_btn_opmod);
	DDX_Text(pDX, IDC_EDIT_XFLOOR, m_mod_edit_xfloor);
	DDX_Text(pDX, IDC_EDIT_YFLOOR, m_mod_edit_yfloor);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR, m_mod_edit_thetafloor);
	DDX_Text(pDX, IDC_EDIT_XCEILING, m_mod_edit_xceil);
	DDX_Text(pDX, IDC_EDIT_YCEILING, m_mod_edit_yceil);
	DDX_Text(pDX, IDC_EDIT_THETACEILING, m_mod_edit_thetaceil);
	DDX_Control(pDX, IDC_MOD_PIC_LOGO, m_mod_pic_logo);
	DDX_Control(pDX, IDC_BUTTON1, m_mod_btn_timesend);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR2, m_mod_edit_threshold);
	DDX_Text(pDX, IDC_EDIT_THETACEILING2, m_mod_edit_filter);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR3, m_mod_edit_rect_width);
	DDX_Text(pDX, IDC_EDIT_THETACEILING3, m_mod_edit_height);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR4, m_mod_edit_rect_topleft_x);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR5, m_mod_edit_rect_topleft_y);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR6, m_mod_edit_right_rect_topleft_x);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR7, m_mod_edit_right_rect_topleft_y);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR8, m_mod_edit_left_baoguang);
	DDX_Text(pDX, IDC_EDIT_THETAFLOOR9, m_mod_edit_right_baoguang);
	DDX_Control(pDX, IDC_MOD_BTN_EXIT, m_mod_btn_exit);
}


BEGIN_MESSAGE_MAP(CmodbusDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CmodbusDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND_ONCE, &CmodbusDlg::OnBnClickedButtonSendOnce)
	ON_BN_CLICKED(IDC_BUTTON1, &CmodbusDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, &CmodbusDlg::OnBnClickedButtonClean)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MOD_BTN_OPCAD, &CmodbusDlg::OnBnClickedModBtnOpcad)
	ON_BN_CLICKED(IDC_MOD_BTN_OPDATA, &CmodbusDlg::OnBnClickedModBtnOpdata)
	ON_BN_CLICKED(IDC_MOD_BTN_OPVS, &CmodbusDlg::OnBnClickedModBtnOpvs)
	ON_BN_CLICKED(IDC_MOD_BTN_CHANGE, &CmodbusDlg::OnBnClickedModBtnChange)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_MOD_BTN_OPMON, &CmodbusDlg::OnBnClickedModBtnOpmon)
	
	ON_BN_CLICKED(IDC_MOD_BTN_EXIT, &CmodbusDlg::OnBnClickedModBtnExit)
END_MESSAGE_MAP()


UINT ThreadRec(LPVOID param)
{
	CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;

	while(1)
	{
		pdlg->OnReceive();
	}
	AfxEndThread(0);
	return 0;
}


// CmodbusDlg 消息处理程序
//窗口初始化
BOOL CmodbusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//初始化串口指针
	pModbusdlg = this;
	// 串口选择组合框
	CString temp;
	//添加波特率到下拉列表
	for (int i = 0; i < sizeof(BaudRateArray) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRateArray[i]);
		m_comb2.InsertString(i, temp);
	}

	temp.Format(_T("%d"), 19200);
	m_comb2.SetCurSel(m_comb2.FindString(0, temp));

	//校验位
	for (int i = 0; i < sizeof(ParityArray) / sizeof(std::string); i++)
	{
#ifdef UNICODE
		temp.Format(_T("%S"), ParityArray[i].c_str());
#else
		temp.Format(_T("%s"), ParityArray[i].c_str());
#endif
		m_comb3.InsertString(i, temp);
	}
	m_comb3.SetCurSel(2);

	//数据位
	for (int i = 0; i < sizeof(DataBitsArray) / sizeof(std::string); i++)
	{
#ifdef UNICODE
		temp.Format(_T("%S"), DataBitsArray[i].c_str());
#else
		temp.Format(_T("%s"), DataBitsArray[i].c_str());
#endif
		m_comb4.InsertString(i, temp);
	}
	m_comb4.SetCurSel(3);

	//停止位
	for (int i = 0; i < sizeof(StopArray) / sizeof(std::string); i++)
	{
#ifdef UNICODE
		temp.Format(_T("%S"), StopArray[i].c_str());
#else
		temp.Format(_T("%s"), StopArray[i].c_str());
#endif
		m_comb5.InsertString(i, temp);
	}
	m_comb5.SetCurSel(0);

	//获取串口号  这个是获取当前可用的串口
	std::vector<SerialPortInfo> m_portsList = CSerialPortInfo::availablePortInfos();
	TCHAR m_regKeyValue[255];
	for (int i = 0; i < m_portsList.size(); i++)
	{
#ifdef UNICODE
		int iLength;
		const char * _char = m_portsList[i].portName.c_str();
		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, m_regKeyValue, iLength);
#else
		strcpy_s(m_regKeyValue, 255, m_portsList[i].portName.c_str());
#endif
		m_comb1.AddString(m_regKeyValue);
	}
	m_comb1.SetCurSel(0);

	
	m_Brush.CreateSolidBrush(RGB(240, 240, 220));

	//按钮重绘
	{
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_SEND_ONCE)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_btn_SendOnce.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_btn_SendOnce.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_btn_SendOnce.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_btn_SendOnce.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_OPEN)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_OpenCloseCtrl.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_OpenCloseCtrl.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_OpenCloseCtrl.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_OpenCloseCtrl.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_CLEAN)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_btn_Clean.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_btn_Clean.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_btn_Clean.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_btn_Clean.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MOD_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_opmon.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MOD_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_opvs.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MOD_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_opcad.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MOD_BTN_OPDATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_opdata.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_opdata.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_opdata.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_opdata.setWordSize(200);

		GetDlgItem(IDC_MOD_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_opmod.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_MOD_BTN_CHANGE)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_change.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_change.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_change.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_change.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON1)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_timesend.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_timesend.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_timesend.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_timesend.setWordSize(200);

		GetDlgItem(IDC_MOD_BTN_EXIT)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_mod_btn_exit.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_mod_btn_exit.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_mod_btn_exit.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_mod_btn_exit.setWordSize(200);
	}

	//静态文本字体改变
	{
		f_mod_font.CreateFontW(18,      // nHeight，文字大小
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
		GetDlgItem(IDC_STATIC4)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC5)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC6)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC7)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC8)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC9)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC14)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC15)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC16)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC17)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC18)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC19)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC20)->SetFont(&f_mod_font, false);

		GetDlgItem(IDC_STATIC21)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC22)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC23)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC24)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC25)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC26)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC27)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC28)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC29)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC30)->SetFont(&f_mod_font, false);
		//三个 group_box
		GetDlgItem(IDC_STATIC3)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC10)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC11)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC12)->SetFont(&f_mod_font, false);
		GetDlgItem(IDC_STATIC13)->SetFont(&f_mod_font, false);


		f_mod_name.CreateFontW(50,      // nHeight，文字大小
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
		GetDlgItem(IDC_MOD_STATIC_NAME)->SetFont(&f_mod_name, false);
	}
	//CFont* p_font = new CFont;
	//p_font->CreateFont(18,      // nHeight，文字大小
	

	
	InitLayoutModbus(m_layoutMod, this);
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
		rectFullScreen.bottom = rectWholeDlg.bottom + g_iCurScreenHeight - rectClient.bottom - 20;

		//enter into full screen;
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = rectFullScreen;
		SetWindowPlacement(&struWndpl);

		m_mod_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
		m_mod_pic_logo.SetBitmap(m_mod_hBitmap_logo);
	}
	
	//读取设置
	{
		CInfoFile file;
		file.ReadDocline(backboard, x_floor, x_ceil, y_floor, y_ceil, theta_floor, theta_ceil, hv_Threshold_8,
			hv_Filter_block_radius_8, rect_height, rect_width, m_startPos_left_8_x,
			m_startPos_left_8_y, m_startPos_right_8_x, m_startPos_right_8_y, left_baoguang_time, right_baoguang_time);
		m_mod_type = backboard;
		m_mod_edit_xfloor = x_floor;
		m_mod_edit_xceil = x_ceil;
		m_mod_edit_yfloor = y_floor;
		m_mod_edit_yceil = y_ceil;
		m_mod_edit_thetafloor = theta_floor;
		m_mod_edit_thetaceil = theta_ceil;

		m_mod_edit_threshold = hv_Threshold_8;
		m_mod_edit_filter = hv_Filter_block_radius_8;
		//存盘的是压缩后的裁剪框位置和尺寸,编辑框显示中的是实际的裁剪框位置和尺寸,所以编辑框显示时要乘以压缩比
		m_mod_edit_height = rect_height * scale;
		m_mod_edit_rect_width = rect_width * scale;
		m_mod_edit_rect_topleft_x = m_startPos_left_8_x * scale;
		m_mod_edit_rect_topleft_y = m_startPos_left_8_y * scale;
		m_mod_edit_right_rect_topleft_x = m_startPos_right_8_x * scale;
		m_mod_edit_right_rect_topleft_y = m_startPos_right_8_y * scale;

		m_mod_edit_left_baoguang = left_baoguang_time;
		m_mod_edit_right_baoguang = right_baoguang_time;
		UpdateData(FALSE);
	}
	

	HANDLE hthreadREC = AfxBeginThread(ThreadRec, this, THREAD_PRIORITY_BELOW_NORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//设置背景颜色
HBRUSH CmodbusDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	

	//((CStatic*)GetDlgItem(CTLCOLOR_STATIC))->SetFont(p_font, FALSE);
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
	//两个编辑框透明
	if (pWnd->GetDlgCtrlID() == IDC_EDIT2 || IDC_EDIT1 || IDC_EDIT_TYPE)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔


	return hbr;
	//return (HBRUSH)m_Brush.GetSafeHandle();
	//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}
//打开串口
void CmodbusDlg::OnBnClickedButtonOpen()
{
	CString temp;
	m_OpenCloseCtrl.GetWindowText(temp);///获取按钮的文本
	UpdateData(true);
	if (temp == _T("关闭串口"))///表示点击后是"关闭串口"，也就是已经关闭了串口
	{
		m_SerialPort.close();
		m_OpenCloseCtrl.SetWindowText(_T("打开串口"));///设置按钮文字为"打开串口"
	}
	///打开串口操作
	else if (m_comb1.GetCount() > 0)///当前列表的内容个数
	{
		string portName;
		int SelBaudRate;
		int SelParity;
		int SelDataBits;
		int SelStop;

		UpdateData(true);
		m_comb1.GetWindowText(temp);///CString temp
#ifdef UNICODE
		portName = CW2A(temp.GetString());
#else
		portName = temp.GetBuffer();
#endif	
		//CPublic::Port = portName;
		

		m_comb2.GetWindowText(temp);
		SelBaudRate = _tstoi(temp);
		//CPublic::BaudRate = SelBaudRate;

		SelParity = m_comb3.GetCurSel();
		//CPublic::Parity = SelParity;

		m_comb4.GetWindowText(temp);
		SelDataBits = _tstoi(temp);
		//CPublic::DataBits = SelDataBits;

		SelStop = m_comb5.GetCurSel();
		//CPublic::Stop = SelStop;


		m_SerialPort.init(portName, SelBaudRate, itas109::Parity(SelParity), itas109::DataBits(SelDataBits), itas109::StopBits(SelStop));
		m_SerialPort.open();
		m_SerialPort.setMinByteReadNotify(1);

		if (m_SerialPort.isOpened())
		{
			m_OpenCloseCtrl.SetWindowText(_T("关闭串口"));
		}
		else
		{
			AfxMessageBox(_T("串口已被占用！"));
		}
	}
	else
	{
		AfxMessageBox(_T("没有发现串口！"));
	}
}

//串口单次发送
void CmodbusDlg::OnBnClickedButtonSendOnce()
{
	// TODO: 在此添加控件通知处理程序代码

	SendOnce = true;

	CByteArray HexDataBuf;
	char sendData[8] = { 0 };
	int i = 0;
	BYTE SendBuf[128] = { 0 };
	BYTE GetData[256] = { 0 };
	int SendLen = 0;
	int GetLen = 0;
	unsigned short CRCData;

	UpdateData(TRUE);
	if (m_EditSend.IsEmpty())
	{
		AfxMessageBox(_T("发送数据为空!"));
		return;
	}
	HexDataBuf.RemoveAll(); //清空数组
	GetLen = m_EditSend.GetLength();
	for (i = 0; i < GetLen; i++)
	{
		GetData[i] = (BYTE)m_EditSend.GetBuffer()[i];
	}
	StringtoHex(GetData, GetLen, SendBuf, &SendLen); //将字符串转化为字节数据

	CRCData = CRC16(SendBuf, SendLen);
	SendBuf[6] = CRCData >> 8;
	SendBuf[7] = CRCData - 256 * SendBuf[6];

	HexDataBuf.SetSize(8); //设置数组大小为帧长度
	for (i = 0; i < 8; i++)
	{
		HexDataBuf.SetAt(i, SendBuf[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		sendData[i] = HexDataBuf[i];
	}
	

	m_SerialPort.writeData(sendData, sizeof(sendData));

	//发送一次清空编辑框
	m_EditSend = _T("");	//给接收编辑框发送空格符
	UpdateData(false);		//更新数据

}
//16位CRC校验
unsigned short CmodbusDlg::CRC16(unsigned char* puchMsg, unsigned short usDataLen)
{
	// TODO: 在此处添加实现代码.
	static unsigned char auchCRCHi[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
		0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
		0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	};

	static unsigned char auchCRCLo[] =
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
		0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
		0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
		0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
		0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
		0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
		0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
		0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
		0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
		0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
		0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
		0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
		0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
		0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
		0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
		0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
		0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
		0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
		0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
		0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
		0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
		0x43, 0x83, 0x41, 0x81, 0x80, 0x40
	};

	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned char uIndex;
	while (usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}
//16进制转换
char CmodbusDlg::HexChar(char c)
{
	// TODO: 在此处添加实现代码.
	if ((c >= '0') && (c <= '9'))
		return c - '0';//16进制中的，字符0-9转化成10进制，还是0-9
	else if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;//16进制中的A-F，分别对应着11-16
	else if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;//16进制中的a-f，分别对应也是11-16，不区分大小写
	else
		return 0x20; // 其他返回0x10
}
void CmodbusDlg::StringtoHex(BYTE * GB, int glen, BYTE* SB, int* slen)
{
	// TODO: 在此处添加实现代码.
	int i; //遍历输入的字符串
	int a = 0;
	char temp; //接收字符，用来判断是否为空格，若是则跳过
	char temp1, temp2; //接收一个字节的两个字符 例如EB，则temp1='E',temp2 = 'B'
	*slen = 0; //输出的16进制字符串长度
	for (i = 0; i < glen; i++)
	{
		temp = GB[i];
		if (temp == ' ')
			continue;

		if (a == 0)
			temp1 = GB[i];
		if (a == 1)
			temp2 = GB[i];
		a++;

		if (a == 2)
		{
			a = 0;
			temp1 = HexChar(temp1);
			//temp1 = temp1 - '0';
			//if (temp1>10)
		   // temp1 = temp1 -7;
			temp2 = HexChar(temp2);
			//temp2 = temp2 - '0';
			//if (temp2>10)
			//temp2 = temp2 -7;

			SB[*slen] = temp1 * 16 + temp2;
			(*slen)++;
		}
	}
}
//数据接收
void CmodbusDlg::OnReceive()
{
	// TODO: 在此处添加实现代码.
	unsigned char SendFreqData[50];
	unsigned char RecCrcData[50];
	DWORD MidData;
	CString RecStr;
	CString strtemp;
	


	//每次传进来的str都是新的 第二次的str是直接把所有的数据读到一块儿了
	char * str = NULL;
	try {
		str = new char[1024];
	}
	catch (...)
	{

	}
	//std::shared_ptr<char> str(new char[1024]);
	if (exitFlag == true)
	{
		
		if (str != NULL)
			delete[]str;
		return;
	}

	int iRet = m_SerialPort.readAllData(str); //06发过来的数据长度为8 接收到的数据是没错的

	
	if (iRet > 0)
	{
		//设置单次发送不计时T2,非单次发送计时T2
		if (SendOnce == false)
		{
			//MessageBox(_T("计时"));
			m_CadT2 = GetTickCount64();
			if ((m_CadT2 - m_CadT1) > 300)
				OverTime = true;
		}
		/*if ((m_CadT2 - m_CadT1) > 50)
			OverTime = true;*/

		if (SendOnce_Vision == false)
		{
			m_Vision_T2 = GetTickCount64();
			if ((m_Vision_T2 - m_Vision_T1) > 200)
				OverTime_Vision = true;
		}
		/*if ((m_Vision_T2 - m_Vision_T1) > 50)
			OverTime_Vision = true;*/
		m_Status_T2 = GetTickCount64();
		//MessageBox(_T("1"));
		if (iRet == 7)
		{
			//计算收到的数据的CRC值
			SendFreqData[0] = str[0];
			SendFreqData[1] = str[1];
			SendFreqData[2] = str[2];
			SendFreqData[3] = str[3];
			SendFreqData[4] = str[4];
			MidData = CRC16(SendFreqData, 5);
			SendFreqData[5] = MidData >> 8;
			SendFreqData[6] = MidData - 256 * SendFreqData[4];
			//这部分是用来显示在接受栏中 确保显示出来的接收数据的原生性
			RecCrcData[0] = str[0];
			RecCrcData[1] = str[1];
			RecCrcData[2] = str[2];
			RecCrcData[3] = str[3];
			RecCrcData[4] = str[4];
			RecCrcData[5] = str[5];
			RecCrcData[6] = str[6];
			if (SendFreqData[5] == RecCrcData[5] && SendFreqData[6] == RecCrcData[6])
			{
				RecMsgFlag = true;
			    //MessageBox(_T("相等"));
				if (SendFreqData[3] == 0 && ReadStatus == false)
				{
					MidData = SendFreqData[4];
					char MyChar[10];
					_itoa_s(MidData, MyChar, 10);
					//测试成功 RecStr值为1，长度为1可以进行后续判断了
					RecStr = MyChar;
					if (RecStr == "0")
					{
						PlcCadRecFlag = true;
					}
				}
				//如果读的是数据的话，就会进入这个判断
				if(ReadStatus == true)
				{
					int temp = 0;
					//读到的10进制数据
					temp = SendFreqData[3] * 256 + SendFreqData[4];
					//位操作
					BitManipul(temp);
					//背板到位
					if (bit_manipul[0] == true)
						ArriveFlag = true;
					else
					{
						ArriveFlag = false;
						//背板不在（离开）要把这个置为false，方便下一次进入程序
						SendDone = false;
					}

					//胶机状态位
					if (bit_manipul[1] == true)
						SprayFlag = true;
					else
						SprayFlag = false;
					//PLC状态
					if (bit_manipul[2] == true)
						PlcFlag = true;
					else
						PlcFlag = false;
					//急停
					if (bit_manipul[3] == true)
						StopFlag = true;
					else
						StopFlag = false;
					if (bit_manipul[4] == true)
					{
						if (CadBtnStatus == true)
						{
							CcadDlg* pcaddlg = CcadDlg::pCaddlg;
							pcaddlg->BanBtnSend();
						}
					}
					else
					{
						if (CadBtnStatus == false)
						{
							CcadDlg* pcaddlg = CcadDlg::pCaddlg;
							pcaddlg->EnableBtnSend();
						}
						
					}
					//请求连接尝试标志位
					if (bit_manipul[15] == true)
					{
						PlcAskFlag = true;
						//ASCII码对应CR
						SendData(1, 75, 21059);
						Sleep(50);
					}
					else
						PlcAskFlag = false;
					bit_manipul = vector<bool>(16, 0);
				}
			   
			}
			else
			{
				RecMsgFlag = false;
			}
		}
		else if (iRet == 8)
		{
			SendFreqData[0] = str[0];
			SendFreqData[1] = str[1];
			SendFreqData[2] = str[2];
			SendFreqData[3] = str[3];
			SendFreqData[4] = str[4];
			SendFreqData[5] = str[5];
			MidData = CRC16(SendFreqData, 6);
			SendFreqData[6] = MidData >> 8;
			SendFreqData[7] = MidData - 256 * SendFreqData[5];

			//显示在接受栏中
			RecCrcData[0] = str[0];
			RecCrcData[1] = str[1];
			RecCrcData[2] = str[2];
			RecCrcData[3] = str[3];
			RecCrcData[4] = str[4];
			RecCrcData[5] = str[5];
			RecCrcData[6] = str[6];
			RecCrcData[7] = str[7];
			//对比发送再收回的CRC校验
			if (SendFreqData[6] == RecCrcData[6] && SendFreqData[7] == RecCrcData[7])
			{
				RecMsgFlag = true;
			}
			else
			{
				RecMsgFlag = false;

			}
		}
		else if (iRet == 9)
		{
			SendFreqData[0] = str[0];
			SendFreqData[1] = str[1];
			SendFreqData[2] = str[2];
			SendFreqData[3] = str[3];
			SendFreqData[4] = str[4];
			SendFreqData[5] = str[5];
			SendFreqData[6] = str[6];
			MidData = CRC16(SendFreqData, 7);
			SendFreqData[7] = MidData >> 8;
			SendFreqData[8] = MidData - 256 * SendFreqData[6];
			//显示在接受栏中
			RecCrcData[0] = str[0];
			RecCrcData[1] = str[1];
			RecCrcData[2] = str[2];
			RecCrcData[3] = str[3];
			RecCrcData[4] = str[4];
			RecCrcData[5] = str[5];
			RecCrcData[6] = str[6];
			RecCrcData[7] = str[7];
			RecCrcData[8] = str[8];
			//对比发送再收回的CRC校验
			if (SendFreqData[7] == RecCrcData[7] && SendFreqData[8] == RecCrcData[8])
			{
				RecMsgFlag = true;
			}
			else
			{
				RecMsgFlag = false;
			}
		}


		
		if (SendDone == true && insertdata == 0)
		{
			CTime curTime;//当前时间
			curTime = CTime::GetCurrentTime();
			CString testLastTime = curTime.Format("%Y-%m-%d %H:%M:%S");
			JudgeStatus();
			CdataDlg* pdatadlg = CdataDlg::pDatadlg;
			pdatadlg->InsertDB(testLastTime, backboard, SprayBatch, vs_x, vs_y,vs_x_right,vs_y_right, vs_theta, data_good, data_plc, data_spray, data_stop);
			insertdata = 1;
		}
		CString RecStr;
		for (int k = 0; k < iRet; k++)
		{
			strtemp.Format(_T("%02X "), RecCrcData[k]);

			RecStr += strtemp;
		}
		RecStr += "\r\n";
		m_EditReceiveCtrl.ReplaceSel(RecStr);

		m_EditReceiveCtrl.SetSel(-1, -1); 
		
	}
	delete []str;
}

//数据发送函数
//CommTypeIn：0是读寄存器，1是写寄存器
//发送边框尺寸数据即 SendData(1,10,10000)
//最高位做判断位 0-32767为正 32768-65535为负
void CmodbusDlg::SendData(int CommTypeIn, WORD DownAdd, DWORD DownData)
{
	// TODO: 在此处添加实现代码.
	unsigned char SendData[200];
	unsigned short CRCData;
	//CByteArray SendArray;
	char SendArray[8];
	DWORD MidData;
	BYTE StrLength;

	SendData[0] = 01;
	if (CommTypeIn == 0)
		SendData[1] = 3;
	else
		SendData[1] = 6;
	MidData = DownAdd;
	SendData[2] = (unsigned char)(MidData >> 8);
	SendData[3] = (unsigned char)(MidData - 256 * SendData[2]);
	MidData = DownData;
	SendData[4] = (unsigned char)(MidData >> 8);
	SendData[5] = (unsigned char)(MidData - 256 * SendData[4]);
	//CRC校验
	CRCData = CRC16(SendData, 6);
	SendData[6] = CRCData >> 8;
	SendData[7] = CRCData - 256 * SendData[6];
	StrLength = 8;

	for (int Circle = 0; Circle < StrLength; Circle++)
		SendArray[Circle] = SendData[Circle];
	m_SerialPort.writeData(SendArray, 8);
	//m_SerialPort.writeData(m_str, len);
}

//定时发送
void CmodbusDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	m_mod_btn_timesend.GetWindowText(temp);///获取按钮的文本
	UpdateData(true);
	if (temp == _T("关闭通讯"))///表示点击后是"关闭串口"，也就是已经关闭了串口
	{
		CvisionDlg *pvsdlg = CvisionDlg::pVisiondlg;
		pvsdlg->KillTime1(); 
		ConnectClose = true;
		m_mod_btn_timesend.SetWindowText(_T("开启通讯"));
	}
	if (temp == _T("开启通讯"))///表示点击后是"关闭串口"，也就是已经关闭了串口
	{
		CvisionDlg *pvsdlg = CvisionDlg::pVisiondlg;
		pvsdlg->ReSetTime();
		ConnectClose = false;
		m_mod_btn_timesend.SetWindowText(_T("关闭通讯"));
	}
	
	
}


void CmodbusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{

		case 1:
		{
			break;
		}

	}
	__super::OnTimer(nIDEvent);
}





void CmodbusDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_layoutMod.OnSize(cx, cy);
	
}


void CmodbusDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	__super::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}





void CmodbusDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	m_SerialPort.close();
	__super::OnClose();
}


void CmodbusDlg::OnBnClickedButtonClean()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditReceive = _T("");
	UpdateData(FALSE);
}


void CmodbusDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);
	
	dc.FillSolidRect(rect, RGB(240, 240, 220));
	//dc.FillSolidRect(rect, RGB(125, 125, 255));

	CDialogEx::OnPaint();
					   // 不为绘图消息调用 __super::OnPaint()
}


BOOL CmodbusDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	
	if (pMsg->wParam == VK_F3)
	{
		OnBnClickedModBtnOpcad();
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			OnBnClickedModBtnOpmon();
		}
		if (pMsg->wParam == VK_F2)
		{
			OnBnClickedModBtnOpvs();
		}
		if (pMsg->wParam == VK_F4)
		{
			OnBnClickedModBtnOpdata();
			return true;
		}
	}
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CmodbusDlg::OnBnClickedModBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CmodbusDlg::OnBnClickedModBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CmodbusDlg::OnBnClickedModBtnOpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowData();
}


void CmodbusDlg::OnBnClickedModBtnOpvs()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowVision();
}


void CmodbusDlg::OnBnClickedModBtnChange()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CInfoFile file;

	backboard = m_mod_type;
	SprayBatch = 0;
	x_floor = m_mod_edit_xfloor;
	x_ceil = m_mod_edit_xceil;
	y_floor = m_mod_edit_yfloor;
	y_ceil = m_mod_edit_yceil;
	theta_floor = m_mod_edit_thetafloor;
    theta_ceil = m_mod_edit_thetaceil;
	if (hv_Threshold_8 != m_mod_edit_threshold)
	{
		if (hv_Threshold_8 > 255)
			hv_Threshold_8 = 254;
		if (hv_Threshold_8 < 50)
			hv_Threshold_8 = 50;
		hv_Threshold_8 = m_mod_edit_threshold;
	}

	hv_Filter_block_radius_8 = m_mod_edit_filter;
	//编辑框显示中的是实际的裁剪框位置和尺寸,存盘的是压缩后的裁剪框位置和尺寸,所以存盘时要除以压缩比
	rect_height = m_mod_edit_height / scale;
	rect_width = m_mod_edit_rect_width / scale;
	m_startPos_left_8_x = m_mod_edit_rect_topleft_x / scale;
	m_startPos_left_8_y = m_mod_edit_rect_topleft_y / scale;
	m_startPos_right_8_x = m_mod_edit_right_rect_topleft_x / scale;
	m_startPos_right_8_y = m_mod_edit_right_rect_topleft_y /scale;
	if (left_baoguang_time != m_mod_edit_left_baoguang)
	{
		if (m_mod_edit_left_baoguang > 1000000)
			m_mod_edit_left_baoguang = 1000000;
		if (m_mod_edit_left_baoguang < 60000)
			m_mod_edit_left_baoguang = 60000;

		left_baoguang_time = m_mod_edit_left_baoguang;
		leftCam->ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(left_baoguang_time);
	}
	if (right_baoguang_time != m_mod_edit_right_baoguang)
	{
		if (m_mod_edit_right_baoguang > 1000000)
			m_mod_edit_right_baoguang = 1000000;
		if (m_mod_edit_right_baoguang < 60000)
			m_mod_edit_right_baoguang = 60000;
		right_baoguang_time = m_mod_edit_right_baoguang;
		rightCam->ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(right_baoguang_time);
	}


	file.WirteDocline(backboard, x_floor, x_ceil, y_floor, y_ceil, theta_floor, theta_ceil,
		hv_Threshold_8, hv_Filter_block_radius_8, rect_height, rect_width, m_startPos_left_8_x,
		m_startPos_left_8_y, m_startPos_right_8_x, m_startPos_right_8_y,left_baoguang_time,right_baoguang_time);
}


BOOL CmodbusDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return __super::OnHelpInfo(pHelpInfo);
}





void CmodbusDlg::JudgeStatus()
{
	// TODO: 在此处添加实现代码.
	if ((!flag_left_locate_error) && (!flag_right_locate_error))
	{
		data_good = _T("成功");
	}
	else
	{
		data_good = _T("失败");
	}
	if (SprayFlag == false)
		data_spray = _T("正常");
	if (SprayFlag == true)
		data_spray = _T("停机");
	//PLC
	if (PlcFlag == true)
		data_plc = _T("正常");
	if (PlcFlag == false)
		data_plc = _T("停机");
	//急停
	if (StopFlag == true)
		data_stop = _T("急停");
	if (StopFlag == false)
		data_stop = _T("没有急停");
}


// 位操作
void CmodbusDlg::BitManipul(int temp)
{
	// TODO: 在此处添加实现代码.
	int n, i, j = 0;
	i = temp;
	while (i)
	{
		bit_manipul[j] = i % 2;
		i /= 2;
		j++;
	}
	//BitManu[14] 就是最高位
}







void CmodbusDlg::OnBnClickedModBtnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	exitFlag = true;
	Sleep(1500);
	OnClose();
	Sleep(200);
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
