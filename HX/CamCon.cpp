#include "pch.h"
#include "CamCon.h"
#define DATA_FOLDER "D://HX-master/HX-master/HX/"

int CamCon::init_all_cam(vector<shared_ptr<SingleCam>>& cam_vec)
{
	bool bIsDeviceOpen = false;              ///< 设备是否已打开标识
	bool bIsStreamOpen = false;              ///< 设备流是否已打开标识

	gxstring serialNum;
	gxstring macAddr;
	try
	{
		if (!cam_vec.empty())
			return -1;
		int i = 0;
		const size_t totalDeviceNum = vectorDeviceInfo.size();
		CString a;
		a.Format(_T("%d 个设备"), totalDeviceNum);
		//AfxMessageBox(a);

		if (totalDeviceNum == 0) // 完全搜不到设备
		{
			throw exception("No Device Detected");
		}
		//暂时先不对单相机做任何处理
		else if (totalDeviceNum == 1)
		{
			throw exception("Only one Cam Is Detected");
		}

		cam_vec.resize(2);

		CGXDevicePointer ObjDevicePtr;

		//读取ROI配置文件
		Rect leftROI = Rect(PC_WIDTH / 3, 0, PC_WIDTH / 3, PC_HEIGHT);
		Rect rightROI = Rect(PC_WIDTH / 3, 0, PC_WIDTH / 3, PC_HEIGHT);

		FILE* fp;
		pair<Rect, Rect> ROI_pair;
		fopen_s(&fp, "ROI.ini", "rb");
		if (fp != NULL)
		{
			//AfxMessageBox(L"read an ini");
			fread(&ROI_pair, sizeof(ROI_pair), 1, fp);
			leftROI = ROI_pair.first;
			rightROI = ROI_pair.second;
			fclose(fp);
		}


		while (i < totalDeviceNum)
		{

			//AfxMessageBox(CString(vectorDeviceInfo[i].GetVendorName()));
			//AfxMessageBox(CString(vectorDeviceInfo[i].GetSN()));//通过Serial Number 进行开机，有好处，比MAC更加直观
			serialNum = vectorDeviceInfo[i].GetSN();
			macAddr = vectorDeviceInfo[i].GetMAC();


			/*ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
			serialNum,
			GX_ACCESS_EXCLUSIVE);*/



			if (serialNum.c_str() == CPublic::LEFT_CAM_SN())
			{
				shared_ptr<SingleCam> p(new SingleCam(serialNum, CPublic::LEFT_MATWINDOW_NAME(), leftROI));
				cam_vec[LEFT] = p;

			}
			else if (serialNum.c_str() == CPublic::RIGHT_CAM_SN())
			{
				shared_ptr<SingleCam> p(new SingleCam(serialNum, CPublic::RIGHT_MATWINDOW_NAME(), rightROI));
				cam_vec[RIGHT] = p;

			}
			else
			{
				throw exception("Found a new Cam");
			}
			++i;
		}
	}


	catch (CGalaxyException& e)
	{
		cout << "错误码: " << e.GetErrorCode() << endl;
		cout << "错误描述信息: " << e.what() << endl;
		//AfxMessageBox(CString(e.what()));
		//相机初始化出现问题，提示是否选择是否进行相机重连
		//1.检查电缆
		//2.进行复位
		//如果cam_vec不为空，则直接清空数组进行执行关设备，关流
		if (!cam_vec.empty())
			cam_vec.clear();

		//这时候，应该铁定是炸了，复位这个相机
		IGXFactory::GetInstance().GigEResetDevice(macAddr, GX_MANUFACTURER_SPECIFIC_RECONNECT);
		//清空设备数组,否则接下来的更新设备列表会抛出异常
		if (vectorDeviceInfo.size() != 0)
			vectorDeviceInfo.clear();
		try {
			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

		}
		catch (CGalaxyException& e)
		{

		}
		//更新设备列表
		while (vectorDeviceInfo.size() != 2)
		{
			vectorDeviceInfo.clear();
			try {
				IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);

			}
			catch (CGalaxyException& e)
			{

			}

		}

		//AfxMessageBox(L"发现掉线相机");
		init_all_cam(cam_vec);
	}
	catch (std::exception& e)
	{

		//相机初始化出现问题
		if (string(e.what()) == "No Device Detected") //完全没有检测到任何相机
		{
			//执行相应的措施，比如提供一个按钮再次执行init_all_cam
			//AfxMessageBox(CString(e.what()));
			//请检查接电，网线连接是否正常，然后点击“连接相机按钮”
			//如果多次连接失败，请尝试使用复位相机按钮
		}
		else if (string(e.what()) == "Found a new Cam") //��⵽�����
		{
			//ִ����Ӧ�Ĵ�ʩ�������ṩһ����ť�ٴ�ִ��init_all_cam
			//AfxMessageBox(CString(e.what()));
			//����ӵ磬���������Ƿ�������Ȼ���������������ť��
			//����������ʧ�ܣ��볢��ʹ�ø�λ�����ť
		}
		//else if (string(e.what()) == "Only one Cam Is Detected")
		//{
		//	//执行相应的措施，比如提供一个按钮再次执行init_all_cam
		//	//或者允许单相机执行检测任务，这一部分先不做
		//}
		//如果cam_vec不为空，则直接清空数组进行执行关设备，关流
		if (!cam_vec.empty())
			cam_vec.clear();
	}

	return 0;
}

