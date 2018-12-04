//应用程序调用接口

#ifndef _MATLAB_COMM_H_
#define _MATLAB_COMM_H_

#include "pub_define.h"
#include "matlab_interface.h"
#include "engine.h"

#include <QStringList>
#include <map>
#include <QObject>
#include <QProcess>

typedef Engine*			(*SLApi_engOpen)(const char *startcmd);
typedef int				(*SLApi_engClose)(Engine *ep);
typedef int				(*SLApi_engEvalString)(Engine *ep, const char *strCmd);
typedef mxArray*		(*SLApi_engGetVariable)(Engine *ep, const char *szName);
typedef int				(*SLApi_engOutputBuffer)(Engine *ep, char *p, int n);
typedef int				(*SLApi_engSetVisible)(Engine *ep, bool value);

class weMatlabComm : public QObject
{
	Q_OBJECT

public:
	weMatlabComm(QObject *parent = 0);
	~weMatlabComm();

	bool Initlize();
	bool UnInitlize();

	vector<string>* GetMatlabInfos();

	QStringList m_matlabVerLst;
	QStringList m_matlabBinPath;

	int RegEng();
	int OpenEng();
	int ExecCmd(const char* szCmd);
	int GetValue(const char* szName, double** pValue);
	int CloseEng();

	QProcess  *m_execCmdProcess;

public slots:
	QString getCmdProcessOutputs();

protected:
	int GetAllInstall();
	//eg:"SOFTWARE//MathWorks//MATLAB....", "MATROOT"
	int TraversalReg(const char* szRegPath, const char* szFieldName);

private:
	bool m_bInit;

	string m_strNewAddEnvPath;
	//本地所有Matlab版本信息,eg:  <"7.13",R2011b...>
	map<string, struct matlab_node*> m_mapMatlabInfos;
	//版本名，eg： <"7.13"> ;与map里面的顺序一致
	vector<string>	m_vecMatlabVer;

	string	m_strCurMatlabVer;	//"7.13"

	string	m_strSinFilePath;
	string  m_strOutPutDir;
	string  m_strOutPutFileName;
	string  m_strTemplMdlPath;
	string  m_strTargetVer;

	string  m_strMsgInfo;

};

#endif