#pragma once
#include <string>
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"



//复制卓承CPublic
#include "opencv_include.h"
#include "STL_include.h"
using namespace std;

using namespace itas109;

//复制卓承CPublic
//相机参数，放缩比
enum
{
	PC_WIDTH = 5496,
	PC_HEIGHT = 3672,
	AS_RATIO = 10,
};

class CPublic
{
public:
	CPublic();
	virtual ~CPublic();

public:
	


	//内存泄漏
	//static CSerialPort m_SerialPort;//About CSerialPort
public:
	////创建CRC16的全局函数
	//static unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	//
	////发送函数
	//static void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);



	//复制卓承CPublic

	//返回一个存放着Mat的vector的引用
	//第一个存放着完全没缩放过的原图
	//第二个位置存放着缩放过的原图
	//第三个存放裁切后的原图
	enum
	{
		ORIGINAL,
		RESIZED,
		CROPPED_ORIGINAL,
		CROPPED_RESIZED,
	};


	///
	//相机相关的字符串类配置函数
	///
	//返回左显示窗口名称
	static constexpr char* LEFT_MATWINDOW_NAME()
	{
		return "view1";
	}
	static constexpr char* RIGHT_MATWINDOW_NAME()
	{
		return "view2";
	}

	//返回左相机SN
	static const string& LEFT_CAM_SN()
	{
		//static const string LCSN("LT0200091520");
		static const string LCSN("LT0200091519");
		
		return LCSN;
	}
	//返回右相机SN
	static const string& RIGHT_CAM_SN()
	{
		//static const string RCSN("WM0200120251");
		static const string RCSN("LT0200091521");
		return RCSN;
	}

	//返回左相机的官方名称，约定为小写，下划线分隔
	static const string& LEFT_CAM_NAME()
	{
		static const string LCN("left_cam");
		return LCN;
	}
	//返回右相机的官方名称
	static const string& RIGHT_CAM_NAME()
	{
		static const string RCN("right_cam");
		return RCN;
	}

	static std::vector<bool>& allRuntimeFlag()
	{
		static std::vector<bool> b;
		return b;
	}

	//所有相机使能成功flag
	static bool allCamsOK;
	//左相机框选使能flag
	static bool leftSelectionEnabled;
	//右相机框选使能flag
	static bool rightSelectionEnabled;



};