int CamCon::CamInit()
{
	//初始化
	IGXFactory::GetInstance().Init();
	//枚举设备
	return ScanCam();

}

int CamCon::CamUnInit()
{
	IGXFactory::GetInstance().Uninit();
	return 0;
}

size_t CamCon::ScanCam()
{
	vectorDeviceInfo.clear();
	IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
	if (0 == vectorDeviceInfo.size())
	{
		//cout << "无可用设备!" << endl;
		CString C(L"无可用设备!");
		AfxMessageBox(C);
		return 0;
	}

	for (int i = 0; i < vectorDeviceInfo.size(); ++i)
	{
		gxstring tmp;
		tmp = vectorDeviceInfo[i].GetMAC();
		macAddrList.push_back(tmp);
	}

	return vectorDeviceInfo.size();
}

void CamCon::SoftResetAllCam()
{
	for (int i = 0; i < vectorDeviceInfo.size(); ++i)
	{
		IGXFactory::GetInstance().GigEResetDevice(macAddrList[i], GX_MANUFACTURER_SPECIFIC_RECONNECT);

	}

}

SingleCam::SingleCam(gxstring m_serialNum, const char* windowname, const cv::Rect& roi)
	: serialNum(m_serialNum), showWindowName(windowname), ROI(roi), m_bIsOffline(false), m_bIsSnap(false), m_bIsOpened(true)/*此时设备和流一定是打开的*/
{
	bool bIsDeviceOpen = false;              ///< 设备是否已打开标识
	bool bIsStreamOpen = false;              ///< 设备流是否已打开标识

	ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(m_serialNum, GX_ACCESS_EXCLUSIVE);
	bIsDeviceOpen = true;
	try
	{
		//打开流,如果设备流数大于零则打开

		int nStreamCount = ObjDevicePtr->GetStreamCount();
		if (nStreamCount > 0)
		{
			ObjStreamPtr = ObjDevicePtr->OpenStream(0);
			ObjStreamFeatureControlPtr = ObjStreamPtr->GetFeatureControl();
		}


		bIsStreamOpen = true;
		//获取远端设备属性控制器
		ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

		//生成设备掉线处理函数句柄
		//GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline = NULL;
		pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
		//hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, this);
		//生成回调采集函数句柄
		pCaptureEventHandler = new CSampleCaptureEventHandler();
		ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, this);


		// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
		// 以提高网络相机的采集性能,设置方法参考以下代码。
		GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
		if (GX_DEVICE_CLASS_GEV == objDeviceClass)
		{
			// 判断设备是否支持流通道数据包功能
			if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
			{
				// 获取当前网络环境的最优包长值
				int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
				// 将最优包长值设置为当前设备的流通道包长值
				ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
			}
		}

		//初始化相机各个参数
		__InitParam();
		//
	}

	catch (CGalaxyException& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			ObjStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			ObjDevicePtr->Close();
		}
		//AfxMessageBox(L"error open Device");
		//AfxMessageBox(CString(e.what()));
		//提示处理函数注册失败，流和设备已经关闭
	}
	catch (std::exception& e)
	{
		//判断设备流是否已打开
		if (bIsStreamOpen)
		{
			ObjStreamPtr->Close();
		}

		//判断设备是否已打开
		if (bIsDeviceOpen)
		{
			ObjDevicePtr->Close();
		}

		cout << "<" << e.what() << ">" << endl;
		//提示处理注册失败，流和设备已经关闭

	}

}
SingleCam::~SingleCam()
{
	try
	{
		//判断是否已经停止采集
		if (m_bIsSnap)
		{
			//发送停采命令
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();

			//关闭流层采集
			ObjStreamPtr->StopGrab();

			//注销采集回调
			ObjStreamPtr->UnregisterCaptureCallback();
		}
	}
	catch (CGalaxyException)
	{
		//do nothing
	}
	try
	{
		//关闭流对象
		ObjStreamPtr->Close();
	}
	catch (CGalaxyException)
	{
		//do nothing
	}
	try
	{
		//关闭设备
		ObjDevicePtr->Close();
	}
	catch (CGalaxyException)
	{
		//do nothing
	}

	m_bIsOpened = false;
	m_bIsSnap = false;


	//销毁事件回调指针
	if (NULL != pCaptureEventHandler)
	{
		delete pCaptureEventHandler;
		pCaptureEventHandler = NULL;
	}
	if (NULL != pDeviceOfflineEventHandler)
	{
		delete pDeviceOfflineEventHandler;
		pDeviceOfflineEventHandler = NULL;
	}
	if (NULL != pFeatureEventHandler)
	{
		delete pFeatureEventHandler;
		pFeatureEventHandler = NULL;
	}
}

