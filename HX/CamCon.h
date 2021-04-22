#pragma once

#include "pch.h"
#include "opencv_include.h"
#include "CPublic.h"

using namespace std;
using namespace cv;


//初始化PitureControl控件
void PicConInit(HWND hWnd, int IDD, const char* windowname);



class CSampleDeviceOfflineEventHandler : public IDeviceOfflineEventHandler
{
public:

	void DoOnDeviceOfflineEvent(void* pUserParam);
};

//用户继承属性更新事件处理类

class CSampleFeatureEventHandler : public IFeatureEventHandler
{
public:
	void DoOnFeatureEvent(const GxIAPICPP::gxstring& strFeatureName, void* pUserParam)
	{
		cout << "收到曝光结束事件!" << endl;
	}
};

//用户继承采集事件处理类
class CSampleCaptureEventHandler : public ICaptureEventHandler
{
public:

	void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);

};
class SingleCam;


/// <summary>
/// Cam总线保留，将它配合上复位功能，把复位功能做进来
/// 放入两个相机
/// </summary>
class CamCon //Cam总线
{
public:

	enum {
		LEFT,
		RIGHT,
	};


	CamCon() { CamInit(); };
	~CamCon() { CamUnInit(); };
	int init_all_cam(vector<shared_ptr<SingleCam>>& cam_vec);
	//初始化函数
	int CamInit();
	int CamUnInit();

	//扫描相机函数
	size_t ScanCam();
	//复位相机函数
	void SoftResetAllCam();

	static shared_ptr<SingleCam> LEFT_CAM;
	static shared_ptr<SingleCam> RIGHT_CAM;

public:

private:
	//禁止复制
	CamCon(const CamCon&);
	CamCon operator=(const CamCon&);

	gxdeviceinfo_vector vectorDeviceInfo;//整个总线包含的相机信息

	vector<gxstring> macAddrList;

};
class SingleCam//独立相机
{
	friend class CvisonDlg;
	friend class CSampleCaptureEventHandler;
	friend class CamCon;
public:

	SingleCam(gxstring m_serialNum, const char* windowname, const cv::Rect& roi);
	~SingleCam();
public:

	//设置相机为连续模式
	void SetAcqusition_Continuous();
	//设置相机为软触发模式
	void SetAcqusition_SoftwareTrigger();

	//发送软触发指令
	void SendSoftwareTrigger();


	void Record_start();
	void Record_stop();
	cv::Rect ROI;

	inline bool& IsOffline()
	{
		return m_bIsOffline;
	}

	gxstring GetSN() const
	{
		return serialNum;
	}
	string GetShowWindow() const
	{
		return showWindowName;
	}
	Mat GetSrc() const
	{
		return src;
	}
	double GetShutterTime() const
	{
		return m_dcurCamShutterTime;
	}
public:
	//标志位
	bool m_bIsOffline;     //是否离线
	bool m_bIsSnap;        //是否在采集
	bool m_bIsOpened;      //是否已经打开
	bool m_bTriggerMode;   //是否支持触发
	bool m_bTriggerSource; //是否支持选择触发源
	bool m_frame_ready_;//帧拍摄完毕
	//设备参数，比如增益最大值，曝光时间范围等
	double m_dShutterValueMax;//快门
	double m_dShutterValueMin;
	double m_dGainValueMax;//增益
	double m_dGainValueMin;

	CGXFeatureControlPointer   ObjFeatureControlPtr;//设备属性控制器
	CGXFeatureControlPointer   ObjStreamFeatureControlPtr;//流层控制器对象
private:
	//禁止复制相机
	SingleCam(const SingleCam&);
	const SingleCam& operator=(const SingleCam&);
	//掉线处理
	void __ProcessOffline();
	//重连函数
	void __Recovery();
	//初始化相机参数
	void __InitParam();
	//各种Handler
	IDeviceOfflineEventHandler* pDeviceOfflineEventHandler;///<掉线事件回调对象
	IFeatureEventHandler* pFeatureEventHandler;///<远端设备事件回调对象
	ICaptureEventHandler* pCaptureEventHandler;///<采集回调对象


	CGXDevicePointer ObjDevicePtr;//设备指针
	CGXStreamPointer ObjStreamPtr;//打开流

	GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline; //掉线回调句柄


	//相机参数，暂时设置两项
	double m_dcurCamGain;       //相机当前增益
	double m_dcurCamShutterTime;//相机当前快门时间


	//序列号
	gxstring serialNum;
	//画面显示窗口名称
	string showWindowName;

	//源图
	Mat src;
	//裁切后的原图像
	Mat croppedSrc;
	//缩小后的原图
	Mat resizedSrc;
	//缩小后的裁切图
	Mat resizedCropped;

};



//之后双相机会改的代码
int StartCam(std::shared_ptr<SingleCam> p);
int StopCam(std::shared_ptr<SingleCam> p);

static CamCon& CAMVEC()
{
	static CamCon c1;
	return c1;
}
//single_cam vector
static vector<shared_ptr<SingleCam>>& SCV()
{
	static vector<shared_ptr<SingleCam>> c1;
	return c1;
}
