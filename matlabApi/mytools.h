//π§æﬂ¿‡
#ifndef _MYTOOLS_H_
#define _MYTOOLS_H_
#include <string>
#include <vector>

using namespace  std;

class CMyTools
{
public:
	static bool IsFileExist(const char* szFilePath);
	static void strToVector(const char *src, const char *split, vector<std::string> & vecDest);
};

#endif