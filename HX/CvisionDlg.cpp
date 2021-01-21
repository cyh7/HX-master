#include "pch.h"
#include "HX.h"
#include "CvisionDlg.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include "HXDlg.h"
#include "layoutinitVision.h"
#include <cstdio>

#include "CamCon.h"//相机控制，必须第一引入，避免前面出现 using namespace cv;
#include "opencv_include.h"
#include "HalconCpp.h"
using namespace HalconCpp;
#define DATA_FOLDER "E://HX-master/HX/"
#define DATA_FOLDERs "E:/HX-master/HX/"

HTuple Num_lines = 0;
int flag_sec_big = 1;
int flag_sec_small = 2;
int num_left;
int num_right;

//左右相机智能指针
shared_ptr<SingleCam> leftCam;
shared_ptr<SingleCam> rightCam;

/*以下参数用于打开相机OnOpenCam函数*/
//定义相机句柄
HTuple hv_AcqHandle_8_left, hv_AcqHandle_8_right;
//定义左相机曝光时间,60000-1000000
int baoguang_time_8_left = 320000;
//定义右相机曝光时间,60000-1000000
int baoguang_time_8_right = 320000;
/*以上参数用于打开相机OnOpenCam函数*/

//定义左右相机定位完成标志,flag_locate_over==2时表示双相机定位完成
byte flag_locate_over = 0;
//以下参数用于列表中的定位数据显示
CString Column_Left;
CString Row_Left;
CString Column_Right;
CString Row_Right;
CString Angle;

//定义左右相机的手眼标定转换矩阵
cv::Mat cam_matrix_left, r_left, t_left;
cv::Mat cam_matrix_right, r_right, t_right;
string filename;
//定义左右相机像素坐标系的位置矩阵
cv::Mat left_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
cv::Mat right_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
//定义左右相机世界坐标系的位置矩阵
cv::Mat left_robot_coordinate;
cv::Mat right_robot_coordinate;
//定义矩阵转换的临时矩阵
cv::Mat t1, t2, t3, t4;
//定义矩阵转换系数
double s_left, s_right;
//定义最终定位数据
double ROW_LEFT_8;
double COL_LEFT_8;
double ROW_RIGHT_8;
double COL_RIGHT_8;
double ANGLE;


/*以下参数用于采集，压缩，裁剪所用全局变量*/
CBitmap bitmap_8_left;  // CBitmap对象，用于加载位图   
HBITMAP hBmp_8_left;    // 保存CBitmap加载的位图的句柄

CBitmap bitmap_8_right;  // CBitmap对象，用于加载位图   
HBITMAP hBmp_8_right;    // 保存CBitmap加载的位图的句柄

HTuple hv_Width = 5496;
HTuple hv_Height = 3672;
//定义图片显示的压缩比例
int scale = 9;
//左相机原图中的裁剪框坐标，用于图片裁剪
HTuple hv_Row1_left_8;
HTuple hv_Column1_left_8;
HTuple hv_Row2_left_8;
HTuple hv_Column2_left_8;
//左相机压缩图中的裁剪框坐标，用于裁剪框显示
int m_startPos_left_8_y = 0;
int m_startPos_left_8_x = 0;
int m_endPos_left_8_y = 250;
int m_endPos_left_8_x = 250;

HObject ho_image_left_8;
HObject ho_ImageZoom_left_8;
HObject ho_ImagePart_left_8;

//右相机压缩图中的裁剪框坐标，用于裁剪框显示
HTuple hv_Row1_right_8;
HTuple hv_Column1_right_8;
HTuple hv_Row2_right_8;
HTuple hv_Column2_right_8;
//右相机压缩图中的裁剪框坐标，用于裁剪框显示
int m_startPos_right_8_y = 100;
int m_startPos_right_8_x = 100;
int m_endPos_right_8_y = 360;
int m_endPos_right_8_x = 480;

HObject ho_image_right_8;
HObject ho_ImageZoom_right_8;
HObject ho_ImagePart_right_8;
/*以上参数用于采集，压缩，裁剪所用全局变量*/

//定义左相机检测出来水平直线和垂直直线的端点坐标,用于显示定位效果
int width_RowBegin_left_8 = 0;
int width_ColBegin_left_8 = 0;
int width_RowEnd_left_8 = 0;
int width_ColEnd_left_8 = 0;

int height_RowBegin_left_8 = 0;
int height_ColBegin_left_8 = 0;
int height_RowEnd_left_8 = 0;
int height_ColEnd_left_8 = 0;

//创建一个画笔工具,用来给左相机画裁剪框和定位直线


//定义显示用的裁剪框左上角点和右下角点
POINT m_startPos_left_8 = { 0,0 };
POINT m_endPos_left_8 = { 0,0 };
//定义左相机水平直线起点
POINT m_width_left_8 = { 0,0 };
//定义左相机垂直直线起点
POINT m_height_left_8 = { 0,0 };

//定义左相机中值滤波图片
HObject ho_ImagePart_median_left_8;
//定义左相机临时图片
HObject ho_temporary_left_8;
//定义左相机阈值区域
HObject ho_threshold_left_8;
//定义左相机最大区域
HObject ho_max_left_8;
//定义左相机凸包区域
HObject ho_convex_left_8;
//定义左相机亚像素轮廓XLD
HObject ho_xld_left_8;
//定义左相机水平亚像素轮廓XLD
HObject ho_xld_width_left_8;
//定义左相机垂直亚像素轮廓XLD
HObject ho_xld_height_left_8;
//定义左相机裁剪图片宽度
HTuple hv_ImagePart_Width_left_8;
//定义左相机裁剪图片高度
HTuple hv_ImagePart_Height_left_8;
//定义左相机中值滤波块大小
int hv_Filter_block_radius_left_8 = 3;


//定义左相机阈值
int hv_Threshold_left_8 = 100;


//定义左相机检测的水平直线最小长度
int hv_min_length_width_line_left_8 = 200;
//定义左相机检测的垂直直线最小长度
int hv_min_length_height_line_left_8 = 200;
//定义左相机检测的水平直线起点横坐标
HTuple hv_width_line_RowBegin_left_8;
//定义左相机检测的水平直线起点纵坐标
HTuple hv_width_line_ColBegin_left_8;
//定义左相机检测的水平直线终点横坐标
HTuple hv_width_line_RowEnd_left_8;
//定义左相机检测的水平直线终点纵坐标
HTuple hv_width_line_ColEnd_left_8;
//定义左相机检测的直线的其他参数
HTuple hv_width_line_other_data1_left_8;
//定义左相机检测的直线的其他参数
HTuple hv_width_line_other_data2_left_8;
//定义左相机检测的直线的其他参数
HTuple hv_width_line_other_data3_left_8;
//定义左相机找到的水平直线条数
HTuple hv_width_line_Num_left_8;
//定义检测到的直线的延长线长度
HTuple hv_line_length_8;
//定义找到的水平直线在像素坐标系的角度
HTuple hv_width_line_phi_left_8;
//定义左相机检测的垂直直线起点横坐标
HTuple hv_height_line_RowBegin_left_8;
//定义左相机检测的垂直直线起点纵坐标
HTuple hv_height_line_ColBegin_left_8;
//定义左相机检测的垂直直线终点横坐标
HTuple hv_height_line_RowEnd_left_8;
//定义左相机检测的垂直直线终点纵坐标
HTuple hv_height_line_ColEnd_left_8;
//定义左相机找到的垂直直线条数
HTuple hv_height_line_Num_left_8;
//定义左相机找到的垂直直线在像素坐标系的角度
HTuple hv_height_line_phi_left_8;
//定义左相机找到的角点行坐标
HTuple hv_ROW_left_8;
//定义左相机找到的角点列坐标
HTuple hv_COL_left_8;
//定义左相机角点检测标志
HTuple hv_IsOverlapping_left_8;