void SingleCam::SetAcqusition_Continuous()
{
	try
	{
		//关闭触发模式
		//ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
		//设置采集模式为连续
		ObjFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
	}
	catch (CGalaxyException& e)
	{
		//AfxMessageBox(CString(e.what()));
	}

}

void SingleCam::SetAcqusition_SoftwareTrigger()
{
	try
	{
		//打开触发模式
		ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("On");
		//设置触发模式为软触发
		ObjFeatureControlPtr->GetEnumFeature("TriggerSource")->SetValue("Software");
	}
	catch (CGalaxyException& e)
	{
		AfxMessageBox(CString(e.what()));
	}
}

void SingleCam::SendSoftwareTrigger()
{
	try
	{
		ObjFeatureControlPtr->GetCommandFeature("TriggerSoftware")->Execute();
	}
	catch (CGalaxyException& ge)
	{
		AfxMessageBox(CString(ge.what()));
	}
}

void SingleCam::Record_start()
{
	try
	{
		//ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, this);

		ObjStreamPtr->StartGrab();

		ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();

		m_bIsSnap = true;//置位执行采集标志位
	}
	catch (CGalaxyException& e)
	{
		cout << "错误码: " << e.GetErrorCode() << endl;
		cout << "错误描述信息: " << e.what() << endl;
		AfxMessageBox(CString(e.what()));
	}
	catch (std::exception& e)
	{
		cout << "错误描述信息: " << e.what() << endl;
	}

}
void SingleCam::Record_stop()
{

	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();
	//ObjStreamPtr->UnregisterCaptureCallback();

	m_bIsSnap = false;

	//停止采集，填充Mat_Vec
	//Mat resized;
	//resize(src, resized, Size(PC_WIDTH / AS_RATIO, PC_HEIGHT / AS_RATIO));
	//CPublic::Mat_Vec()[CPublic::ORIGINAL] = src;
	//CPublic::Mat_Vec()[CPublic::RESIZED] = resized;
	//CPublic::Mat_Vec()[CPublic::CROPPED_ORIGINAL] = src(ROI);
	//cv::Rect roiResized(ROI.x / AS_RATIO, ROI.y / AS_RATIO, ROI.width / AS_RATIO, ROI.height / AS_RATIO);
	//CPublic::Mat_Vec()[CPublic::CROPPED_RESIZED] = resized(roiResized);
	////imshow("resized", CPublic::Mat_Vec()[CPublic::CROPPED_ORIGINAL]);
	//imwrite("test_pic.bmp", CPublic::Mat_Vec()[CPublic::CROPPED_ORIGINAL]);

}

