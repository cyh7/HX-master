#include "pch.h"
#include "CMyButton.h"
BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


void CMyButton::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMyButton::SetDownColor(COLORREF color)
{
	m_DownColor = color;
	bDownColor = TRUE;//颜色是否设置标志位
}
void CMyButton::SetUpColor(COLORREF color)
{
	m_UpColor = color;
	bUpColor = TRUE;
}
void CMyButton::setBit(UINT ID)
{
	BKID = ID;
}
void CMyButton::setWordSize(int wordsize)
{
	nWordSize = wordsize;
}
void CMyButton::setWordColor(COLORREF wordColor)
{
	m_TextColor = wordColor;
}
void CMyButton::brushBit(CRect rect, CDC *pDc, UINT ID)
{
	CDC   dcBmp;             //定义并创建一个内存设备环境
	dcBmp.CreateCompatibleDC(pDc);             //创建兼容性DC
	CBitmap   bmpBackground;
	bmpBackground.LoadBitmap(ID);    //载入资源中图片
	BITMAP   m_bitmap;                         //图片变量               
	bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
	  //将位图选入临时内存设备环境
	CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
	//调用函数显示图片StretchBlt显示形状可变
	pDc->SetStretchBltMode(HALFTONE);
	pDc->StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
}
// 文字处理
void CMyButton::setWord(CRect rect, CDC* pDc, int nWordSize)
{
	CString str;
	GetWindowText(str);//获取文字
	CFont font;
	font.CreatePointFont(nWordSize, L"微软雅黑");//设置控件文字大小与字体
	pDc->SelectObject(&font);
	pDc->SetTextColor(m_TextColor);//设置文字颜色
	// dc.SetBkColor(RGB(255, 0, 0));//设置文字背景颜色
	pDc->SetBkMode(TRANSPARENT);//设置文字背景透明
	pDc->DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_TABSTOP);//绘制文字
}
void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// TODO:  添加您的代码以绘制指定项
	CDC dc;
	dc.Attach(lpDIS->hDC);//得到绘制的设备环境CDC
	//按钮外框处理
	{
		dc.SelectStockObject(NULL_BRUSH);//选择按钮默认色
		dc.Rectangle(&lpDIS->rcItem);//画出按钮框
	}
	CString str;
	CRect rect = lpDIS->rcItem;
	GetClientRect(&rect);//得到客户端坐标 //ScreenToClient();将屏幕坐标转换为客户端坐标
	 //加载背景图片，判断是否加载背景图片，没有则使用默认背景
	if (BKID != 0) brushBit(rect, &dc, BKID);
	//按钮按下时改变背景色
	if (lpDIS->itemState&ODS_SELECTED)
	{
		if (bDownColor)
		{
			CBrush brush(m_DownColor);
			dc.FillRect(&(lpDIS->rcItem), &brush);//利用画刷brush，填充矩形框
		}
	}
	else//当按钮不操作或者弹起时改变背景色
	{
		if (bUpColor)
		{
			CBrush brush(m_UpColor);
			dc.FillRect(&(lpDIS->rcItem), &brush);//利用画刷brush，填充矩形框
		}
	}
	//选中了本控件,高亮边框
	if ((lpDIS->itemState&ODS_SELECTED) && (lpDIS->itemAction &(ODA_SELECT | ODA_DRAWENTIRE)))
	{
		//设置按钮边框与按钮色相反
		COLORREF fc = RGB(255 - GetRValue(m_UpColor), 255 - GetGValue(m_UpColor), 255 - GetBValue(m_UpColor));
		CBrush brush(fc);
		dc.FrameRect(&(lpDIS->rcItem), &brush);//用画刷brush，填充矩形边框
	}
	//控制的选中状态结束,去掉边框
	if (!(lpDIS->itemState &ODS_SELECTED) && (lpDIS->itemAction & ODA_SELECT))
	{
		CBrush brush(m_UpColor);
		dc.FrameRect(&lpDIS->rcItem, &brush);
		dc.Detach();
	}
	//文字处理（绘制文字）
	setWord(rect, &dc, nWordSize);
}