//定义用于选择直线的临时变量
int a1, b1, c1, d1;
//定义右相机检测出来水平直线和垂直直线的端点坐标,用于显示定位效果
int width_RowBegin_right_8 = 0;
int width_ColBegin_right_8 = 0;
int width_RowEnd_right_8 = 0;
int width_ColEnd_right_8 = 0;

int height_RowBegin_right_8 = 0;
int height_ColBegin_right_8 = 0;
int height_RowEnd_right_8 = 0;
int height_ColEnd_right_8 = 0;



//定义显示用的裁剪框左上角点和右下角点
POINT m_startPos_right_8 = { 0,0 };
POINT m_endPos_right_8 = { 0,0 };
//定义右相机水平直线起点
POINT m_width_right_8 = { 0,0 };
//定义右相机垂直直线起点
POINT m_height_right_8 = { 0,0 };

//定义右相机中值滤波图片
HObject ho_ImagePart_median_right_8;
//定义右相机临时图片
HObject ho_temporary_right_8;
//定义右相机阈值区域
HObject ho_threshold_right_8;
//定义右相机最大区域
HObject ho_max_right_8;
//定义右相机凸包区域
HObject ho_convex_right_8;
//定义右相机亚像素轮廓XLD
HObject ho_xld_right_8;
//定义右相机水平亚像素轮廓XLD
HObject ho_xld_width_right_8;
//定义右相机垂直亚像素轮廓XLD
HObject ho_xld_height_right_8;
//定义右相机裁剪图片宽度
HTuple hv_ImagePart_Width_right_8;
//定义右相机裁剪图片高度
HTuple hv_ImagePart_Height_right_8;

//定义测试次数和出错次数
int test_times=0;
int wrong_times=0;

//定义右相机中值滤波块大小
int hv_Filter_block_radius_right_8 = 3;
//定义右相机阈值
int hv_Threshold_right_8 = 100;


//定义右相机检测的水平直线最小长度
int hv_min_length_width_line_right_8 = 200;
//定义右相机检测的垂直直线最小长度
int hv_min_length_height_line_right_8 = 200;
//定义右相机检测的水平直线起点横坐标
HTuple hv_width_line_RowBegin_right_8;
//定义右相机检测的水平直线起点纵坐标
HTuple hv_width_line_ColBegin_right_8;
//定义右相机检测的水平直线终点横坐标
HTuple hv_width_line_RowEnd_right_8;
//定义右相机检测的水平直线终点纵坐标
HTuple hv_width_line_ColEnd_right_8;
//定义右相机检测的直线的其他参数
HTuple hv_width_line_other_data1_right_8;
//定义右相机检测的直线的其他参数
HTuple hv_width_line_other_data2_right_8;
//定义右相机检测的直线的其他参数
HTuple hv_width_line_other_data3_right_8;
//定义右相机找到的水平直线条数
HTuple hv_width_line_Num_right_8;

//定义找到的水平直线在像素坐标系的角度
HTuple hv_width_line_phi_right_8;
//定义右相机检测的垂直直线起点横坐标
HTuple hv_height_line_RowBegin_right_8;
//定义右相机检测的垂直直线起点纵坐标
HTuple hv_height_line_ColBegin_right_8;
//定义右相机检测的垂直直线终点横坐标
HTuple hv_height_line_RowEnd_right_8;
//定义右相机检测的垂直直线终点纵坐标
HTuple hv_height_line_ColEnd_right_8;
//定义右相机找到的垂直直线条数
HTuple hv_height_line_Num_right_8;
//定义右相机找到的垂直直线在像素坐标系的角度
HTuple hv_height_line_phi_right_8;
//定义右相机找到的角点行坐标
HTuple hv_ROW_right_8;
//定义右相机找到的角点列坐标
HTuple hv_COL_right_8;
//定义右相机角点检测标志
HTuple hv_IsOverlapping_right_8;


//发送错误次数
int BadVisionNum = 0;
//当前发送的是第几个
int LocVisionNum = 0;
//判断是否识别完成
bool IdentifyDone = false;

//上一次时间
CString LastTime;
//插入数据库时的字符串

//double x y theta
//X Y Theta
double vs_x;
double vs_y;
double vs_theta;
CvisionDlg * CvisionDlg::pVisiondlg = NULL;


// CvisionDlg 对话框

IMPLEMENT_DYNAMIC(CvisionDlg, CDialogEx)

CvisionDlg::CvisionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VISION, pParent), pen_left_8(PS_SOLID, 2, RGB(255, 0, 0))
	, m_vs_edit_type(_T(""))
	, m_vs_edit_batch(0)
	, m_test_locate_num(0)
	, m_locate_test_wrong(0)
{
	
}

CvisionDlg::~CvisionDlg()
{
	
	DeleteObject(hBmp_8_left);
	DeleteObject(bitmap_8_left);
	DeleteObject(hBmp_8_right);
	DeleteObject(bitmap_8_right);
	//DeleteObject(ho_image_left_8);
	//DeleteObject(ho_image_right_8);
	//HObject ho_image_left_8;
	//HObject ho_image_right_8;
	//HObject ho_ImageZoom_left_8;
	//HObject ho_ImageZoom_right_8;
	//HObject ho_ImagePart_left_8;//定义裁剪后的左相机图片
}

void CvisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIS_BTN_OPMON, m_vs_btn_opmon);
	DDX_Control(pDX, IDC_VIS_BTN_OPCAD, m_vs_btn_opcad);
	DDX_Control(pDX, IDC_VIS_BTN_OPDATA, m_vs_btn_opdata);
	DDX_Control(pDX, IDC_VIS_BTN_OPMOD, m_vs_btn_opmod);
	DDX_Control(pDX, IDC_VIS_BTN_OPVS, m_vs_btn_opvs);
	DDX_Control(pDX, IDC_VS_PIC_LOGO, m_vs_pic_logo);
	DDX_Control(pDX, IDC_VS_BTN_RESEND, m_vs_btn_resend);
	DDX_Control(pDX, IDC_VS_8_LEFT_PIC, m_vs_pic_8_left);
	DDX_Control(pDX, IDC_VS_8_RIGHT_PIC, m_vs_pic_8_right);
	DDX_Control(pDX, IDC_VS_LIST_LOCATE, m_vs_list_location_data);
	DDX_Text(pDX, IDC_VS_EDIT_TYPE, m_vs_edit_type);
	DDX_Text(pDX, IDC_VS_EDIT_BATCH, m_vs_edit_batch);
	DDX_Text(pDX, IDC_VS_EDIT_TEST1, m_test_locate_num);
	DDX_Text(pDX, IDC_VS_EDIT_TEST2, m_locate_test_wrong);
}


