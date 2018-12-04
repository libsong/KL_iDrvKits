#include "stdafx.h"
#include "matlab_interface.h"


extern void OutPrint(const char* szMsgInfo);

CMatlab_Interface::CMatlab_Interface()
{
	m_bInit = false;
	m_strMsgInfo = "";
	m_hInstanceEng = NULL;
	m_hInstanceMx = NULL;

	m_api_open = NULL;
	m_api_close = NULL;
	m_api_evalstring = NULL;
	m_api_getvariable = NULL;
	m_api_outputBuffer = NULL;
	m_api_setvisible = NULL;
	m_api_getpr = NULL;
	
}
CMatlab_Interface::~CMatlab_Interface()
{
	UnInitlize();
}
int CMatlab_Interface::CloseEng()
{
	int nRetCode = 0;
	if (!m_bInit)
	{
		return 1;
	}
	if(m_engine)
	{
		if(m_api_outputBuffer)
		{
			m_api_outputBuffer(m_engine, NULL, 0);
			memset(m_szEngOutBuffer, 0, 1024);
		}

		nRetCode = m_api_close(m_engine);
		if(nRetCode != 0)
		{
			m_strMsgInfo = "CloseEng : m_api_close fail !";
			OutPrint(m_strMsgInfo.c_str());
			return 2;
		}
	}
	m_engine = NULL;
	return 0;
}

int CMatlab_Interface::RegEng()
{
	char szBuffer[256] = {0};
	sprintf(szBuffer, "matlab /regserver");
	{
		PROCESS_INFORMATION processInfo;
		BOOL bRet = FALSE;
		STARTUPINFO startupInfo;

		ZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		ZeroMemory(&processInfo, sizeof(processInfo));

		bRet = CreateProcess(NULL,
			szBuffer,
			NULL,
			NULL,
			FALSE,
			CREATE_NO_WINDOW,
			NULL,
			NULL,
			&startupInfo,
			&processInfo);
		if(bRet == 0)
		{
			m_strMsgInfo = "RegEng : CreateProcess fail !";
			OutPrint(m_strMsgInfo.c_str());
			return 1;
		}
		Sleep(1000*5);
	}	
	return 0;
}
int CMatlab_Interface::OpenEng()
{
	if (!m_bInit)
	{
		return 1;
	}
	if(m_api_open == NULL)
	{
		return 2;
	}

	m_engine = m_api_open(NULL);
	if(m_engine == NULL)
	{
		m_strMsgInfo = "OpenEng : m_api_open fail !";
		OutPrint(m_strMsgInfo.c_str());
		return 3;
	}
	if(m_api_outputBuffer)
	{
		memset(m_szEngOutBuffer, 0, 1024);
		m_api_outputBuffer(m_engine, m_szEngOutBuffer, 1024);
	}
	if(m_api_setvisible)
	{
		m_api_setvisible(m_engine, 0);
	}
	return 0;
}
int CMatlab_Interface::GetOutputBuffer(char *szOutputBuffer)
{
	if ( szOutputBuffer && strlen(m_szEngOutBuffer) > 0)
	{
		strcpy(szOutputBuffer, m_szEngOutBuffer);
	}
	return 0;
}
int CMatlab_Interface::ExecCmd(const char* szCmd)
{
	if (!m_bInit)
	{
		return 1;
	}
	if(m_engine == NULL)
	{
		m_strMsgInfo = "ExecCmd : m_api_open is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 2;
	}
	if(m_api_evalstring == NULL)
	{
		m_strMsgInfo = "ExecCmd : m_api_evalstring is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 3;
	}

	if(m_api_evalstring(m_engine, szCmd) != 0)
	{
		m_strMsgInfo = "ExecCmd : m_api_evalstring fail !";
		OutPrint(m_strMsgInfo.c_str());
		return 4;
	}
	
	return 0;
}
int CMatlab_Interface::GetValue(const char* szName, double** pValue)
{
	mxArray *mxD = NULL;

	if (!m_bInit)
	{
		return 1;
	}
	if(m_engine == NULL)
	{
		m_strMsgInfo = "GetValue : m_api_open is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 2;
	}
	if(m_api_getvariable == NULL)
	{
		m_strMsgInfo = "GetValue : m_api_getvariable is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 3;
	}
 
	if(m_api_getpr == NULL)
	{
		m_strMsgInfo = "GetValue : m_api_getpr is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 4;
	}

	mxD = m_api_getvariable(m_engine, szName);
	if(mxD == NULL)
	{
		m_strMsgInfo = "GetValue : m_api_getvariable is null !";
		OutPrint(m_strMsgInfo.c_str());
		return 5;
	}
	*pValue = m_api_getpr(mxD);

	return 0;
}
char g_szEnv_Path[10240] = {0};
void CMatlab_Interface::SetCurEnvPath(string strNewPath)
{
	
	char szEnv_NewPath[2048] = {0};

	if(strlen(g_szEnv_Path) < 1)
	{
		strcpy(g_szEnv_Path, getenv("PATH"));
	}

	if(strlen(g_szEnv_Path) > 1)
	{
		strcpy(szEnv_NewPath, "PATH=");
		strcat(szEnv_NewPath, strNewPath.c_str());
		strcat(szEnv_NewPath, g_szEnv_Path);
		if(_putenv(szEnv_NewPath) != 0)
		{
			OutPrint(" _putenv fail! ");
		}
	}
}
bool CMatlab_Interface::Initlize(const struct matlab_node* s_matlabNode)
{
	string strNewEnvPath("");
	m_bInit = false;
	
	if(m_engine != NULL)
	{
		if(m_api_close)
		{
			m_api_close(m_engine);
		}
	}
	m_engine = NULL;
	UnInitlize();

	//1 检查有效性
	if(!s_matlabNode)
	{
		m_strMsgInfo = "Initlize : s_matlabNode is NULL!";
		OutPrint(m_strMsgInfo.c_str());
		return false;
	}
	m_sMatlabNode = (struct matlab_node*)s_matlabNode;
	static int sbFirstSet = 0;
	if(sbFirstSet < 1)
	{
		//2 设置Env path
		strNewEnvPath = m_sMatlabNode->strlibRootDir + ";" + m_sMatlabNode->strlibBinDir + ";" + m_sMatlabNode->strlibWinDir + ";";
		SetCurEnvPath(strNewEnvPath);

		RegEng();

		sbFirstSet++;
	}
	
	//3
	if(GetApiAddress() != 0)
	{
		m_strMsgInfo = "Initlize : GetApiAddress is NULL!";
		OutPrint(m_strMsgInfo.c_str());
		return false;
	}
	m_bInit = true;
	return true;
}
bool CMatlab_Interface::UnInitlize()
{
	UnLoadDll();
	return true;
}


