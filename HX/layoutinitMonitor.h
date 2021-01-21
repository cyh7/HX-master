#ifndef _LAYOUT_INIT_H_
#define _LAYOUT_INIT_H_

#include "CLayout.h"

void InitLayoutMonitor(CLayout& layout, CWnd* parent)
{
	layout.Initial(parent);
	layout.RegisterControl(IDC_MON_STATIC_PLC, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_GLUE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_STOP, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_PIC_PLC, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_PIC_GLUE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_PIC_STOP, CLayout::e_stretch_all);


	layout.RegisterControl(IDC_MON_STATIC_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_BATCH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_COORD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_X, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_Y, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_STATIC_THETA, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_MON_EDIT_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_EDIT_BATCH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_EDIT_X, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_EDIT_Y, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_EDIT_THETA, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_MON_BTN_OPVS, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_BTN_OPMON, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_BTN_OPCAD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_BTN_OPDATA, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_BTN_OPMOD, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_MON_STATIC_NAME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_PIC_MAIN, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MON_PIC_LOGO, CLayout::e_stretch_all);
}

#endif