BEGIN_MESSAGE_MAP(CvisionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_VIS_BTN_OPCAD, &CvisionDlg::OnBnClickedVisBtnOpcad)
	ON_BN_CLICKED(IDC_VIS_BTN_OPMOD, &CvisionDlg::OnBnClickedVisBtnOpmod)
	
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_VIS_BTN_OPDATA, &CvisionDlg::OnBnClickedVisBtnOpdata)
	ON_WM_PAINT()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_VIS_BTN_OPMON, &CvisionDlg::OnBnClickedVisBtnOpmon)
	ON_BN_CLICKED(IDC_VS_BTN_RESEND, &CvisionDlg::OnBnClickedVsBtnResend)
	ON_BN_CLICKED(IDC_BUTTON1, &CvisionDlg::OnBnClickedButton1)
	
END_MESSAGE_MAP()


// CvisionDlg 消息处理程序
BOOL CvisionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	pVisiondlg = this;
	//ModifyStyle(WS_CAPTION, 0, 0);  // 如果只是要普通的全屏，不想去掉标题栏，就不用第一个语句
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	//按钮绘制
	{
		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPMON)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opmon.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opmon.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opmon.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opmon.setWordSize(200);

		GetDlgItem(IDC_VIS_BTN_OPVS)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opvs.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opvs.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opvs.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opvs.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPCAD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opcad.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opcad.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opcad.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opcad.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPDATA)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opdata.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opdata.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opdata.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opdata.setWordSize(200);

		//将按钮修改为BS_OWNERDRAW风格,允许button的采用自绘模式
		GetDlgItem(IDC_VIS_BTN_OPMOD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_opmod.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_opmod.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_opmod.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_opmod.setWordSize(200);

		GetDlgItem(IDC_VS_BTN_RESEND)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_vs_btn_resend.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_vs_btn_resend.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_vs_btn_resend.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_vs_btn_resend.setWordSize(200);
	}
	//改变字体
	{
		f_vs_font.CreateFontW(50,      // nHeight，文字大小
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
		GetDlgItem(IDC_VS_STATIC_NAME)->SetFont(&f_vs_font, false);


		f_vs_name.CreateFontW(18,      // nHeight，文字大小
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
		GetDlgItem(IDC_VS_STATIC_TYPE)->SetFont(&f_vs_name, false);
		GetDlgItem(IDC_VS_STATIC_BATCH)->SetFont(&f_vs_name, false);
	}
	
	

	InitLayoutVision(m_layoutVision, this);
	m_Brush.CreateSolidBrush(RGB(240, 240, 220));
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
	}
	m_vs_hBitmap_logo = (HBITMAP)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_HG), IMAGE_BITMAP, 200, 40, LR_DEFAULTCOLOR);
	m_vs_pic_logo.SetBitmap(m_vs_hBitmap_logo);

	CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;
	pdlg->OnBnClickedButtonOpen();

	//SetTimer(1, 100, NULL);

	//定位数据列表初始化
	m_vs_list_location_data.InsertColumn(0, _T("左行坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(1, _T("左列坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(2, _T("右行坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(3, _T("右列坐标"), LVCFMT_CENTER, 120);
	m_vs_list_location_data.InsertColumn(4, _T("倾角"), LVCFMT_CENTER, 120);
	

	//初始化相机驱动库
	CAMVEC();
	//扫描总线上所有的相机，这里会保证一定能搜到相机
	//扫描完毕后，相机会设置为连续采集模式，会记录自己所支持的： 
	//曝光时间，增益
	//触发模式，是否支持触发源选择

	CAMVEC().init_all_cam(SCV());

	//获取左右相机引用
	if (!SCV().empty())
	{
		leftCam = SCV()[0];
		rightCam = SCV()[1];
		//开始采集
		leftCam->Record_start();
		rightCam->Record_start();
		//设置软触发模式
		leftCam->SetAcqusition_SoftwareTrigger();
		rightCam->SetAcqusition_SoftwareTrigger();
	}
	//读取左相机标定文件
	filename = DATA_FOLDER + string("24_left_cam_calib.xml");
	//用相机标定文件来将图象处理得到的像素角点坐标转换为机器人基坐标系下的坐标
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	fs["cameraMatrix"] >> cam_matrix_left;//读num1 其中["num1"]为1.yml内标识作用
	fs["rvec"] >> r_left;
	fs["tvec"] >> t_left;
	fs.release();
	//读取右相机标定文件
	filename = DATA_FOLDER + string("26_right_cam_calib.xml");
	cv::FileStorage fss(filename, cv::FileStorage::READ);
	fss["cameraMatrix"] >> cam_matrix_right;//读num1 其中["num1"]为1.yml内标识作用
	fss["rvec"] >> r_right;
	fss["tvec"] >> t_right;
	fss.release();

	//堆上分配CClient 由智能指针管理
	dc_left_8_ptr = shared_ptr<CClientDC>(new CClientDC(GetDlgItem(IDC_VS_8_LEFT_PIC)));
	dc_right_8_ptr = shared_ptr<CClientDC>(new CClientDC(GetDlgItem(IDC_VS_8_RIGHT_PIC)));
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

HBRUSH CvisionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	if (pWnd->GetDlgCtrlID() == IDC_VS_EDIT_TYPE || IDC_VS_EDIT_BATCH)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	return hbr;
	
}


unsigned short CvisionDlg::CRC16(unsigned char* puchMsg, unsigned short usDataLen)
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


void CvisionDlg::SendData(int CommTypeIn, WORD DownAdd, DWORD DownData)
{
	// TODO: 在此处添加实现代码.
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

	CmodbusDlg *pdlg = CmodbusDlg::pModbusdlg;
	pdlg->m_SerialPort.writeData(SendArray, 8);
	

	//CPublic::m_SerialPort.writeData(SendArray, 8);
}

//每次定时器1进入时执行一个判断
//如果到达 就开始调用相机识别的函数
//相机识别完成，把一个标志位打正
//用另一个定时器每隔多少ms判断一下这个定时器
//如果判别完成就把数据发送过去


BOOL CvisionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F1)
		{
			OnBnClickedVisBtnOpmon();
		}
		if (pMsg->wParam == VK_F3)
		{
			OnBnClickedVisBtnOpcad();
		}
		if (pMsg->wParam == VK_F4)
		{
			OnBnClickedVisBtnOpdata();
		}
		if (pMsg->wParam == VK_F5)
		{
			OnBnClickedVisBtnOpmod();
		}
	}
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CvisionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_layoutVision.OnSize(cx, cy);
}




void CvisionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//static CTime preTime = CTime().GetCurrentTime();
	switch (nIDEvent)
	{
		//发送命令询问背板是否到位
		case 1:
		{
			//寄存器地址95 读1位数据
			//SprayBatch喷涂批次
			m_vs_edit_batch = SprayBatch;
			//backboard背板型号
			m_vs_edit_type = backboard;
			UpdateData(FALSE);

			SendOnce_Vision = true;
			ReadStatus = true;
			SendData(0, 74, 1);  
			
			//判断上一次发送的是否为0，为0没有触发receive则断线
			if (m_Status_T2 == 0)
			{
				//m_Status_T2 = 0; //在这里把m_Status_T2置为0
				//断线标志位
				DisconnectFlag = true;
				DisconnectNum += 1;
				//断线超过60s，即DisconnectNum=50则提示断线
				if(DisconnectNum >= 300)
				{
					KillTimer(1);
					AfxMessageBox(_T("请检查连接！"));
				}
			}
			//没有断线 通信没断线才能执行下边的程序
			else
			{
				DisconnectFlag = false;
				DisconnectNum = 0;
				m_Status_T2 = 0;//如果没有断线 那么在onReceive里会更改这个值，如果断线了那就不会更改了
				//只能对上一个时间循环里的数据进行判断
				if (ArriveFlag == true)
					//如果已经到位,且未识别完成 加一个喷胶是否喷完的标志位
					//进入判断是 到位，未识别完成，归根结底是什么时候进入判断呢，是在背板到了，再进入判断
					//背板到了，就停止定时器
					//主要问题，重启之后还是true怎么，把killtime放里边
					//背板到位之后，开始判断喷胶，
					//喷胶喷完为true,在下一台背板进来的时候喷胶标志位一直为true，进入判断的条件是
					//SprayDone == false
					//在背板到达时打为false 此时ArriveFlag = true 
				{
					//IdentifyDone初始值设为false,执行完程序后设置为true，在背板离开的时候设置为false
					//Sleep()
					//Send()  发送喷胶判断
					if (IdentifyDone == false)
					{
						KillTimer(1); //先终止该定时器，进行视觉处理

						/*
						识别完成的时候就把数据读到容器里
						其实就三个数据可以换个方式
						IdentifyDone = true;
						//对比设置的界限
						//产生是否良品字符串 在这里边判断只用写一个字符串就可以了
						//当前的日期
						//坐标数据
						//三个状态 是来自收到

						//程序识别完之后，直接开始settimer
						SetTimer(2,30,Null);
						*/
						
						//CTime curTime = CTime().GetCurrentTime();//当前时间
						//LastTime = preTime.Format("%Y-%m-%d %H:%M:%S");
						if ((vs_x >= x_floor && vs_x <= x_ceil) && (vs_y >= x_floor && vs_y <= y_ceil) && (vs_theta >= theta_floor && vs_theta <= theta_ceil))
						{
							data_good = _T("良品");
						}
						else
						{
							data_good = _T("非良品");
						}

						//if (SprayBatch > 0)
						//{
						//	
						//	
						//	LastTime;
						//	SprayBatch;
						//	backboard;
						//	vs_x;
						//	vs_y;
						//	vs_theta;
						//	
						//	//插入数据库,插入(LastTime  1
						//	//这里判断四个flag 生成四个CString 
						//	//能进来就说明当前是正常的  要不通信状态不插入数据库
						//	//上一次的坐标对比设置 CString 良与不良  7 8 9 10
						//	//背板型号 2
						//	//喷涂批次就是当前的SprayBatch 3
						//	//X Y theta坐标   4 5 6
						//}
						//preTime = curTime;
						insertdata = 0;
						SprayBatch += 1;
						//执行视觉识别程序 产生三个坐标，
						SetTimer(2, 50, NULL);
						//执行发送函数  这里的发送函数应该是启动定时器2
					}
				}

			}
			break;
		}
		case 2:
		{
			DisconnectFlag = false;
			SendOnce_Vision = false;
			m_Vision_T1 = GetTickCount64();
			if (m_Vision_T2 != 0 && RecMsgFlag == true && OverTime_Vision == false)
			{
				//
				BadVisionNum = 0;
				m_Vision_T2 = 0;
				if (LocVisionNum < 3)
				{
					//SendData;
					//SendData(1, LocVisionNum + 90, testLoc[LocVisionNum]);
					LocVisionNum++;
				}
				//发送完毕
				else
				{
					//SendData 这个跟轩举商量
					//发送完毕 发送数清0
					KillTimer(2);
					IdentifyDone = true;
					SendOnce_Vision = true;
					SendData(1, 73, 21573);
					LocVisionNum = 0;
					m_Vision_T2 = GetTickCount64();//这里加一个计时是防止下一组背板错误判断
					//SprayBatch += 1; //喷涂批次加一
					//重启定时器1
					DisconnectNum = 0;
					ReSetTime();
				}

			}
			else
			{
				BadVisionNum++;
				if (BadVisionNum < 4)
				{
					//第一个数据出现错误与后边的数据出现错误是一样的处理措施
					//先减1发送前一个数据
					LocVisionNum = LocVisionNum - 1;
					//寄存器地址  发送的数据根据LocVisionNum来定

					//SendData(1, , );
					//SendData(1, LocVisionNum + 70, testLoc[LocVisionNum]);
					//发送完之后做加一处理
					LocVisionNum++;
				}
				else
				{

					//停止发送
					KillTimer(2);
					//报错
					CString msg;
					LocVisionNum = 0;
					BadVisionNum = 0;
					//%02X为16进制显示  %d十进制 %s 字符串
					msg.Format(_T("视觉数据发送错误，请检查连接并重启！"));
					AfxMessageBox(msg);

				}
			}

			break;
		}
		case 3:
		{
			m_test_locate_num = test_times;
			//backboard背板型号
			m_locate_test_wrong = wrong_times;
			UpdateData(FALSE);

			OnBnClickedButton1();

			break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CvisionDlg::OnBnClickedVisBtnOpmon()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowMonitor();
}

void CvisionDlg::OnBnClickedVisBtnOpcad()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowCad();
}


void CvisionDlg::OnBnClickedVisBtnOpmod()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowModbus();
}


void coordinate_transformation_left()
{
	double CA= hv_COL_left_8.D();
	double CB = hv_ROW_left_8.D();
	left_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);

	left_pixel_coordinate.at<double>(0, 0) = hv_COL_left_8.D();
	left_pixel_coordinate.at<double>(0, 1) = hv_ROW_left_8.D();
	left_pixel_coordinate.at<double>(0, 2) = 1.f;
	left_pixel_coordinate = left_pixel_coordinate.t();

	//计算标定中的比例系数s_left
	t1 = r_left.inv() * cam_matrix_left.inv() * left_pixel_coordinate;
	t2 = r_left.inv() * t_left;
	s_left = t2.at<double>(2, 0) / t1.at<double>(2, 0);

	left_robot_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
	left_robot_coordinate = r_left.inv() * (cam_matrix_left.inv() * s_left * left_pixel_coordinate - t_left);//从标定文件中读标定数据,将像素坐标转换为世界坐标
	ROW_LEFT_8 = left_robot_coordinate.at<double>(0, 0);
	COL_LEFT_8 = left_robot_coordinate.at<double>(1, 0);

}

void coordinate_transformation_right()
{
	right_pixel_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
	right_pixel_coordinate.at<double>(0, 0) = hv_COL_right_8.D();
	right_pixel_coordinate.at<double>(0, 1) = hv_ROW_right_8.D();
	right_pixel_coordinate.at<double>(0, 2) = 1.f;
	right_pixel_coordinate = right_pixel_coordinate.t();

	//计算标定中的比例系数s_left
	t3 = r_right.inv() * cam_matrix_right.inv() * right_pixel_coordinate;
	t4 = r_right.inv() * t_right;
	s_right = t4.at<double>(2, 0) / t3.at<double>(2, 0);

	right_robot_coordinate = cv::Mat::ones(cv::Size(3, 1), CV_64FC1);
	right_robot_coordinate = r_right.inv() * (cam_matrix_right.inv() * s_right * right_pixel_coordinate - t_right);//从标定文件中读标定数据,将像素坐标转换为世界坐标
	ROW_RIGHT_8 = right_robot_coordinate.at<double>(0, 0);
	COL_RIGHT_8 = right_robot_coordinate.at<double>(1, 0);
}


