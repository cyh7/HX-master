// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "InfoFile.h"
#include "CchagePwdDlg.h"

//全局变量 管理员登录标志位
bool LoginFlag = false;

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_user(_T(""))
	, m_pwd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_user);
	DDX_Text(pDX, IDC_EDIT_PWD, m_pwd);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_log_btn_login);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_log_btn_cancel);
	DDX_Control(pDX, IDC_BUTTON_SETPWD, m_log_btn_setpwd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CLoginDlg::OnBnClickedButtonLogin)
	
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CLoginDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETPWD, &CLoginDlg::OnBnClickedButtonSetpwd)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	//登录按钮点击事件
	UpdateData(true);//控件的内容同步到变量上

	if (m_pwd.IsEmpty() || m_user.IsEmpty())
	{
		MessageBox(_T("输入的内容不能为空"));
		return;
	}

	//获取正确的值
	CInfoFile file;
	CString name, pwd;
	file.ReadLogin(name, pwd);

	if (name == m_user) //用户名一致
	{
		if (pwd == m_pwd)
		{
			//管理员登陆标志位打为正，管理员登陆成功
			LoginFlag = true;
			//关闭当前的对话框
			CDialog::OnCancel();
		}
		else
		{
			MessageBox(_T("密码有误"));
		}
	}
	else
	{
		MessageBox(_T("用户名有误"));
	}
}




//重写onok避免按回车使窗口消失
void CLoginDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CLoginDlg::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//默认登陆信息
	CInfoFile file;
	CString name, pwd;
	file.ReadLogin(name, pwd);
	//默认用户名
	m_user = name;
	//密码
	//m_pwd = pwd;
	UpdateData(FALSE);

	m_log_Brush.CreateSolidBrush(RGB(240, 240, 220));
	//按钮重绘
	{
		GetDlgItem(IDC_BUTTON_LOGIN)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_log_btn_login.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_log_btn_login.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_log_btn_login.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_log_btn_login.setWordSize(100);

		GetDlgItem(IDC_BUTTON_CANCEL)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_log_btn_cancel.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_log_btn_cancel.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_log_btn_cancel.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_log_btn_cancel.setWordSize(100);

		GetDlgItem(IDC_BUTTON_SETPWD)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_log_btn_setpwd.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_log_btn_setpwd.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_log_btn_setpwd.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_log_btn_setpwd.setWordSize(100);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedButtonSetpwd()
{
	// TODO: 在此添加控件通知处理程序代码
	CchagePwdDlg m_Dlg_pwd;
	m_Dlg_pwd.DoModal();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	
	if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
		return TRUE;
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)//如果当前控件属于静态文本
	{
		//pDC->SetTextColor(RGB(255, 0, 0)); //字体颜色
		pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		pDC->SetBkMode(TRANSPARENT); //设置字体背景为透明
		//pDC->SetFont(p_font);//设置字体

		//pDC-> SetBkColor(RGB(0, 0, 255));  //字体背景色
		return (HBRUSH)m_log_Brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_USER || IDC_EDIT_PWD)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_log_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	return hbr;
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(rect);

	dc.FillSolidRect(rect, RGB(240, 240, 220));
	//dc.FillSolidRect(rect, RGB(125, 125, 255));

	CDialogEx::OnPaint();		   // 不为绘图消息调用 CDialogEx::OnPaint()
}