void SingleCam::__ProcessOffline()
{
	try
	{
		printf("**********************Process Offline**********************\r");
		//判断设备是否停止采集
		if (m_bIsSnap)
		{
			cout << "\n<Send stop snap command to device>" << endl;
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
		}
	}
	catch (CGalaxyException& e)
	{
		cout << "<" << e.GetErrorCode() << ">" << "<" << e.what() << ">" << endl;
	}
	catch (std::exception& e)
	{
		cout << "<" << e.what() << ">" << endl;
	}

	try
	{
		//判断设备是否停止采集
		if (m_bIsSnap)
		{
			ObjStreamPtr->StopGrab();
			m_bIsSnap = false;
		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}

	try
	{
		//判断设备是否打开
		if (m_bIsOpened)
		{
			//注销掉线回调函数
			cout << "<Unregister device Offline callback>" << endl;
			ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);

			//关闭流和设备
			cout << "<Close Device>" << endl;
			ObjStreamPtr->Close();
			ObjDevicePtr->Close();
			m_bIsOpened = false;

		}
	}
	catch (CGalaxyException)
	{
		//do noting
	}
	catch (std::exception)
	{
		//do noting
	}
}

void SingleCam::__InitParam()
{
	//设置采集模式为连续模式

	ObjFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
	////是否支持触发模式选择
	m_bTriggerMode = ObjFeatureControlPtr->IsImplemented("TriggerMode");
	if (m_bTriggerMode)
	{
		ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
	}

	//是否支持触发源选择
	m_bTriggerSource = ObjFeatureControlPtr->IsImplemented("TriggerSource");

	//获取曝光时间、增益及自动白平衡系数的最大值和最小值
	m_dGainValueMin = ObjFeatureControlPtr->GetFloatFeature("Gain")->GetMin();
	m_dGainValueMax = ObjFeatureControlPtr->GetFloatFeature("Gain")->GetMax();
	m_dShutterValueMin = ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMin();
	m_dShutterValueMax = ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetMax();
	//获取相机当前曝光时间
	m_dcurCamShutterTime = ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();
}

void PicConInit(HWND hWnd_dlg, int IDD, const char* windowname)
{
	cv::namedWindow(windowname, cv::WINDOW_AUTOSIZE);
	HWND hWnd_pic = GetDlgItem(hWnd_dlg, IDD);

	//设定这玩意的硬性大小
	CRect rect;
	GetWindowRect(hWnd_pic, rect);
	MoveWindow(hWnd_pic, rect.left, rect.top,
		PC_WIDTH / AS_RATIO, PC_HEIGHT / AS_RATIO, true);

	HWND hWnd = (HWND)cvGetWindowHandle(windowname);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, hWnd_pic);
	::ShowWindow(hParent, SW_HIDE);
}