//定义直线数量
int comparelines(HTuple linesbegin, HTuple linesend, int aaa)
{
	//aaa=1时取小的，aaa=2时取大的
	TupleLength(linesbegin, &Num_lines);
	if (Num_lines == 2 && aaa == 1)
	{
		if (linesbegin[0] > linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] > linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (b1 > a1 || b1 == a1)
			return 1;
		else
			return 0;
	}
	if (Num_lines == 2 && aaa == 2)
	{
		if (linesbegin[0] < linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] < linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (b1 < a1 || b1 == a1)
			return 1;
		else
			return 0;
	}
	if (Num_lines == 3 && aaa == 1)
	{
		if (linesbegin[0] > linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] > linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (linesbegin[2] > linesend[2])
			c1 = linesbegin[2].D();
		else
			c1 = linesend[2].D();
		if ((b1 > a1 || b1 == a1) && (b1 > c1 || b1 == c1))
			return 1;
		if ((a1 > b1 || a1 == b1) && (a1 > c1 || a1 == c1))
			return 0;
		if ((c1 > b1 || c1 == b1) && (c1 > a1 || c1 == a1))
			return 2;
	}
	if (Num_lines == 3 && aaa == 2)
	{
		if (linesbegin[0] < linesend[0] || linesbegin[0] == linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] < linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (linesbegin[2] < linesend[2])
			c1 = linesbegin[2].D();
		else
			c1 = linesend[2].D();
		if ((b1 < a1 || b1 == a1) && (b1 < c1 || b1 == c1))
			return 1;
		if ((a1 < b1 || a1 == b1) && (a1 < c1 || a1 == c1))
			return 0;
		if ((c1 < b1 || c1 == b1) && (c1 < a1 || c1 == a1))
			return 2;
	}
	if (Num_lines == 4 && aaa == 1)
	{
		if (linesbegin[0] > linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] > linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (linesbegin[2] > linesend[2])
			c1 = linesbegin[2].D();
		else
			c1 = linesend[2].D();
		if (linesbegin[3] > linesend[3])
			d1 = linesbegin[3].D();
		else
			d1 = linesend[3].D();
		if ((b1 > a1 || b1 == a1) && (b1 > c1 || b1 == c1) && (b1 > d1 || b1 == d1))
			return 1;
		if ((a1 > b1 || a1 == b1) && (a1 > c1 || a1 == c1) && (a1 > d1 || a1 == d1))
			return 0;
		if ((c1 > b1 || c1 == b1) && (c1 > a1 || c1 == a1) && (c1 > d1 || c1 == d1))
			return 2;
		if ((d1 > b1 || d1 == b1) && (d1 > a1 || d1 == a1) && (d1 > c1 || d1 == c1))
			return 3;
	}
	if (Num_lines == 4 && aaa == 2)
	{
		if (linesbegin[0] < linesend[0])
			a1 = linesbegin[0].D();
		else
			a1 = linesend[0].D();
		if (linesbegin[1] < linesend[1])
			b1 = linesbegin[1].D();
		else
			b1 = linesend[1].D();
		if (linesbegin[2] < linesend[2])
			c1 = linesbegin[2].D();
		else
			c1 = linesend[2].D();
		if (linesbegin[3] < linesend[3])
			d1 = linesbegin[3].D();
		else
			d1 = linesend[3].D();
		if ((b1 < a1 || b1 == a1) && (b1 < c1 || b1 == c1) && (b1 < d1 || b1 == d1))
			return 1;
		if ((a1 < b1 || a1 == b1) && (a1 < c1 || a1 == c1) && (a1 < d1 || a1 == d1))
			return 0;
		if ((c1 < b1 || c1 == b1) && (c1 < a1 || c1 == a1) && (c1 < d1 || c1 == d1))
			return 2;
		if ((d1 < b1 || d1 == b1) && (d1 < a1 || d1 == a1) && (d1 < c1 || d1 == c1))
			return 3;
	}
	return 0;
}



void CvisionDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}


void CvisionDlg::OnBnClickedVisBtnOpdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CHXDlg *p_hxdlg = (CHXDlg*)this->GetParent();
	p_hxdlg->ShowData();
}


void CvisionDlg::OnPaint()
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


BOOL CvisionDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnHelpInfo(pHelpInfo);
}





// 状态判断


void CvisionDlg::OnBnClickedVsBtnResend()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(3, 3000, NULL);
}


// 停止定时器1
void CvisionDlg::KillTime1()
{
	// TODO: 在此处添加实现代码.
	DisconnectNum = 0;
	//IdentifyWrongNum = 0;
	KillTimer(1);
}


// 激活定时器1
void CvisionDlg::ReSetTime()
{
	// TODO: 在此处添加实现代码.
	DisconnectNum = 0;
	SendOnce = true;
	SendOnce_Vision = true;
	SetTimer(1, 100, NULL);
}





 





void CvisionDlg::OnShowLeftPic()
{
	// 左侧相机图片显示
	hBmp_8_left = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"E://HX-master/HX/left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_8_left.SetBitmap(hBmp_8_left);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	width_RowBegin_left_8 = hv_width_line_RowBegin_left_8[0].D() / scale;
	width_ColBegin_left_8 = hv_width_line_ColBegin_left_8[0].D() / scale;
	width_RowEnd_left_8 = hv_width_line_RowEnd_left_8[0].D() / scale;
	width_ColEnd_left_8 = hv_width_line_ColEnd_left_8[0].D() / scale;

	height_RowBegin_left_8 = hv_height_line_RowBegin_left_8[0].D() / scale;
	height_ColBegin_left_8 = hv_height_line_ColBegin_left_8[0].D() / scale;
	height_RowEnd_left_8 = hv_height_line_RowEnd_left_8[0].D() / scale;
	height_ColEnd_left_8 = hv_height_line_ColEnd_left_8[0].D() / scale;

	//更新裁剪框坐标
	m_startPos_left_8 = { m_startPos_left_8_x ,m_startPos_left_8_y };
	m_endPos_left_8 = { m_endPos_left_8_x , m_endPos_left_8_y };

	dc_left_8_ptr->SelectObject(&pen_left_8);//选择画笔
	dc_left_8_ptr->MoveTo(m_startPos_left_8);
	dc_left_8_ptr->LineTo(m_startPos_left_8_x, m_endPos_left_8_y);
	dc_left_8_ptr->LineTo(m_endPos_left_8_x, m_endPos_left_8_y);
	dc_left_8_ptr->LineTo(m_endPos_left_8_x, m_startPos_left_8_y);
	dc_left_8_ptr->LineTo(m_startPos_left_8_x, m_startPos_left_8_y);

	//更新左相机定位直线坐标
	if(width_ColBegin_left_8> width_ColEnd_left_8)
		m_width_left_8 = { width_ColBegin_left_8,width_RowBegin_left_8 };
	else
		m_width_left_8 = { width_ColEnd_left_8,width_RowEnd_left_8 };
	if(height_ColBegin_left_8< height_ColEnd_left_8)
		m_height_left_8 = { height_ColBegin_left_8 ,height_RowBegin_left_8 };
	else
		m_height_left_8 = { height_ColEnd_left_8 ,height_RowEnd_left_8 };
	dc_left_8_ptr->MoveTo(m_width_left_8);
	dc_left_8_ptr->LineTo((int)(hv_COL_left_8.D() / scale),(int)( hv_ROW_left_8.D() /scale));
	dc_left_8_ptr->MoveTo(m_height_left_8);
	dc_left_8_ptr->LineTo((int)(hv_COL_left_8.D() / scale), (int)(hv_ROW_left_8.D() / scale));
}



