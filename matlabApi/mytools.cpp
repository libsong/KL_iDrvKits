#include "stdafx.h"
#include "mytools.h"
#include <Windows.h>

bool CMyTools::IsFileExist(const char* szFilePath)
{
	WIN32_FIND_DATA file;
	HANDLE hFind;
	DWORD fileSize  = 0;

	hFind = FindFirstFile(szFilePath, &file);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return true;
	}
	return false;
}
void CMyTools::strToVector(const char *src, const char *split, vector<std::string> & vecDest)
{
	vecDest.clear();
	char *str = new char[strlen(src)+1];
	memset(str, 0, strlen(src)+1);
	memcpy(str, src, strlen(src));
	char *token = strtok(str, split);
	while ( token != NULL)
	{
		vecDest.push_back(token);
		token = strtok(NULL, split);
	}
	delete []str;
}