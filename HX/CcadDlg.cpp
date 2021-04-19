// CcadDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CcadDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <memory>
#include <string>
#include <vector>

#include "HXDlg.h"
#include  "layoutinitCad.h"


using namespace Gdiplus;
//全局变量
ULONG_PTR m_gdiplusToken;

int BadCadNum = 0;

//胶条数据的总数量，是从sendGlueData赋值
//一定要执行选择图纸再点发送，不然这个值不会赋予
int vecGlueNum = 0;
//这个数据暂时不用发完就清空，因为每一次都会把对应的值覆盖进去
WORD GlueTemp[200];//把胶条数据从函数里边提取出来变成全局的，用以发送
//发送胶条时计数
int locGlueNum = 0;
//读取PLC接收到所有图纸之后的确认
int plcRecNum = 0;
// CcadDlg 对话框
//cad按钮状态
bool CadBtnStatus = true;
CcadDlg* CcadDlg::pCaddlg = NULL;

IMPLEMENT_DYNAMIC(CcadDlg, CDialogEx)

CcadDlg::CcadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAD, pParent)
	, m_cad_check_outer_frame(FALSE)
{
}

CcadDlg::~CcadDlg()
{
}

void CcadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAD_LOC, m_CadGlueList);
	DDX_Control(pDX, IDC_BUTTON_CAD_OPEN, m_cad_btn_open);
	DDX_Control(pDX, IDC_BUTTON_CAD_DRAW, m_cad_btn_draw);
	DDX_Control(pDX, IDC_BUTTON_CAD_SEND, m_cad_btn_send);
	DDX_Control(pDX, IDC_CAD_BTN_OPMON, m_cad_btn_opmon);
	DDX_Control(pDX, IDC_CAD_BTN_OPVS, m_cad_btn_opvs);
	DDX_Control(pDX, IDC_CAD_BTN_OPDATA, m_cad_btn_opdata);
	DDX_Control(pDX, IDC_CAD_BTN_OPMOD, m_cad_btn_opmod);
	DDX_Control(pDX, IDC_CAD_BTN_OPCAD, m_cad_btn_opcad);
	DDX_Control(pDX, IDC_CAD_PIC_LOGO, m_cad_pic_logo);
	DDX_Check(pDX, IDC_CAD_check_outerframe, m_cad_check_outer_frame);
}


BEGIN_MESSAGE_MAP(CcadDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BUTTON_CAD_OPEN, &CcadDlg::OnBnClickedButtonCadOpen)
	ON_BN_CLICKED(IDC_BUTTON_CAD_DRAW, &CcadDlg::OnBnClickedButtonCadDraw)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_SIZING()

	ON_BN_CLICKED(IDC_BUTTON_CAD_SEND, &CcadDlg::OnBnClickedButtonCadSend)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CAD_BTN_OPVS, &CcadDlg::OnBnClickedCadBtnOpvs)
	ON_BN_CLICKED(IDC_CAD_BTN_OPMOD, &CcadDlg::OnBnClickedCadBtnOpmod)
	ON_BN_CLICKED(IDC_CAD_BTN_OPDATA, &CcadDlg::OnBnClickedCadBtnOpdata)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CAD_BTN_OPMON, &CcadDlg::OnBnClickedCadBtnOpmon)
	ON_BN_CLICKED(IDC_CAD_check_outerframe, &CcadDlg::OnBnClickedCadcheckouterframe)
END_MESSAGE_MAP()


// CcadDlg 消息处理程序