int CMatlab_Interface::LoadEngDll(string strDllFile)
{
	int nLastError = 0;
	nLastError = GetLastError();
	m_hInstanceEng = LoadLibrary(strDllFile.c_str());
//	m_hInstanceEng = LoadLibrary("C:\\ProgramFiles\\MATLAB\\R2011b\\bin\\win64\\libeng.dll");
	nLastError = GetLastError();
	if(m_hInstanceEng == NULL)
	{
		m_strMsgInfo = "Fail : LoadEngDll ";
		OutPrint(m_strMsgInfo.c_str());
		return 1;
	}
	return 0;
}
int CMatlab_Interface::LoadMxDll(string strDllFile)
{
	m_hInstanceMx = LoadLibrary(strDllFile.c_str());
	if(m_hInstanceMx == NULL)
	{
		m_strMsgInfo = "Fail : LoadMxDll ";
		OutPrint(m_strMsgInfo.c_str());
		return 1;
	}
	return 0;
}
int CMatlab_Interface::UnLoadDll()
{
	if(m_hInstanceEng != NULL)
	{
		FreeLibrary(m_hInstanceEng);
	}
	if(m_hInstanceMx != NULL)
	{
		FreeLibrary(m_hInstanceMx);
	}

	m_hInstanceEng = NULL;
	m_hInstanceMx = NULL;

	m_api_open = NULL;
	m_api_close = NULL;
	m_api_evalstring = NULL;
	m_api_getvariable = NULL;
	m_api_outputBuffer = NULL;
	m_api_setvisible = NULL;
	m_api_getpr = NULL;

	m_bInit = false;
	return 0;
}
int CMatlab_Interface::GetApiAddress()
{
	int nRetCode = 0;
	int err1 = 0;
	err1 = GetLastError();
	
	do 
	{
		if(LoadEngDll(m_sMatlabNode->strlibengPath) != 0)
		{
			err1 = GetLastError();
			m_strMsgInfo = "加载动态库失败：";
			m_strMsgInfo += m_sMatlabNode->strlibengPath;
			OutPrint(m_strMsgInfo.c_str());
			nRetCode = 1;
			break;
		}
	/*	if(LoadMxDll(m_sMatlabNode->strlibmxPath) != 0)
		{
			err1 = GetLastError();
			m_strMsgInfo = "加载动态库失败：";
			m_strMsgInfo += m_sMatlabNode->strlibmxPath;
			nRetCode = 2;
			break;
		}*/
		m_api_open = (SLApi_engOpen)GetProcAddress(m_hInstanceEng, "engOpen");
		if(m_api_open == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  engOpen Fail!";
			nRetCode = 3;
			break;
		}

		m_api_close = (SLApi_engClose)GetProcAddress(m_hInstanceEng, "engClose");
		if(m_api_close == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  engClose Fail!";
			nRetCode = 4;
			break;
		}
		m_api_evalstring = (SLApi_engEvalString)GetProcAddress(m_hInstanceEng, "engEvalString");
		if(m_api_evalstring == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  engEvalString Fail!";
			nRetCode = 5;
			break;
		}
		m_api_getvariable = (SLApi_engGetVariable)GetProcAddress(m_hInstanceEng, "engGetVariable");
		if(m_api_getvariable == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  engGetVariable Fail!";
			nRetCode = 6;
			break;
		}
		m_api_outputBuffer = (SLApi_engOutputBuffer)GetProcAddress(m_hInstanceEng, "engOutputBuffer");
		if(m_api_outputBuffer == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  m_api_outputBuffer Fail!";
			nRetCode = 7;
			break;
		}
		m_api_setvisible = (SLApi_engSetVisible)GetProcAddress(m_hInstanceEng, "engSetVisible");
		if(m_api_outputBuffer == NULL)
		{
			m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  m_api_setvisible Fail!";
			nRetCode = 7;
			break;
		}
		/*	m_api_getpr = (SLApi_engGetPr)GetProcAddress(m_hInstanceMx, "mxGetPr");
		if(m_api_getpr == NULL)
		{
		m_strMsgInfo = "GetApiAddress Fun: GetProcAddress  mxGetPr Fail!";
		nRetCode = 8;
		break;
		}*/
	} while (0);

	OutPrint(m_strMsgInfo.c_str());

	if (nRetCode != 0)
	{
		UnLoadDll();
	}

	return nRetCode;
}
