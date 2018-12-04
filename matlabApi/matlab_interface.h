//加载Matlab动态库 获取对应的函数接口
#ifndef _MATLAB_INTERFACE_H_
#define _MATLAB_INTERFACE_H_
#include <Windows.h>
#include "pub_define.h"
#include "engine.h"


typedef Engine*			(*SLApi_engOpen)(const char *startcmd);
typedef int				(*SLApi_engClose)(Engine *ep);
typedef int				(*SLApi_engEvalString)(Engine *ep, const char *strCmd);
typedef mxArray*		(*SLApi_engGetVariable)(Engine *ep, const char *szName);
typedef int				(*SLApi_engOutputBuffer)(Engine *ep, char *p, int n);
typedef int				(*SLApi_engSetVisible)(Engine *ep, bool value);

typedef double*			(*SLApi_engGetPr)(const mxArray *pm);

class CMatlab_Interface
{
public:
	CMatlab_Interface();
	~CMatlab_Interface();

	bool Initlize(const struct matlab_node* s_matlabNode);
	bool UnInitlize();

	int RegEng();
	int OpenEng();
	int ExecCmd(const char* szCmd);
	int GetValue(const char* szName, double** pValue);
	int CloseEng();

	int GetOutputBuffer(char *szOutputBuffer);
protected:
	int LoadEngDll(string strDllFile);
	int LoadMxDll(string strDllFile);
	int UnLoadDll();
	int GetApiAddress();
	void SetCurEnvPath(string strNewPath);
private:
	
private:
	bool m_bInit;
	struct matlab_node*	m_sMatlabNode;
	HINSTANCE m_hInstanceEng;
	HINSTANCE m_hInstanceMx;

	string m_strMsgInfo;

	Engine  *m_engine;
	char    m_szEngOutBuffer[1024];
	////////////////////Api Address
	SLApi_engOpen			m_api_open;
	SLApi_engClose			m_api_close;
	SLApi_engEvalString		m_api_evalstring;
	SLApi_engGetVariable	m_api_getvariable;
	SLApi_engOutputBuffer   m_api_outputBuffer;
	SLApi_engSetVisible		m_api_setvisible;
	SLApi_engGetPr			m_api_getpr;
};
#endif