BOOL CcadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pCaddlg = this;
	////胶条列表
	CString strglue[] = { _T("点数"),_T("X"),_T("Y"),_T("是否喷胶") };
	for (int i = 0; i < 4; i++)
	{
		//设置表头 索引 内容 对齐方式 列宽度
		m_CadGlueList.InsertColumn(i, strglue[i], LVCFMT_LEFT, 100);
	}
	m_CadGlueList.GetScrollPos(SB_VERT); //设置垂直滚动条
	DWORD dwStyle1 = m_CadGlueList.GetExtendedStyle();
	dwStyle1 |= LVS_EX_GRIDLINES;
	m_CadGlueList.SetExtendedStyle(dwStyle1);//设置扩展风格

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_Brush.CreateSolidBrush(RGB(240, 240, 220));

	//按钮重绘
	{
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_CAD_OPEN)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_open.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_open.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_open.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_open.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_CAD_DRAW)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_draw.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_draw.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_draw.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_draw.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_BUTTON_CAD_SEND)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_send.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_send.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_send.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_send.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_CAD_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_opmon.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_CAD_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_opvs.setWordSize(200);
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式

		GetDlgItem(IDC_CAD_BTN_OPDATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_opdata.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_opdata.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_opdata.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_opdata.setWordSize(200);

		GetDlgItem(IDC_CAD_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_opcad.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_CAD_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_cad_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_cad_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_cad_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_cad_btn_opmod.setWordSize(200);
	}


	//静态文本字体改变

	f_cad_font.CreateFontW(18,      // nHeight，文字大小
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
	GetDlgItem(IDC_STATIC_CAD_DRAW)->SetFont(&f_cad_font, false);
	GetDlgItem(IDC_STATIC_CAD_CHOOSE)->SetFont(&f_cad_font, false);
	GetDlgItem(IDC_STATIC_CAD_LOC)->SetFont(&f_cad_font, false);
	GetDlgItem(IDC_STATIC_CAD_LOC)->SetFont(&f_cad_font, false); 
	GetDlgItem(IDC_CAD_check_outerframe)->SetFont(&f_cad_font, false);
	f_cad_name.CreateFontW(50,      // nHeight，文字大小
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
	GetDlgItem(IDC_CAD_STATIC_NAME)->SetFont(&f_cad_name, false);

	InitLayoutCad(m_layoutCad, this);
	//全屏幕操作
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

	m_cad_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
	m_cad_pic_logo.SetBitmap(m_cad_hBitmap_logo);


	//画图设施初始化
	GraphicInit();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CcadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_CAD_PATH)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
	}

	if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		pDC->SetBkColor(RGB(240, 240, 220));
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
	}
	return hbr;
	//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}







unsigned short CcadDlg::CRC16(unsigned char* puchMsg, unsigned short usDataLen)
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


void CcadDlg::SendData(int CommTypeIn, WORD DownAdd, DWORD DownData)
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

	//CString msg;
	////%02X为16进制显示  %d十进制 %s 字符串
	//msg.Format(_T("%02X"), SendData[3]);
	//MessageBox(msg);

	/*SendArray.RemoveAll();
	SendArray.SetSize(StrLength);*/
	//把待发送数据存入数组中

	//CString msg;
 //   //%02X为16进制显示  %d十进制 %s 字符串
 //   msg.Format(_T("%02X"), SendData[3]);
 //   MessageBox(msg);

	for (int Circle = 0; Circle < StrLength; Circle++)
		SendArray[Circle] = SendData[Circle];

	//int len1 = SendArray.GetSize();
	//CString temp('x', len1);
	//for (int i = 0; i < len1; i++)
	//{
	//	temp.SetAt(i, SendArray.GetAt(i));
	//}

	//int n = temp.GetLength();//n= 8
	//int len = WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), NULL, 0, NULL, NULL);//len = 8
	//char *m_str = new char[len + 1];
	//WideCharToMultiByte(CP_ACP, 0, temp, temp.GetLength(), m_str, len, NULL, NULL);
	//m_str[len + 1] = '\0';

	CmodbusDlg* pdlg = CmodbusDlg::pModbusdlg;
	pdlg->m_SerialPort.writeData(SendArray, 8);

	//CPublic::m_SerialPort.writeData(SendArray, 8);
}




void CcadDlg::OnBnClickedButtonCadOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;
	CFileDialog fileDlg(true, _T("dwg"), _T("*.dwg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T(" dwg files (*.dwg)|*.dwg|All Files (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)    //弹出对话框  
	{
		filename = fileDlg.GetPathName();//得到完整的文件名和目录名拓展名  
		GetDlgItem(IDC_EDIT_CAD_PATH)->SetWindowText(filename);//将路径显示  
		//CString filename = fileDlg.GetFileName();
	}
	else
		return;

	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = this->m_hWnd;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = _T("libdwg_exmaple.exe");
	ShExecInfo.lpParameters = filename;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOWNORMAL;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	if (ShExecInfo.hProcess != NULL)
	{
		//等待程序运行完毕
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		TerminateProcess(ShExecInfo.hProcess, 0);
		ShExecInfo.hProcess = NULL;

	}
	//进行路径规划
	PathGen p1;
	p1.ReadAllFiles();
	p1.RefinePaths();
	p1.GenerateActualPaths(actual_path_);

	m_CadGlueList.DeleteAllItems(); //清空所有表项

	WORD temp[200];//发送数据存储空间
	int i = 0;//把一行数据读到寄存器时用到的
	CString w[1024];
	CString str;//用于接收分割字符的临时变量
	CString decimals;

	CString strline;//定义一个变量用于接收读取的一行内容
	CStdioFile file;//定义一个CStdioFile类的对象 file
	//GlueX1 = _wtoi(GlueAdd); CString转word
	std::vector<CString> vecResult;
	std::vector<CString> strTmp;
	std::vector<CString> strDecimals;

	//清除元素，并收回内存 std::vector <CString>().swap(strTmp);
	//vecResult.clear()清除元素但不收回空间
	BOOL flag = file.Open(_T(".\\path.txt"), CFile::modeRead);//open函数需要传两个参数，前一个是文件路径，后一个是文件的打开模式
	if (flag == FALSE)
	{
		//MessageBox(_T("文件打开失败"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	for (std::vector<CString>::iterator it = vecResult.begin(); it != vecResult.end(); it++)
	{
		w[i] = vecResult[i];
		i = i + 1;
	}
	//分割字符部分//

	for (int j = 0; j < i; j++)
	{
		int curPos = 0;
		str = w[j].Tokenize(_T(" "), curPos);
		while (str.Trim() != _T(""))
		{
			//输入寄存器里的喷胶数据5个一轮 0-4为第一组，但是注意多出来的空的为00
			strTmp.push_back(str);
			str = w[j].Tokenize(_T(" "), curPos);
		}
	}
	//把容器的
	vecGlueNum = strTmp.size();
	//通过容器的容量size即当前容器里边的数据多少
	for (int j = 0; j < strTmp.size(); j++)
	{


		temp[j] = _wtoi(strTmp[j]); //将字符串转为int

		GlueTemp[j] = temp[j];
		//CString msg;
		////%02X为16进制显示  %d十进制 %s 字符串
		//msg.Format(_T("%02X"), temp[j]);
		//MessageBox(msg);
	}

	//把容器里的数据除以10正常化
	for (int j = 0; j < strTmp.size(); j++)
	{
		//(.1表示小数点后留1位，小数点后超过1位则四舍五入)
		decimals.Format(_T("%.1lf"), _ttof(strTmp[j]) / 10);
		strDecimals.push_back(decimals);
	}
	//第一列的值
	for (int j = 0, a = 0; j < (strDecimals.size() / 3); j++)
	{
		CString name;
		name.Format(_T("第%d点"), j + 1);
		m_CadGlueList.InsertItem(j, name);

		a = j * 3;

		m_CadGlueList.SetItemText(j, 1, strDecimals[a]);
		m_CadGlueList.SetItemText(j, 2, strDecimals[a + 1]);


		if (strDecimals[a + 2] == _T("0.1"))
		{
			strDecimals[a + 2] = _T("喷胶");
		}
		else if (strDecimals[a + 2] == _T("0.0"))
		{
			strDecimals[a + 2] = _T("不喷胶");
		}


		m_CadGlueList.SetItemText(j, 3, strDecimals[a + 2]);
		/*m_CadGlueList.SetItemText(j, 4, strDecimals[a + 3]);
		m_CadGlueList.SetItemText(j, 5, strTmp[a + 4]);*/

		//a = j * 5;
	}
	std::vector<CString>().swap(vecResult);
	std::vector<CString>().swap(strTmp);
	std::vector<CString>().swap(strDecimals);

}


void CcadDlg::OnBnClickedButtonCadDraw()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearPic();
	SetTimer(1, 200, NULL);
}


void CcadDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
	{
		if (!DrawLineAnimated())
		{
			KillTimer(1);
		}
		break;
	}
	//发送图纸定时器
	case 2:
	{
		SendOnce = false;
		//CString msg;
		////%02X为16进制显示  %d十进制 %s 字符串
		//msg.Format(_T("定时器2执行"));
		//MessageBox(msg);
		//判断的是上一组已发送的数据的标志，如果没有超时且上一组数据CRC校验正确的话，那么可以进行本次发送
		m_CadT1 = GetTickCount64();
		if (m_CadT2 != 0 && OverTime == false && RecMsgFlag == true)
		{
			m_CadT2 = 0;//进入之后把这个时间置为0，用以判断之后的是否断线
			//如果上一个数据发送成功，那么将BadCadNum置为0
			BadCadNum = 0;
			if (locGlueNum < vecGlueNum)
			{
				SendData(1, locGlueNum + 100, GlueTemp[locGlueNum]);
				locGlueNum++;
			}
			else
			{
				Sleep(50);
				KillTimer(2);
				SendOnce = true;
				SendData(1, 99, locGlueNum / 3);
				Sleep(100);
				SendData(1, 78, 21573);
				//发送完毕之后，可以考虑每次按下发送键的时候把这个置为0，把定位数据置为0，方便下次发送
				m_CadT2 = GetTickCount64();
				locGlueNum = 0;
				//读寄存器收完消息没有
				Sleep(100);
				SetTimer(3, 50, NULL);//实际应位30
				/*CvisionDlg* pvsdlg = CvisionDlg::pVisiondlg;
				pvsdlg->ReSetTime();*/
			}
		}
		//这里是上一组信息发送有误的情况
		else
		{
			//进入错误阶段，首先错误数累加，每个数据有两次重发机会，
			BadCadNum++;
			if (BadCadNum < 4)
			{
				//第一个数据出现错误与后边的数据出现错误是一样的处理措施
				//先减1发送前一个数据
				if (locGlueNum >= 1)
					locGlueNum = locGlueNum - 1;
				SendData(1, locGlueNum + 100, GlueTemp[locGlueNum]);
				//发送完之后做加一处理
				locGlueNum++;
			}
			else
			{

				//停止发送
				KillTimer(2);
				//报错
				CString msg;

				//%02X为16进制显示  %d十进制 %s 字符串
				msg.Format(_T("图纸数据发送错误，请检查连接并重新发送！"));
				MessageBox(msg);
				locGlueNum = 0;

			}
		}
		break;
	}
	case 3:
	{
		SendOnce = true;
		ReadStatus = false;
		SendData(0, 78, 1);//255
		plcRecNum += 1;//50 * 20 * 3 
		if (plcRecNum <= 60)
		{
			if (PlcCadRecFlag == true)
			{
				KillTimer(3);
				Sleep(50);
				MessageBox(_T("CAD图纸数据发送完毕"));
				PlcCadRecFlag = false;
				locGlueNum = 0;
				plcRecNum = 0;

				CvisionDlg* pvsdlg = CvisionDlg::pVisiondlg;
				pvsdlg->ReSetTime();


			}

		}
		else
		{
			KillTimer(3);
			Sleep(50);
			MessageBox(_T("请重新发送CAD数据!"));

			PlcCadRecFlag = false;
			plcRecNum = 0;
			locGlueNum = 0;


			CvisionDlg* pvsdlg = CvisionDlg::pVisiondlg;
			pvsdlg->ReSetTime();
		}
	}
	break;
	}


	__super::OnTimer(nIDEvent);
}


void CcadDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_layoutCad.OnSize(cx, cy);
}


void CcadDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	__super::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}





void CcadDlg::OnBnClickedButtonCadSend()
{
	// TODO: 在此添加控件通知处理程序代码

	OverTime = false;
	BadCadNum = 0;
	locGlueNum = 0;
	plcRecNum = 0;

	//发送cad数据时停掉定时器1
	CvisionDlg* pvsdlg = CvisionDlg::pVisiondlg;
	pvsdlg->KillTime1();
	//begin ASCII码BG
	Sleep(100);
	SendData(1, 76, 18242);
	Sleep(100);
	SetTimer(2, 100, NULL);



	////send();
	////settime(3,) //隔100ms判断一次 可以写了

	//CString temp;
	//CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;
	//pdlg->m_OpenCloseCtrl.GetWindowText(temp);///获取按钮的文本
	////UpdateData(true);
	//if (temp == _T("打开串口"))///表示点击后是"关闭串口"，也就是已经关闭了串口
	//{
	//	AfxMessageBox(_T("请先打开串口！"));
	//}
	//else
	//	SetTimer(2, 50, NULL);

}


BOOL CcadDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			OnBnClickedCadBtnOpmon();
		}
		if (pMsg->wParam == VK_F2)
		{
			OnBnClickedCadBtnOpvs();
		}
		if (pMsg->wParam == VK_F4)
		{
			OnBnClickedCadBtnOpdata();
		}
		if (pMsg->wParam == VK_F5)
		{
			OnBnClickedCadBtnOpmod();
		}
	}
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);  //别忘了这句 ,CXXXDlg类的基类是CDialogEx,如果是CDialog则返回CDialog
}


void CcadDlg::OnPaint()
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