int StartCam(std::shared_ptr<SingleCam> p)
{
	/*if (SCV().empty())
	{
		AfxMessageBox(CString(" StartCam01 scv empty!"));
		return -1;
	}
	auto s1 = SCV().back();*/

	//将ROI_1的指针指向了相机里面的ROI

	p->Record_start();
	return 0;
}

int StopCam(std::shared_ptr<SingleCam> p)
{
	/*if (SCV().empty())
	{
		AfxMessageBox(CString("  StopCam01 scv empty!"));
		return -1;
	}*/
	//auto s1 = SCV().back();

	p->Record_stop();
	return 0;
}

void NormalROI(Rect& input)
{
	if (input.x < 0)
		input.x = 0;
	else if (input.x > PC_WIDTH)
		input.x = PC_WIDTH;
	else if (input.y < 0)
		input.y = 0;
	else if (input.y > PC_HEIGHT)
		input.y = PC_HEIGHT;
}


//设备掉线事件处理器
void CSampleDeviceOfflineEventHandler::DoOnDeviceOfflineEvent(void* pUserParam)
{
	SingleCam* Cam = static_cast<SingleCam*>(pUserParam);
	Cam->IsOffline() = true;
}

void CSampleCaptureEventHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	SingleCam* Cam = static_cast<SingleCam*>(pUserParam);
	if (Cam->IsOffline())
	{
		//掉线处理
		//重连
	}
	else if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_INCOMPLETE)
	{
		Cam->SendSoftwareTrigger();
		return;
	}
	else if (objImageDataPointer->GetStatus() == GX_FRAME_STATUS_SUCCESS)
	{
		//继续采集
		Cam->src.create(objImageDataPointer->GetHeight(), objImageDataPointer->GetWidth(), CV_8UC3);
		void* p_gray = NULL;

		//p_gray = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_ADAPTIVE, true);
		p_gray = objImageDataPointer->GetBuffer();
		Cam->src.create(Size(objImageDataPointer->GetWidth(), objImageDataPointer->GetHeight()), CV_8UC1);
		memcpy(Cam->src.data, p_gray, (objImageDataPointer->GetHeight()) * (objImageDataPointer->GetWidth()));
		string name = DATA_FOLDER + Cam->GetShowWindow() + string(".bmp");
		cv::imwrite(name, Cam->src);
		Cam->m_frame_ready_ = true;

		////继续采集
		//Cam->src.create(objImageDataPointer->GetHeight(), objImageDataPointer->GetWidth(), CV_8UC1);
		//void* pRGB24Buffer = NULL;
		////假设原始数据是BayerRG8图像
		////pRGB24Buffer = objImageDataPointer->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, true);
		//memcpy(Cam->src.data, pRGB24Buffer, (objImageDataPointer->GetHeight()) * (objImageDataPointer->GetWidth()) * 3);
		//
		////取消三通道转单通道
		////cvtColor(Cam->src, Cam->src, COLOR_BGR2GRAY);
		////cv::flip(Cam->src, Cam->src, ROTATE_90_CLOCKWISE);

		////Mat img;
		////resize(Cam->src, img, Size(Cam->src.cols / AS_RATIO, Cam->src.rows / AS_RATIO));

		////cv::Rect& roi = Cam->ROI;//我们直接把类带进来，得劲，与其放一个成员指针，何不直接来整个类？

		////cv::Rect roi_resized(roi.x / 10, roi.y / 10, roi.width / 10, roi.height / 10);

		////rectangle(img, roi_resized, Scalar(0, 0, 255), 8);
		//cv::imwrite(Cam->GetShowWindow() + string(".bmp"), Cam->src);
		//cv::waitKey(1);
		////帧获取完毕
		//Cam->m_frame_ready_ = true;
		////cout << "帧数：" << objImageDataPointer->GetFrameID() << endl;
	}


}
