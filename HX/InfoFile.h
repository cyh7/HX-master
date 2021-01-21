#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define _F_LOGIN "./login.ini"
#define _F_STOCK "./stock.txt"

using namespace std;

struct msg
{
	CString backboard; //背板型号
	double x_floor; //x下限
	double x_ceil; //x上限
	double y_floor; //x下限
	double y_ceil; //x上限
	double theta_floor; //角度下限
	double theta_ceil; //角度上限
	/*int frame_length;
	int frame_width;
	int image_threshold;*/

	//int id;				//商品id
	//string name;	//商品名
	//int price;			//商品价格
	//int num;			//商品个数
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//读取登陆信息
	void ReadLogin(CString &name, CString &pwd);

	//修改密码
	void WritePwd(char* name, char* pwd);

	// 读取商品数据
	void ReadDocline(CString &type, double &x_floor, double &x_ceil, double &y_floor, double &y_ceil, double &theta_floor, double &theta_ceil);

	//商品写入文件
	void WirteDocline(CString &type, double &x_floor, double &x_ceil, double &y_floor, double &y_ceil, double &theta_floor, double &theta_ceil);

	//添加新商品
	//void Addline(CString name, int num, int price);

	list<msg> ls;	//存储商品容器
	//int num;			//用来记录商品个数
	
	
	CString DoubleToCString(double x);
};