void CcadDlg::OnBnClickedCadBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CcadDlg::OnBnClickedCadBtnOpvs()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowVision();
}


void CcadDlg::OnBnClickedCadBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowModbus();
}


void CcadDlg::OnBnClickedCadBtnOpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg* p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowData();
}


BOOL CcadDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return __super::OnHelpInfo(pHelpInfo);
}





void CcadDlg::BanBtnSend()
{
	// TODO: 在此处添加实现代码.
	CadBtnStatus = false;
	m_cad_btn_send.EnableWindow(FALSE);
	//设置Button Up的背景色
	m_cad_btn_send.SetUpColor(RGB(180, 180, 165));
	//设置字体颜色
	m_cad_btn_send.setWordColor(RGB(255, 250, 250));
	//刷新按钮控件
	CRect brect;
	GetDlgItem(IDC_BUTTON_CAD_SEND)->GetWindowRect(brect);
	ScreenToClient(brect);
	InvalidateRect(brect);
}



void CcadDlg::EnableBtnSend()
{
	// TODO: 在此处添加实现代码.
	CadBtnStatus = true;
	m_cad_btn_send.EnableWindow(TRUE);
	//设置Button Up的背景色
	m_cad_btn_send.SetUpColor(RGB(2, 158, 160));
	//设置字体颜色
	m_cad_btn_send.setWordColor(RGB(255, 250, 250));
	//刷新按钮控件
	CRect brect;
	GetDlgItem(IDC_BUTTON_CAD_SEND)->GetWindowRect(brect);
	ScreenToClient(brect);
	InvalidateRect(brect);
}


/////////////////////////////////////////////////画图所需要的东西

void CcadDlg::GraphicInit()
{
	pen_color_ = Color::Black;
	pen_width_ = 2.0;
	graphics_ptr_.reset(new Graphics(GetDlgItem(IDC_STATIC_CAD_PIC)->GetDC()->m_hDC));
	GetDlgItem(IDC_STATIC_CAD_PIC)->GetClientRect(&pic_rect_);
}

void CcadDlg::ClearPic()
{
	graphics_ptr_->Clear(Color::White);
}

void CcadDlg::DrawLine(const Gdiplus::ARGB& color, const float& width, const Gdiplus::Point& p1, const Gdiplus::Point& p2)
{
	Pen pen(Color(color), width);
	graphics_ptr_->DrawLine(&pen, p1, p2);
}

void CcadDlg::DrawLineOffset(const double& width_max, const double& height_max, const int& x1, const int& y1, const int& x2, const int& y2)
{
	const float scale_x = (pic_rect_.Width() * 0.8f) / width_max;
	const float scale_y = (pic_rect_.Height() * 0.8f) / height_max;

	Gdiplus::Point offset(pic_rect_.Width() / 10, pic_rect_.Height() / 10);

	Gdiplus::Point p1(x1 * scale_x, y1 * scale_y);
	Gdiplus::Point p2(x2 * scale_x, y2 * scale_y);
	DrawLine(pen_color_, pen_width_, p1 + offset, p2 + offset);
}

bool CcadDlg::DrawLineAnimated()
{
	static size_t i = 0;
	if (i < actual_path_.len)
	{
		if (i % 2)
		{
			pen_color_ = Color::Black;
		}
		else
		{
			pen_color_ = Color::Red;
		}
		int tmp_sx = 0, tmp_sy = 0, tmp_ex = 0, tmp_ey = 0;
		tmp_sx = actual_path_.real_path[i].sx - actual_path_.rsx;
		tmp_sy = abs(actual_path_.real_path[i].sy - int(actual_path_.height) - actual_path_.rsy);
		tmp_ex = actual_path_.real_path[i].ex - actual_path_.rsx;
		tmp_ey = abs(actual_path_.real_path[i].ey - int(actual_path_.height) - actual_path_.rsy);
		DrawLineOffset((double)actual_path_.width, (double)actual_path_.height, tmp_sx, tmp_sy, tmp_ex, tmp_ey);
		i++;
	}
	else
	{
		i = 0;
		return false;
	}
	return true;

}


void CcadDlg::OnBnClickedCadcheckouterframe()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CAD_check_outerframe);
	int state = pBtn->GetCheck();
	if (state == 1) // 选中
	{
		m_cad_check_outer_frame = 1;
		CADdata = PathGen::bp_info.e_width;
	}
	else // 取消选中
	{
		m_cad_check_outer_frame = 0;
		CADdata = PathGen::bp_info.i_width;
	}
}
