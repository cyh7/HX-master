#ifndef _LAYOUT_INIT_H_
#define _LAYOUT_INIT_H_

#include "CLayout.h"

void InitLayoutData(CLayout& layout, CWnd* parent)
{
	layout.Initial(parent);
	layout.RegisterControl(IDC_DAT_STATIC_NAME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_STATIC_TIME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_STATIC_TIMEEND, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DATETIMEPICKER1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DATETIMEPICKER2, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DATETIMEPICKER3, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DATETIMEPICKER4, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_DAT_BTN_QUERY, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_DELETE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_CLEAR, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_LIST, CLayout::e_stretch_all);


	layout.RegisterControl(IDC_DAT_BTN_OPMON, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_OPVS, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_OPCAD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_OPMOD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_BTN_OPDAT, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_DAT_STATIC1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_DAT_STATIC2, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_DATA_PIC_LOGO, CLayout::e_stretch_all);
}

#endif