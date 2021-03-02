#include "PathGen.h"


using namespace std;
namespace std
{
	ifstream& operator>>(ifstream& fin, point4w& output)
	{

		fin >> output.sx >> output.sy >> output.ex >> output.ey;
		return fin;
	}
}

bool PathGen::ReadAllFiles()
{
	//读取背板文件
	FILE* bp_pos_fp = NULL;
	bp_pos_fp = fopen(bp_pos_file_name_.c_str(), "rb");
	if (bp_pos_fp == NULL)
	{
		//读取失败！
		cout << "read failed !!!!" << endl;
		return false;
	}
	fread(&bp_info, sizeof(back_plate_info), 1, bp_pos_fp);
#ifdef _DEBUG
	cout << "width " << bp_info.width << " height " << bp_info.height << "min_width" << bp_info.min_width << "min_height" << bp_info.height << endl;
#endif // _DEBUG
	fclose(bp_pos_fp);
	//读入横向数组
	//初始化横向数组
	hori_paths_ = vector<point4w>();
	FILE* fp_hori = NULL;
	fp_hori = fopen(hori_file_name_.c_str(), "rb");
	if (fp_hori != NULL)
	{
		fseek(fp_hori, 0, SEEK_END); // 寻位到结尾
		std::size_t filesize = std::ftell(fp_hori) / sizeof(point4w);
		fseek(fp_hori, 0, SEEK_SET); // 寻位到起始
		for (int i = 0; i < filesize; ++i)
		{
			point4w tmp;
			fread(&tmp, sizeof(point4w), 1, fp_hori);
			hori_paths_.push_back(tmp);
#ifdef _DEBUG
			cout << tmp.sx << " " << tmp.sy << " " << tmp.ex << " " << tmp.ey << " " << endl;
#endif // DEBUG
		}
		fclose(fp_hori);
	}
	cout << endl;

	FILE* fp_vert = NULL;
	fp_vert = fopen(vert_file_name_.c_str(), "rb");
	if (fp_vert != NULL)
	{
		fseek(fp_vert, 0, SEEK_END); // 寻位到结尾
		std::size_t filesize = std::ftell(fp_vert) / sizeof(point4w);
		fseek(fp_vert, 0, SEEK_SET); // 寻位到起始
		for (int i = 0; i < filesize; ++i)
		{
			point4w tmp;
			fread(&tmp, sizeof(point4w), 1, fp_vert);
			vert_paths_.push_back(tmp);
#ifdef _DEBUG
			cout << tmp.sx << " " << tmp.sy << " " << tmp.ex << " " << tmp.ey << " " << endl;
#endif // DEBUG
		}
		fclose(fp_vert);
	}

	return true;
}