void CvisionDlg::OnShowRightPic()
{
	// 右侧相机图片显示
	hBmp_8_right = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), L"E://HX-master/HX/right.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_vs_pic_8_right.SetBitmap(hBmp_8_right);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1  

	width_RowBegin_right_8 = hv_width_line_RowBegin_right_8[0].D() / scale;
	width_ColBegin_right_8 = hv_width_line_ColBegin_right_8[0].D() / scale;
	width_RowEnd_right_8 = hv_width_line_RowEnd_right_8[0].D() / scale;
	width_ColEnd_right_8 = hv_width_line_ColEnd_right_8[0].D() / scale;

	height_RowBegin_right_8 = hv_height_line_RowBegin_right_8[0].D() / scale;
	height_ColBegin_right_8 = hv_height_line_ColBegin_right_8[0].D() / scale;
	height_RowEnd_right_8 = hv_height_line_RowEnd_right_8[0].D() / scale;
	height_ColEnd_right_8 = hv_height_line_ColEnd_right_8[0].D() / scale;

	//更新裁剪框坐标
	m_startPos_right_8 = { m_startPos_right_8_x ,m_startPos_right_8_y };
	m_endPos_right_8 = { m_endPos_right_8_x , m_endPos_right_8_y };

	dc_right_8_ptr->SelectObject(&pen_left_8);//选择画笔
	dc_right_8_ptr->MoveTo(m_startPos_right_8);
	dc_right_8_ptr->LineTo(m_startPos_right_8_x, m_endPos_right_8_y);
	dc_right_8_ptr->LineTo(m_endPos_right_8_x, m_endPos_right_8_y);
	dc_right_8_ptr->LineTo(m_endPos_right_8_x, m_startPos_right_8_y);
	dc_right_8_ptr->LineTo(m_startPos_right_8_x, m_startPos_right_8_y);

	//更新右相机定位直线坐标
	if (width_ColBegin_right_8 < width_ColEnd_right_8)
		m_width_right_8 = { width_ColBegin_right_8,width_RowBegin_right_8 };
	else
		m_width_right_8 = { width_ColEnd_right_8,width_RowEnd_right_8 };
	if (height_ColBegin_right_8 < height_ColEnd_right_8)
		m_height_right_8 = { height_ColBegin_right_8 ,height_RowBegin_right_8 };
	else
		m_height_right_8 = { height_ColEnd_right_8 ,height_RowEnd_right_8 };
	dc_right_8_ptr->MoveTo(m_width_right_8);
	dc_right_8_ptr->LineTo((int)(hv_COL_right_8.D() / scale), (int)(hv_ROW_right_8.D() / scale));
	dc_right_8_ptr->MoveTo(m_height_right_8);
	dc_right_8_ptr->LineTo((int)(hv_COL_right_8.D() / scale), (int)(hv_ROW_right_8.D() / scale));
}

void CvisionDlg::OnLeftCollectAndCompress()
{
	//GrabImage(&ho_image_left_8, hv_AcqHandle_8_left);
	//string pic_name = CPublic::LEFT_MATWINDOW_NAME() + string(".bmp");
	string pic_name = DATA_FOLDERs + string("view1.bmp");
	ReadImage(&ho_image_left_8, pic_name.c_str());


	//压缩图片方便显示
	ZoomImageSize(ho_image_left_8, &ho_ImageZoom_left_8, hv_Width / scale, hv_Height / scale, "constant");
	//需要改成绝对路径
	WriteImage(ho_ImageZoom_left_8, "bmp", 0, "left.bmp");
	//添加裁剪框数据
	hv_Row1_left_8 = scale * m_startPos_left_8_y;
	hv_Column1_left_8 = scale * m_startPos_left_8_x;
	hv_Row2_left_8 = scale * m_endPos_left_8_y;
	hv_Column2_left_8 = scale * m_endPos_left_8_x;
	CropPart(ho_image_left_8, &ho_ImagePart_left_8, hv_Row1_left_8, hv_Column1_left_8, hv_Column2_left_8 - hv_Column1_left_8,
		hv_Row2_left_8 - hv_Row1_left_8);
}


void CvisionDlg::OnRightCollectAndCompress()
{
	//GrabImage(&ho_image_right_8, hv_AcqHandle_8_right);
	//string pic_name = CPublic::RIGHT_MATWINDOW_NAME() + string(".bmp");
	string pic_name = DATA_FOLDERs + string("view2.bmp");
	ReadImage(&ho_image_right_8, pic_name.c_str());
	//压缩图片方便显示
	ZoomImageSize(ho_image_right_8, &ho_ImageZoom_right_8, hv_Width / scale, hv_Height / scale, "constant");
	//需要改成绝对路径
	WriteImage(ho_ImageZoom_right_8, "bmp", 0, "right.bmp");
	//添加裁剪框数据
	hv_Row1_right_8 = scale * m_startPos_right_8_y;
	hv_Column1_right_8 = scale * m_startPos_right_8_x;
	hv_Row2_right_8 = scale * m_endPos_right_8_y;
	hv_Column2_right_8 = scale * m_endPos_right_8_x;
	CropPart(ho_image_right_8, &ho_ImagePart_right_8, hv_Row1_right_8, hv_Column1_right_8, hv_Column2_right_8 - hv_Column1_right_8,
		hv_Row2_right_8 - hv_Row1_right_8);
}


void CvisionDlg::OnLeftLocate()
{
	// TODO: 在此处添加实现代码.
}


void CvisionDlg::OnRightLocate()
{
	// TODO: 在此处添加实现代码.
}

