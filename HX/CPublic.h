#pragma once
#include <string>
#include "CSerialPort/SerialPort.h"
#include "CSerialPort/SerialPortInfo.h"



//����׿��CPublic
#include "opencv_include.h"
#include "STL_include.h"
using namespace std;

using namespace itas109;

//����׿��CPublic
//���������������
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
	


	//�ڴ�й©
	//static CSerialPort m_SerialPort;//About CSerialPort
public:
	////����CRC16��ȫ�ֺ���
	//static unsigned short CRC16(unsigned char* puchMsg, unsigned short usDataLen);
	//
	////���ͺ���
	//static void SendData(int CommTypeIn, WORD DownAdd, DWORD DownData);



	//����׿��CPublic

	//����һ�������Mat��vector������
	//��һ���������ȫû���Ź���ԭͼ
	//�ڶ���λ�ô�������Ź���ԭͼ
	//��������Ų��к��ԭͼ
	enum
	{
		ORIGINAL,
		RESIZED,
		CROPPED_ORIGINAL,
		CROPPED_RESIZED,
	};


	///
	//�����ص��ַ��������ú���
	///
	//��������ʾ��������
	static constexpr char* LEFT_MATWINDOW_NAME()
	{
		return "view1";
	}
	static constexpr char* RIGHT_MATWINDOW_NAME()
	{
		return "view2";
	}

	//���������SN
	static const string& LEFT_CAM_SN()
	{

		//static const string LCSN("LT0200091520");//8��������к�
		static const string LCSN("LT0200091519");//7��������к�
		
		return LCSN;
	}
	//���������SN
	static const string& RIGHT_CAM_SN()
	{
		//static const string RCSN("WM0200120251");//8��������к�
		static const string RCSN("LT0200091521");//7��������к�
		return RCSN;
	}

	//����������Ĺٷ����ƣ�Լ��ΪСд���»��߷ָ�
	static const string& LEFT_CAM_NAME()
	{
		static const string LCN("left_cam");
		return LCN;
	}
	//����������Ĺٷ�����
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

	//�������ʹ�ܳɹ�flag
	static bool allCamsOK;
	//�������ѡʹ��flag
	static bool leftSelectionEnabled;
	//�������ѡʹ��flag
	static bool rightSelectionEnabled;



};



