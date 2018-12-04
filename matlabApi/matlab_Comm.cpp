#include "stdio.h"
#include <windows.h>
#include "matlab_Comm.h"
#include "mytools.h"
#include <process.h>
#include <stdlib.h>

#include <QSettings>
#include <QProcess>

int g_iIndex = 0;

weMatlabComm::weMatlabComm(QObject *parent)
	: QObject(parent)
{
	m_strMsgInfo = "";

	m_execCmdProcess = new QProcess();
	m_execCmdProcess->setProgram("cmd");
	m_execCmdProcess->setProcessChannelMode(QProcess::MergedChannels);
	connect(m_execCmdProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(getCmdProcessOutputs()));
}
weMatlabComm::~weMatlabComm()
{

}

bool weMatlabComm::Initlize()
{
	int count = 0;

	QSettings *setting = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "Mathworks", "MATLAB");
	QStringList matlabVer_CurUsr = setting->childGroups();
	count = matlabVer_CurUsr.size();
	if (count > 0)
	{
		for (int i = 0; i < count;i++)
		{
			m_matlabVerLst.append(matlabVer_CurUsr.at(i));
		}
	}
	delete setting;

	QSettings *setting1 = new QSettings(QSettings::NativeFormat, QSettings::SystemScope, "Mathworks", "MATLAB");
	QStringList matlabVer_LocalMachine = setting1->childGroups();
	count = matlabVer_LocalMachine.size();
	if (count > 0)
	{
		int lstsize = m_matlabVerLst.size();
		if (lstsize == 0)
		{
			for (int i = 0; i < count; i++)
			{
				m_matlabVerLst.append(matlabVer_LocalMachine.at(i));
			}
		} 
		else
		{
			for (int i = 0; i < lstsize; i++)
			{
				if (matlabVer_LocalMachine.at(i) != m_matlabVerLst.at(i))
				{
					m_matlabVerLst.append(matlabVer_LocalMachine.at(i));
				}
			}
		}
		
	}
	delete setting1;

	if (m_matlabVerLst.size() < 1)
	{
		return false;
	} 
	else
	{
		return true;
	}	
}

bool weMatlabComm::UnInitlize()
{
	
	m_mapMatlabInfos.clear();
	m_vecMatlabVer.clear();
	m_bInit = false;
	return true;
}
vector<string>* weMatlabComm::GetMatlabInfos()
{
	return &m_vecMatlabVer;
}

int weMatlabComm::RegEng()
{
	QStringList arg;
	arg << "/c" << "path";
	m_execCmdProcess->setArguments(arg);
	m_execCmdProcess->start();

	return 0;
}

int weMatlabComm::OpenEng()
{
	return 0;
}

int weMatlabComm::ExecCmd(const char * szCmd)
{
	return 0;
}

int weMatlabComm::GetValue(const char * szName, double ** pValue)
{
	return 0;
}

int weMatlabComm::CloseEng()
{
	return 0;
}

QString weMatlabComm::getCmdProcessOutputs()
{
	QByteArray qbt = m_execCmdProcess->readAllStandardOutput();
	QString msg = QString::fromLocal8Bit(qbt);
	QStringList sec = msg.split(QRegExp("[;]"));

	QString match0 = "matlab";
	QString match1 = "bin";

	for (int i = 0; i < sec.size();i++)
	{
		QString tmp = sec.at(i);
		//查找匹配 matlab 与 bin 去得出 bin 的路径
		int idx = -1;
		idx = tmp.indexOf(match0);
		if (idx > -1)
		{
			idx = -1;
			idx = tmp.indexOf(match1);
			if (idx > -1)
			{
				m_matlabBinPath << tmp; //d:\matlab2013a\bin
			}
		}
	}

	return msg;
}

int weMatlabComm::GetAllInstall()
{
	const char* lpszKey_32 = "SOFTWARE\\MathWorks\\MATLAB";
	const char* lpszKey_64 = "SOFTWARE\\Wow6432Node\\MathWorks\\MATLAB";
	m_strNewAddEnvPath = "";
	TraversalReg(lpszKey_32, "MATLABROOT");
	TraversalReg(lpszKey_64, "MATLABROOT");

	return 0;
}
// 获取注册表对应项的节点名和目录
//eg:"SOFTWARE//MathWorks//MATLAB....", "MATROOT"
int weMatlabComm::TraversalReg(const char* szRegPath, const char* szFieldName)
{
	HKEY hSubkey;
	long lresult;
	TCHAR szKeyName[256] = {0};
	DWORD cbName = 256*sizeof(TCHAR);
	DWORD dwIndex = 0;
	DWORD typeSZ = REG_SZ;
	TCHAR matlabPath[256] = {0};


	lresult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, szRegPath, 0, KEY_WOW64_64KEY|KEY_READ, &hSubkey);
	if(ERROR_SUCCESS != lresult)
	{
		RegCloseKey(hSubkey);
		return 1;
	}
	lresult = RegEnumKeyEx(hSubkey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);
	while(lresult == ERROR_SUCCESS && lresult != ERROR_MORE_DATA)
	{
		dwIndex ++;
		cbName = 256 * sizeof(TCHAR);
		HKEY hItem;
		if(::RegOpenKeyEx(hSubkey, szKeyName, 0, KEY_READ, &hItem) == ERROR_SUCCESS)
		{
			if(::RegQueryValueExA(hItem, szFieldName, 0, &typeSZ, (LPBYTE)matlabPath, &cbName) == ERROR_SUCCESS)
			{
				
			}
		}
		lresult = RegEnumKeyEx(hSubkey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);
	}
	RegCloseKey(hSubkey);
	return 0;
}