bool PathGen::RefinePaths()
{
	//提炼横路径
	if (!hori_paths_.empty())
	{
		bool is_same_line = true;
		int previous_y = -1;
		vector<point4w> line_group_buf;
		for (const auto& line : hori_paths_)
		{
			if (line.ey != previous_y)
			{
				if (previous_y != -1)
				{
					refined_hori_paths_.push_back(line_group_buf);
					line_group_buf.clear();
				}
			}
			line_group_buf.push_back(line);
			previous_y = line.ey;
		}
		refined_hori_paths_.push_back(line_group_buf);
	}
	if (!vert_paths_.empty())
	{
		bool is_same_line = true;
		int previous_x = -1;
		vector<point4w> line_group_buf;
		for (const auto& line : vert_paths_)
		{
			if (line.ex != previous_x)
			{
				if (previous_x != -1)
				{
					refined_vert_paths_.push_back(line_group_buf);
					line_group_buf.clear();
				}
			}
			line_group_buf.push_back(line);
			previous_x = line.ex;
		}
		refined_vert_paths_.push_back(line_group_buf);
	}
#ifdef _DEBUG
	for (const auto& linebuf : refined_hori_paths_)
	{
		for (const auto& line : linebuf)
		{
			cout << "sx: " << line.sx << " sy: " << line.sy << " ex: " << line.ex << " ey: " << line.ey << endl;
		}
		cout << endl;
	}
	cout << endl;
	for (const auto& linebuf : refined_vert_paths_)
	{
		for (const auto& line : linebuf)
		{
			cout << "sx: " << line.sx << " sy: " << line.sy << " ex: " << line.ex << " ey: " << line.ey << endl;
		}
		cout << endl;
	}
#endif // DEBUG

	return false;
}
//生成实际路径
void PathGen::GenerateActualPaths(actual_path& ap)
{
	if (refined_hori_paths_.size() == 2)//上下路径皆存在
	{
		//定义下方路径引用
		const vector<point4w>& bottom_line = refined_hori_paths_.front();
		copy(bottom_line.cbegin(), bottom_line.cend(), back_inserter(final_paths_));
		FillVerticalGroup();
		const vector<point4w>& top_line = refined_hori_paths_.back();
		copy(top_line.cbegin(), top_line.cend(), back_inserter(final_paths_));
	}
	else if (refined_hori_paths_.size() == 1)
	{
		vector<point4w>& single_line = refined_hori_paths_.front();
		if (single_line.front().ey < vert_paths_.front().ey) //说明是底线
		{
			copy(single_line.cbegin(), single_line.cend(), back_inserter(final_paths_));
			FillVerticalGroup();
		}
		else//说明是顶线
		{
			FillVerticalGroup();
			ReversePathGroup(single_line);
			copy(single_line.cbegin(), single_line.cend(), back_inserter(final_paths_));
		}
	}
	else if (refined_hori_paths_.empty())//无横线
	{
		FillVerticalGroup();
	}

	if (final_paths_.empty())
	{
		//出错了
		cout << "No Paths were generated!" << endl;
	}
	else
	{
		//修改此处位置为以左上角为原点
		const int file_rsx = bp_info.min_width;
		const int file_rsy = bp_info.min_height + bp_info.height;
		//打开文件
		FILE* fp_output = NULL;
		fp_output = fopen(OUTPUT_FILE_NAME.c_str(), "w+");
		if (fp_output == NULL)
		{
			cout << "error, cant write to disk!" << endl;
			return;
		}
		ap.real_path.clear();
		//补个起点
		std::fprintf(fp_output, "%d %d %d\n", -(final_paths_[0].sy - file_rsy), (final_paths_[0].sx - file_rsx), 0);
		for (int i = 0; i < final_paths_.size(); ++i)
		{
#ifdef  _DEBUG
			std::fprintf(fp_output, "%d %d %d\n", -(final_paths_[i].ey - file_rsy), (final_paths_[i].ex - file_rsx), 1);
			ap.real_path.push_back(final_paths_[i]);
			const int next_index = (i + 1 == final_paths_.size()) ? 0 : i + 1;

			if (next_index)
			{
				//空路径
				//首尾不相连时，输出空路径
				if (final_paths_[i].ex != final_paths_[next_index].sx || final_paths_[i].ey != final_paths_[next_index].sy)
					std::fprintf(fp_output, "%d %d %d\n", -(final_paths_[next_index].sy - file_rsy), (final_paths_[next_index].sx - file_rsx), 0);

				ap.real_path.emplace_back(final_paths_[i].ex, final_paths_[i].ey, final_paths_[next_index].sx, final_paths_[next_index].sy);

			}
#endif //  _DEBUG

#ifdef _RELEASE
			//实际路径
			//坐标系转换
			fprintf(fp_output, "%d %d %d\n", fabs(final_paths_[i].ey - file_rsy), final_paths_[i].ex - file_rsx, 1);
			ap.real_path.push_back(final_paths_[i]);
			const int next_index = (i + 1 == final_paths_.size()) ? 0 : i + 1;

			//空路径
			//首尾不相连时，输出空路径
			if (final_paths_[i].ex != final_paths_[next_index].sx || final_paths_[i].ey != final_paths_[next_index].sy)
				fprintf(fp_output, "%d %d %d\n", fabs(final_paths_[next_index].sy - file_rsy), (final_paths_[next_index].sx - file_rsx), 0);

			ap.real_path.emplace_back(final_paths_[i].ex, final_paths_[i].ey, final_paths_[next_index].sx, final_paths_[next_index].sy);

#endif // _RELEASE

		}
		fclose(fp_output);
		//actual_path 赋值
		ap.rsx = bp_info.min_width;
		ap.rsy = bp_info.min_height;
		ap.len = ap.real_path.size();
		ap.height = bp_info.height;
		ap.width = bp_info.width;

	}

}

void PathGen::ReversePathGroup(vector<point4w>& input)
{
	//反转整个组顺序
	reverse(input.begin(), input.end());
	//反转每根线的起点和终点
	for (auto& line : input)
	{

		swap(line.sx, line.ex);
		swap(line.sy, line.ey);
	}
}

void PathGen::FillVerticalGroup()
{
	if (!final_paths_.empty())
	{
		//如果之前有路径输入，说明存在横直线组
		reverse(refined_vert_paths_.begin(), refined_vert_paths_.end());
	}
	for (int i = 0; i < refined_vert_paths_.size(); ++i)
	{
		if (i % 2 == 0)
		{
			//不需要反向
			copy(refined_vert_paths_[i].cbegin(), refined_vert_paths_[i].cend(), back_inserter(final_paths_));
		}
		else
		{
			//需要反向，总体顺序反向，然后将所有路径的起点和终点对调
			ReversePathGroup(refined_vert_paths_[i]);
			copy(refined_vert_paths_[i].cbegin(), refined_vert_paths_[i].cend(), back_inserter(final_paths_));
		}

	}
}
