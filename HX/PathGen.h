#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>

//默认路径文件名称
const std::string HORI_FILE_NAME = "hori_path.cof";
const std::string VERT_FILE_NAME = "vert_path.cof";
const std::string BP_POS_FILE_NAME = "bp_pos.cof";
const std::string OUTPUT_FILE_NAME = "path.txt";
/*
* 路径生成类的作用
* 1. 读取可用路径文件，共包含两个文件，一个是横路径文件， 一个是竖路径文件，这两个路径都是point4w格式
* 2. 生成实际路径，以point4w数组形式规划
* 3. 补全空路径，打印成txt格式
*/

//定义特殊结构体形式

struct point4w
{
	int sx;
	int sy;
	int ex;
	int ey;
	point4w(const int& isx = 0, const int& isy = 0, const int& iex = 0, const int& iey = 0) : sx(isx), sy(isy), ex(iex), ey(iey)
	{}
};
struct back_plate_info
{
	/* data */
	double min_width;
	double min_height;
	double e_width;
	double e_height;
	double i_width;
	double i_height;
};
struct actual_path
{
	std::vector<point4w> real_path;
	unsigned int len;
	unsigned int width;
	unsigned int height;
	int rsx; // relative start.x
	int rsy;  // relative start,y
};

class PathGen
{
public:
	PathGen(const std::string& hori_file_name = HORI_FILE_NAME,
		const std::string& vert_file_name = VERT_FILE_NAME,
		const std::string& bp_pos_file_name = BP_POS_FILE_NAME)
		: hori_file_name_(hori_file_name), vert_file_name_(vert_file_name), bp_pos_file_name_(bp_pos_file_name)
	{
	}
	//读取文件函数
	bool ReadAllFiles();
	//提炼路径函数
	bool RefinePaths();
	//生成路径函数
	void GenerateActualPaths(actual_path& ap);
	~PathGen() {}
private:
	//未排序路径
	std::string hori_file_name_;//横路径文件名, 该文件为二进制文件
	std::string vert_file_name_;//竖路径文件名，该文件为二进制文件
	std::string bp_pos_file_name_;//背板大小文件名，该文件为二进制文件
	//反转路径组，读取一个vector<point4w>并进行反转
	void ReversePathGroup(std::vector<point4w>& input);
	//填充数值路径组
	void FillVerticalGroup();


	//横路径数组
	std::vector<point4w> hori_paths_;
	std::vector<std::vector<point4w>> refined_hori_paths_;
	//竖路径数组
	std::vector<point4w> vert_paths_;
	std::vector<std::vector<point4w>> refined_vert_paths_;
	//排序好的路径数组
	std::vector<point4w> final_paths_;

public:
	//背板信息
	static back_plate_info bp_info;
};
