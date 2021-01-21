// CchagePwdDlg.cpp: 实现文件
//

#include "pch.h"
#include "HX.h"
#include "CchagePwdDlg.h"
#include "afxdialogex.h"
#include "InfoFile.h"

// CchagePwdDlg 对话框

IMPLEMENT_DYNAMIC(CchagePwdDlg, CDialogEx)

CchagePwdDlg::CchagePwdDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGEPWD, pParent)
	, m_oldPwd(_T(""))
	, m_newPwd(_T(""))
	, m_surePwd(_T(""))
{

}

CchagePwdDlg::~CchagePwdDlg()
{
}

void CchagePwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_oldPwd);
	DDX_Text(pDX, IDC_EDIT3, m_newPwd);
	DDX_Text(pDX, IDC_EDIT4, m_surePwd);
	DDX_Control(pDX, IDC_BUTTON1, m_chg_btn_sure);
	DDX_Control(pDX, IDC_BUTTON4, m_chg_btn_cancel);
}


BEGIN_MESSAGE_MAP(CchagePwdDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CchagePwdDlg::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CchagePwdDlg 消息处理程序


void CchagePwdDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CInfoFile file;
	CString name, pwd;
	file.ReadLogin(name, pwd);

	//修改密码功能
	if (m_oldPwd.IsEmpty() || m_newPwd.IsEmpty() || m_surePwd.IsEmpty())
	{
		MessageBox(_T("输入的内容不能为空！"));
		return;
	}

	//新密码和确定密码不一致
	if (m_newPwd != m_surePwd)
	{
		MessageBox(_T("新密码与确定密码要一致！"));
		return;
	}

	//旧密码与本地的不一致
	if (pwd != m_oldPwd)
	{
		MessageBox(_T("旧密码输入错误！"));
		return;
	}
	//旧密码与新密码不能一致
	if (pwd == m_newPwd)
	{
		MessageBox(_T("新密码与旧密码不能一致！"));
		return;
	}

	//CString转char*
	CStringA tmp;
	CStringA tmp2;
	tmp = name;
	tmp2 = m_newPwd;
	file.WritePwd(tmp.GetBuffer(), tmp2.GetBuffer());
	MessageBox(_T("修改成功！"));

	//清空内容
	m_oldPwd.Empty();
	m_surePwd.Empty();
	m_newPwd.Empty();
	UpdateData(FALSE);
}


BOOL CchagePwdDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)    //屏蔽回车和ESC  
			return TRUE;
		if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)  //屏蔽ALT+F4
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CchagePwdDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_chg_Brush.CreateSolidBrush(RGB(240, 240, 220));
	//按钮重绘
	{
		GetDlgItem(IDC_BUTTON1)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_chg_btn_sure.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_chg_btn_sure.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_chg_btn_sure.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_chg_btn_sure.setWordSize(100);

		GetDlgItem(IDC_BUTTON4)->ModifyStyle(0, BS_OWNERDRAW, 0);
		//设置Button Down的背景色，SetDownColor()和SetUpnColor()是CMyButton类中的析构函数
		m_chg_btn_cancel.SetDownColor(RGB(102, 139, 139));
		//设置Button Up的背景色
		m_chg_btn_cancel.SetUpColor(RGB(2, 158, 160));
		//设置字体颜色
		m_chg_btn_cancel.setWordColor(RGB(255, 250, 250));
		//设置字体大小
		m_chg_btn_cancel.setWordSize(100);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH CchagePwdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		return (HBRUSH)m_chg_Brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_EDIT1 || IDC_EDIT3 || IDC_EDIT4)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(50, 50, 200));  //字体颜色
		//pDC->SetBkColor(RGB(240, 240, 220));   //字体背景色
		return (HBRUSH)m_chg_Brush.GetSafeHandle();  // 设置背景色
		//return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	return hbr;
}


void CchagePwdDlg::OnPaint()
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
