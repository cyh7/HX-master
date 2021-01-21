#include "pch.h"
#include "InfoFile.h"


CInfoFile::CInfoFile()
{
}


CInfoFile::~CInfoFile()
{
}

//读取登陆信息
void CInfoFile::ReadLogin(CString &name, CString &pwd)
{
	ifstream ifs; //创建文件输入对象
	ifs.open(_F_LOGIN); //打开文件

	char buf[1024] = { 0 };

	ifs.getline(buf, sizeof(buf)); //读取一行内容
	name = CString(buf);			 //char *转换为CString

	ifs.getline(buf, sizeof(buf));
	pwd = CString(buf);

	ifs.close(); //关闭文件
}

//修改密码
void CInfoFile::WritePwd(char* name, char* pwd)
{
	ofstream ofs;	 //创建文件输出对象
	ofs.open(_F_LOGIN); //打开文件

	ofs << name << endl; //name写入文件
	ofs << pwd << endl;	//pwd写入文件

	ofs.close();	//关闭文件
}

//读取商品信息
void CInfoFile::ReadDocline(CString &type,double &x_floor, double &x_ceil, double &y_floor, double &y_ceil, double &theta_floor, double &theta_ceil)
{
	std::vector<CString> vecResult;
	std::vector<CString> strTmp;
	//msg tmp;
	CString str;//用于接收分割字符的临时变量
	CString strline;
	
	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;//定义一个CStdioFile类的对象 file
	BOOL flag = file.Open(_T(".\\stock.txt"), CFile::modeRead);//open函数需要传两个参数，前一个是文件路径，后一个是文件的打开模式
	if (flag == FALSE)
	{
		//MessageBox(_T("文件打开失败"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	//分割字符部分//

	
	int curPos = 0;
	str = vecResult[0].Tokenize(_T(" "), curPos);
	while (str.Trim() != _T(""))
	{
		strTmp.push_back(str);
		str = vecResult[0].Tokenize(_T(" "), curPos);
	}
	type = strTmp[0];
	x_floor = _wtof(strTmp[1]);
	x_ceil = _wtof(strTmp[2]);
	y_floor = _wtof(strTmp[3]);
	y_ceil = _wtof(strTmp[4]);
	theta_floor = _wtof(strTmp[5]);
	theta_ceil = _wtof(strTmp[6]);
	/*tmp.frame_length = _wtof(strTmp[7]);
	tmp.frame_width = _wtof(strTmp[8]);
	tmp.image_threshold = _wtof(strTmp[9]);*/

}

//写入文件
void CInfoFile::WirteDocline(CString &type, double &x_floor, double &x_ceil, double &y_floor, double &y_ceil, double &theta_floor, double &theta_ceil)
{
	
	

	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	//创建文件并以文本方式打开
	CStdioFile csdioFile;
	BOOL flag = csdioFile.Open(_T(".\\stock.txt"), CFile::modeReadWrite);
	//写入简体中文数据
	csdioFile.WriteString(type);
	
	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);

	/*const char* query;
	char temp[10];
	::wsprintfA(temp, "%ls", (LPCTSTR)type);
	query = temp;*/
	
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(x_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(x_ceil));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(y_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(y_ceil));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(theta_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(theta_ceil));
	csdioFile.WriteString(_T("\r\n"));
	/*MidData = SendFreqData[4];
	char MyChar[10];
	_itoa_s(MidData, MyChar, 10);*/

	csdioFile.Close();
	//char str[1024];
	//wsprintfA(str, "%ls", type);
	//
	//ofs.write(str, type.GetLength());
	////ofs <<" " << type << " ";
	//ofstream ofs(_F_STOCK);
	//ofs <<" " << x_floor << " ";
	//ofs << x_ceil << " ";
	//ofs << y_floor << " ";
	//ofs << y_ceil << " ";
	//ofs << theta_floor << " ";
	////ofs << theta_ceil << " ";
	//ofs << theta_ceil << endl;
	
	

	//ofs.close();//关闭文件
}

//添加新商品
//name:商品名称，num：库存，price：价格
//void CInfoFile::Addline(CString name, int num, int price)
//{
//	msg tmp;
//
//	if (ls.size() > 0)
//	{
//		//商品名称，库存，价格有效
//		if (!name.IsEmpty() && num > 0 && price > 0)
//		{
//			tmp.id = ls.size() + 1;	//id自动加1
//			CStringA str;
//			str = name;	//CString转CStirngA
//			tmp.name = str.GetBuffer(); //CStirngA转char *，商品名称
//			tmp.num = num;	//库存
//			tmp.price = price;	//价格
//
//			ls.push_back(tmp);	//放在链表的后面
//		}
//	}
//}








CString CInfoFile::DoubleToCString(double x)
{
	// TODO: 在此处添加实现代码.

	char temp[10];
	CString sTemp;
	_gcvt_s(temp, 10, x, 8);//浮点型转为字符串
	sTemp = CA2CT(temp);
	return sTemp;
}
