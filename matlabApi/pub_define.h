//��������

#ifndef _PUB_DEFINE_H_
#define _PUB_DEFINE_H_
#include <string>
#include <vector>
#include <map>

using namespace std;

#define MSG_PROGRESS_INFO    1001

//matlab�ڵ㶨��
struct matlab_node 
{
	string strRegName;	//ע�����
	string strRegPath;	//ע���ýڵ�Ŀ¼
	string strAlias;	//����
	string strlibengPath;	//libeng ��ȫ·��
	string strlibmxPath;	//libmx ��ȫ·��
	string strlibRootDir;	//..\R2011b
	string strlibBinDir;	//..\R2011b\bin
	string strlibWinDir;	//..\R2011b\bin\Win64
	int	   nBit;		//bit: 32; 64
};




#endif