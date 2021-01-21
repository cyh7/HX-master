#ifndef _LAYOUT_INIT_H_
#define _LAYOUT_INIT_H_

#include "CLayout.h"

void InitLayoutVision(CLayout& layout, CWnd* parent)
{
	layout.Initial(parent);
	
	layout.RegisterControl(IDC_VIS_BTN_OPMON, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPCAD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPDATA, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPMOD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPVS, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_BTN_RESEND, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_STATIC_NAME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_PIC_LOGO, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_STATIC_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_STATIC_BATCH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_BATCH, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_8_RIGHT_PIC, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_8_LEFT_PIC, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_LIST_LOCATE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_BUTTON1, CLayout::e_stretch_all);


	layout.RegisterControl(IDC_VS_STATIC_TEST1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_STATIC_TEST2, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_TEST1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_TEST2, CLayout::e_stretch_all);
}

#endif