int locateleft()
//UINT locateleft(LPVOID pParam)
{
	GetImageSize(ho_ImagePart_left_8, &hv_ImagePart_Width_left_8, &hv_ImagePart_Height_left_8);
	//中值滤波减小误差
	MedianImage(ho_ImagePart_left_8, &ho_ImagePart_median_left_8, "square", (HTuple)hv_Filter_block_radius_left_8, "mirrored");

	Threshold(ho_ImagePart_median_left_8, &ho_threshold_left_8, (HTuple)hv_Threshold_left_8, 255);
	Connection(ho_threshold_left_8, &ho_threshold_left_8);
	SelectShapeStd(ho_threshold_left_8, &ho_max_left_8, "max_area", 70);
	//求凸包
	ShapeTrans(ho_max_left_8, &ho_convex_left_8, "convex");
	RegionToBin(ho_convex_left_8, &ho_temporary_left_8, 255, 0, hv_ImagePart_Width_left_8, hv_ImagePart_Height_left_8);
	//检测亚像素边缘
	EdgesSubPix(ho_temporary_left_8, &ho_xld_left_8, "canny", 1, 20, 40);
	SegmentContoursXld(ho_xld_left_8, &ho_xld_left_8, "lines_circles", 0, 4, 2);

	SelectShapeXld(ho_xld_left_8, &ho_xld_height_left_8, "height", "and", hv_min_length_width_line_left_8,
		5000);
	SelectShapeXld(ho_xld_left_8, &ho_xld_width_left_8, "width", "and", hv_min_length_height_line_left_8,
		5000);
	FitLineContourXld(ho_xld_width_left_8, "tukey", -1, 0, 5, 2, &hv_width_line_RowBegin_left_8, &hv_width_line_ColBegin_left_8,
		&hv_width_line_RowEnd_left_8, &hv_width_line_ColEnd_left_8, &hv_width_line_other_data1_left_8, &hv_width_line_other_data2_left_8, &hv_width_line_other_data3_left_8);
	TupleLength(hv_width_line_RowBegin_left_8, &hv_width_line_Num_left_8);
	//检查识别出了几条直线
	if (hv_width_line_Num_left_8 == 0)
	{
		//empty_location_data();
		AfxMessageBox(_T("左边没找到横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		wrong_times++;
		return 0;
	}
	if (hv_width_line_Num_left_8 > 4)
	{
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("左边找到至少5条横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		return 0;
	}
	if (hv_width_line_Num_left_8 > 1 && hv_width_line_Num_left_8 < 5)
	{
		num_left = comparelines(hv_width_line_ColBegin_left_8, hv_width_line_ColEnd_left_8, flag_sec_small);

		hv_width_line_RowBegin_left_8 = hv_width_line_RowBegin_left_8[num_left].D();
		hv_width_line_ColBegin_left_8 = hv_width_line_ColBegin_left_8[num_left].D();
		hv_width_line_RowEnd_left_8 = hv_width_line_RowEnd_left_8[num_left].D();
		hv_width_line_ColEnd_left_8 = hv_width_line_ColEnd_left_8[num_left].D();
	}

	//计算该线段的延长线
	
	FitLineContourXld(ho_xld_height_left_8, "tukey", -1, 0, 5, 2, &hv_height_line_RowBegin_left_8, &hv_height_line_ColBegin_left_8,
		&hv_height_line_RowEnd_left_8, &hv_height_line_ColEnd_left_8, &hv_width_line_other_data1_left_8, &hv_width_line_other_data2_left_8, &hv_width_line_other_data3_left_8);
	TupleLength(hv_height_line_RowBegin_left_8, &hv_height_line_Num_left_8);
	//检查识别出了几条直线
	if (hv_height_line_Num_left_8 == 0)
	{
		//check_l_over = 0;
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("左边没找到竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}
	if (hv_height_line_Num_left_8 > 4)
	{
		//check_l_over = 0;
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("左边找到至少5条竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}

	if (hv_height_line_Num_left_8 > 1 && hv_height_line_Num_left_8 < 5)
	{
		num_left = comparelines(hv_height_line_RowBegin_left_8, hv_height_line_RowEnd_left_8, flag_sec_small);
		hv_height_line_RowBegin_left_8 = hv_height_line_RowBegin_left_8[num_left].D();
		hv_height_line_ColBegin_left_8 = hv_height_line_ColBegin_left_8[num_left].D();
		hv_height_line_RowEnd_left_8 = hv_height_line_RowEnd_left_8[num_left].D();
		hv_height_line_ColEnd_left_8 = hv_height_line_ColEnd_left_8[num_left].D();
	}

	double a1, b1, c1, a2, b2, c2, d;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = hv_width_line_RowBegin_left_8[0].D();
	y1 = hv_width_line_ColBegin_left_8[0].D();
	x2 = hv_width_line_RowEnd_left_8[0].D();
	y2 = hv_width_line_ColEnd_left_8[0].D();

	x3 = hv_height_line_RowBegin_left_8[0].D();
	y3 = hv_height_line_ColBegin_left_8[0].D();
	x4 = hv_height_line_RowEnd_left_8[0].D();
	y4 = hv_height_line_ColEnd_left_8[0].D();
	a1 = y2 - y1;
	b1 = -(x2 - x1);
	c1 = -a1 * x1 - b1 * y1;

	a2 = y4 - y3;
	b2 = -(x4 - x3);
	c2 = -a2 * x3 - b2 * y3;
	d = a1 * b2 - a2 * b1;

	hv_ROW_left_8 = (b1 * c2 - b2 * c1) / d;
	hv_COL_left_8 = (a2 * c1 - a1 * c2) / d;


	hv_width_line_RowBegin_left_8 += hv_Row1_left_8;
	hv_height_line_RowBegin_left_8 += hv_Row1_left_8;
	hv_height_line_RowEnd_left_8 += hv_Row1_left_8;
	hv_width_line_RowEnd_left_8 += hv_Row1_left_8;

	hv_width_line_ColBegin_left_8 += hv_Column1_left_8;
	hv_height_line_ColBegin_left_8 += hv_Column1_left_8;
	hv_width_line_ColEnd_left_8 += hv_Column1_left_8;
	hv_height_line_ColEnd_left_8 += hv_Column1_left_8;

	hv_ROW_left_8 += hv_Row1_left_8;
	hv_COL_left_8 += hv_Column1_left_8;


	flag_locate_over++;

	//AfxMessageBox(_T("左边相机定位结束"));
	return 0;
}



int locateright()
//UINT locateleft(LPVOID pParam)
{
	GetImageSize(ho_ImagePart_right_8, &hv_ImagePart_Width_right_8, &hv_ImagePart_Height_right_8);
	//中值滤波减小误差
	MedianImage(ho_ImagePart_right_8, &ho_ImagePart_median_right_8, "square", (HTuple)hv_Filter_block_radius_right_8, "mirrored");

	Threshold(ho_ImagePart_median_right_8, &ho_threshold_right_8, (HTuple)hv_Threshold_right_8, 255);
	Connection(ho_threshold_right_8, &ho_threshold_right_8);
	SelectShapeStd(ho_threshold_right_8, &ho_max_right_8, "max_area", 70);
	//求凸包
	ShapeTrans(ho_max_right_8, &ho_convex_right_8, "convex");
	RegionToBin(ho_convex_right_8, &ho_temporary_right_8, 255, 0, hv_ImagePart_Width_right_8, hv_ImagePart_Height_right_8);
	//检测亚像素边缘
	EdgesSubPix(ho_temporary_right_8, &ho_xld_right_8, "canny", 1, 20, 40);
	SegmentContoursXld(ho_xld_right_8, &ho_xld_right_8, "lines_circles", 0, 4, 2);

	SelectShapeXld(ho_xld_right_8, &ho_xld_height_right_8 , "height", "and", hv_min_length_width_line_right_8,
		5000);
	SelectShapeXld(ho_xld_right_8, &ho_xld_width_right_8, "width", "and", hv_min_length_height_line_right_8,
		5000);
	FitLineContourXld(ho_xld_width_right_8, "tukey", -1, 0, 5, 2, &hv_width_line_RowBegin_right_8, &hv_width_line_ColBegin_right_8,
		&hv_width_line_RowEnd_right_8, &hv_width_line_ColEnd_right_8, &hv_width_line_other_data1_right_8, &hv_width_line_other_data2_right_8, &hv_width_line_other_data3_right_8);
	TupleLength(hv_width_line_RowBegin_right_8, &hv_width_line_Num_right_8);
	//检查识别出了几条直线
	if (hv_width_line_Num_right_8 == 0)
	{
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("右边没找到横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		return 0;
	}
	if (hv_width_line_Num_right_8 > 4)
	{
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("右边至少找到5条横直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域!"));
		return 0;
	}
	if (hv_width_line_Num_right_8 < 5 && hv_width_line_Num_right_8 > 1)
	{
		num_right = comparelines(hv_width_line_ColEnd_right_8, hv_width_line_ColBegin_right_8, flag_sec_big);
		hv_width_line_RowBegin_right_8 = hv_width_line_RowBegin_right_8[num_right].D();
		hv_width_line_ColBegin_right_8 = hv_width_line_ColBegin_right_8[num_right].D();
		hv_width_line_RowEnd_right_8 = hv_width_line_RowEnd_right_8[num_right].D();
		hv_width_line_ColEnd_right_8 = hv_width_line_ColEnd_right_8[num_right].D();
	}

	FitLineContourXld(ho_xld_height_right_8, "tukey", -1, 0, 5, 2, &hv_height_line_RowBegin_right_8, &hv_height_line_ColBegin_right_8,
		&hv_height_line_RowEnd_right_8, &hv_height_line_ColEnd_right_8, &hv_width_line_other_data1_right_8, &hv_width_line_other_data2_right_8, &hv_width_line_other_data3_right_8);
	TupleLength(hv_height_line_RowBegin_right_8, &hv_height_line_Num_right_8);
	//检查识别出了几条直线
	if (hv_height_line_Num_right_8 == 0)
	{
		//check_l_over = 0;
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("右边没找到竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}
	if (hv_height_line_Num_right_8 > 4)
	{
		//check_l_over = 0;
		//empty_location_data();
		wrong_times++;
		AfxMessageBox(_T("右边至少找到5条竖直线! 定位失败,检测直角失败,请框选图片裁剪框时尽量选择直角附近光照对比明显的区域"));
		return 0;
	}
	if (hv_height_line_Num_right_8 > 1 && hv_height_line_Num_right_8 < 5)
	{
		num_right = comparelines(hv_height_line_RowBegin_right_8, hv_height_line_RowEnd_right_8, flag_sec_small);
		hv_height_line_RowBegin_right_8 = hv_height_line_RowBegin_right_8[num_right].D();
		hv_height_line_ColBegin_right_8 = hv_height_line_ColBegin_right_8[num_right].D();
		hv_height_line_RowEnd_right_8 = hv_height_line_RowEnd_right_8[num_right].D();
		hv_height_line_ColEnd_right_8 = hv_height_line_ColEnd_right_8[num_right].D();
	}


	double a1, b1, c1, a2, b2, c2, d;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	x1 = hv_width_line_RowBegin_right_8[0].D();
	y1 = hv_width_line_ColBegin_right_8[0].D();
	x2 = hv_width_line_RowEnd_right_8[0].D();
	y2 = hv_width_line_ColEnd_right_8[0].D();

	x3 = hv_height_line_RowBegin_right_8[0].D();
	y3 = hv_height_line_ColBegin_right_8[0].D();
	x4 = hv_height_line_RowEnd_right_8[0].D();
	y4 = hv_height_line_ColEnd_right_8[0].D();
	a1 = y2 - y1;
	b1 = -(x2 - x1);
	c1 = -a1 * x1 - b1 * y1;

	a2 = y4 - y3;
	b2 = -(x4 - x3);
	c2 = -a2 * x3 - b2 * y3;
	d = a1 * b2 - a2 * b1;
	if (d == 0)
		;
	else
		if (d > 0)
			;
		else
			;
	hv_ROW_right_8 = (b1 * c2 - b2 * c1) / d;
	hv_COL_right_8 = (a2 * c1 - a1 * c2) / d;

	hv_width_line_RowBegin_right_8 += hv_Row1_right_8;
	hv_height_line_RowBegin_right_8 += hv_Row1_right_8;
	hv_height_line_RowEnd_right_8 += hv_Row1_right_8;
	hv_width_line_RowEnd_right_8 += hv_Row1_right_8;

	hv_width_line_ColBegin_right_8 += hv_Column1_right_8;
	hv_height_line_ColBegin_right_8 += hv_Column1_right_8;
	hv_width_line_ColEnd_right_8 += hv_Column1_right_8;
	hv_height_line_ColEnd_right_8 += hv_Column1_right_8;

	hv_ROW_right_8 += hv_Row1_right_8;
	hv_COL_right_8 += hv_Column1_right_8;
	//将直角边端点写入全局变量中
	flag_locate_over++;
	
	//AfxMessageBox(_T("右边相机定位结束"));
	return 0;
}

void CvisionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	

	try
	{
		//发送软触发指令
		leftCam->m_frame_ready_ = false;
		rightCam->m_frame_ready_ = false;
		leftCam->ObjFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();
		rightCam->ObjFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();
	}
	catch (CGalaxyException& e)
	{
		MessageBox(CString(e.what()));
		return;
	}
	catch (std::exception& e)
	{
		MessageBox(CString(e.what()));
		return;
	}
	while (leftCam->m_frame_ready_ == false || rightCam->m_frame_ready_ == false)
	{

	}

	//Sleep(4000);

	OnLeftCollectAndCompress();
	OnRightCollectAndCompress();
	locateleft();
	locateright();
	OnShowLeftPic();
	OnShowRightPic();

	if (flag_locate_over == 2)
	{
		coordinate_transformation_left();
		coordinate_transformation_right();
		OnShowList();
		flag_locate_over = 0;
		OnInitLocateData();
	}
	test_times++;
}




void CvisionDlg::OnShowList()
{
	// TODO: 在此处添加实现代码.
	//将定位数据添加到列表中
	Column_Left.Format(_T("%f"), ROW_LEFT_8);
	m_vs_list_location_data.InsertItem(0, Column_Left);

	Row_Left.Format(_T("%f"), COL_LEFT_8);
	m_vs_list_location_data.SetItemText(0, 1, Row_Left);

	Column_Right.Format(_T("%f"), ROW_RIGHT_8);
	m_vs_list_location_data.SetItemText(0, 2, Column_Right);

	Row_Right.Format(_T("%f"), COL_RIGHT_8);
	m_vs_list_location_data.SetItemText(0, 3, Row_Right);

	ANGLE = -atan((ROW_RIGHT_8 - ROW_LEFT_8) / (COL_RIGHT_8 - COL_LEFT_8));

	Angle.Format(_T("%f"), ANGLE);
	m_vs_list_location_data.SetItemText(0, 4, Angle);
}


void CvisionDlg::OnInitLocateData()
{
	// TODO: 在此处添加实现代码.
	// 将所有定位数据置零
	hv_ROW_left_8 = 0;
	hv_COL_left_8 = 0;
	hv_ROW_right_8 = 0;
	hv_COL_right_8 = 0;

	ROW_LEFT_8 = 0;
	COL_LEFT_8 = 0;
	ROW_RIGHT_8 = 0;
	COL_RIGHT_8 = 0;
	ANGLE = 0;

	hv_width_line_RowBegin_left_8 = 0;
	hv_width_line_ColBegin_left_8 = 0;
	hv_width_line_RowEnd_left_8 = 0;
	hv_width_line_ColEnd_left_8 = 0;
	hv_height_line_RowBegin_left_8 = 0;
	hv_height_line_ColBegin_left_8 = 0;
	hv_height_line_RowEnd_left_8 = 0;
	hv_height_line_ColEnd_left_8 = 0;

	hv_width_line_RowBegin_right_8 = 0;
	hv_width_line_ColBegin_right_8 = 0;
	hv_width_line_RowEnd_right_8 = 0;
	hv_width_line_ColEnd_right_8 = 0;
	hv_height_line_RowBegin_right_8 = 0;
	hv_height_line_ColBegin_right_8 = 0;
	hv_height_line_RowEnd_right_8 = 0;
	hv_height_line_ColEnd_right_8 = 0;
}
