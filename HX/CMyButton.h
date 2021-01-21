#pragma once
#include <afxwin.h>
class CMyButton :
	public CButton
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	//三种颜色分别为文字，Button Down的背景颜色，Button Up的背景颜色
	COLORREF m_TextColor, m_DownColor, m_UpColor;
	//Button Down的背景颜色，Button Up的背景颜色标志位
	bool bDownColor, bUpColor;
	//设置文字大小
	int nWordSize = 100;
	UINT BKID;//记录图片ID
public:
	//设置Button Down的背景颜色
	void SetDownColor(COLORREF color);
	//设置Button Up的背景颜色
	void SetUpColor(COLORREF color);
	//设置背景图片
	void setBit(UINT ID);
	//设置文字大小，默认为10号
	void setWordSize(int wordsize);
	//设置文字颜色默认为黑色
	void setWordColor(COLORREF wordColor);
public:
	// 文字处理
	void setWord(CRect rect, CDC* pDc, int size);
	//绘制背景图片
	void brushBit(CRect rect, CDC *pDc, UINT ID);
	//重载DrawItem
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

