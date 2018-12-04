//公共定义

#ifndef _PUB_DEFINE_H_
#define _PUB_DEFINE_H_
#include <string>
#include <vector>
#include <map>

using namespace std;

#define MSG_PROGRESS_INFO    1001

//matlab节点定义
struct matlab_node 
{
	string strRegName;	//注册表名
	string strRegPath;	//注册表该节点目录
	string strAlias;	//别名
	string strlibengPath;	//libeng 库全路径
	string strlibmxPath;	//libmx 库全路径
	string strlibRootDir;	//..\R2011b
	string strlibBinDir;	//..\R2011b\bin
	string strlibWinDir;	//..\R2011b\bin\Win64
	int	   nBit;		//bit: 32; 64
};




#endif