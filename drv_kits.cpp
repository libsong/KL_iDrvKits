#include "drv_kits.h"
#include "we_types.h"
#include "SFTPClient.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <windows.h>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#define PRODUCTVERSION		"1,0"	//用于注册表中的版本记录，驱动版本
#define SIMULNKPACK			"/ks_v1"
#define SOPACK				"ks_lib_v1"
#define REGKEY				"KL DrvKits"
#define SLINKKEYNAME		"KL Simulink"
#define SLAVESOPATH			"/usr/lib"
#define KEYPATH				"HKEY_CURRENT_USER\\Software\\KL DrvKits"

#define FILEVERSION			1,0,0,0	//about点击后显示的源码文件版本
#define PVERSION			1,1	//about 点击后显示的产品版本


using namespace  GWI_SFTPClient;
#pragma comment(lib, "We_libssh2_v1.0.lib")

drv_kits::drv_kits(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	now_installStep = 0;
	installedLibPath = "";
	usrSelectPath = "";
	m_usrSltMatlabVer = "";

	ftpinf = nullptr;
	m_MatlabInf = new weMatlabComm();

	ui.label_disp->setText(weChinese2LocalCode("欢迎使用 科梁模型&驱动 安装工具集 ..."));

	//检测是否安装 matlab 以及版本
	if (!m_MatlabInf->Initlize())
	{
		QMessageBox::warning(this, tr("WARNING"), weChinese2LocalCode("未检测到安装的 Matlab 版本\n请确认后重新打开本软件！"));
		qApp->quit();
	}
	else
	{
		m_ptrVerTree = new versionTree;
		connect(m_ptrVerTree, SIGNAL(treeItemNameSend(QString)), this, SLOT(getMatlabSelectSg(QString)));

		m_ptrVerTree->makeMatlabVersionTree(matlabverAddYear(m_MatlabInf->m_matlabVerLst));
	}

	//菜单栏信号、槽
	connect(ui.action_about, SIGNAL(triggered()), this, SLOT(aboutMenuClicked()));
	connect(ui.action_usrdoc, SIGNAL(triggered()), this, SLOT(usrHelp()));
	connect(ui.action_ftp, SIGNAL(triggered()), this, SLOT(ftp()));

	//读注册表是否安装过
	//weReg = new QSettings (QSettings::NativeFormat, QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	weReg = new QSettings(QSettings::NativeFormat, QSettings::UserScope,REGKEY, SLINKKEYNAME);
	installedLibPath = weReg->value("libpath").toString();	
	if (installedLibPath != "")
	{
		QString installedLibVer = weReg->value("Version").toString();
		ui.label_disp->setText(weChinese2LocalCode("检测到 KL simulink 库已安装, 请点击卸载 ..."));
		ui.pushButton_nextstp->setText(weChinese2LocalCode("卸载"));
	} 
	else
	{
		ui.pushButton_nextstp->setText(weChinese2LocalCode("安装"));
	}
	ui.pushButton_nextstp->setEnabled(true);
	ui.pushButton_cancel->setEnabled(false);

	connect(ui.pushButton_nextstp, SIGNAL(clicked()),this,SLOT(stepChangeResponse()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));	
}

drv_kits::~drv_kits()
{
	if (m_MatlabInf != nullptr)
	{
		m_MatlabInf->UnInitlize();
		delete m_MatlabInf;
	}
}

QStringList drv_kits::matlabverAddYear(QStringList verlst)
{
	QString ver = "";
	QStringList newverLst;
	
	for (int i = 0; i < verlst.size(); i++)
	{
		ver = verlst.at(i);

		if (ver == "7.13")
		{
			newverLst << ver + " , R2011b";
		}
		else if (ver == "7.14")
		{
			newverLst << ver + " , R2012a";
		}
		else if (ver == "8.0")
		{
			newverLst << ver + " , R2012b";
		}
		else if (ver == "8.1")
		{
			newverLst << ver + " , R2013a";
		}
		else if (ver == "8.2")
		{
			newverLst << ver + " , R2013b";
		}
		else if (ver == "8.3")
		{
			newverLst << ver + " , R2014a";
		}
		else if (ver == "8.4")
		{
			newverLst << ver + " , R2014b";
		}
		else if (ver == "8.5")
		{
			newverLst << ver + " , R2015a";
		}
		else if (ver == "8.6")
		{
			newverLst << ver + " , R2015b";
		}
		else if (ver == "9.0")
		{
			newverLst << ver + " , R2016a";
		}
		else if (ver == "9.1")
		{
			newverLst << ver + " , R2016b";
		}
		else
		{
			newverLst << ver + " , Unknow";
		}
	}

	return newverLst;
}

void drv_kits::execInstall()
{
	QSleepTimeSet *wesleep = new QSleepTimeSet;
	wesleep->Delay_MSec_Suspend(2000, true);

	//写注册表
	ui.label_disp->setText(weChinese2LocalCode("KL simulink 库安装中 ..."));
	if (weReg == nullptr)
	{
		weReg = new QSettings(QSettings::NativeFormat, QSettings::UserScope, REGKEY, SLINKKEYNAME);
	}
	weReg->setValue("Name", "simulink");
	weReg->setValue("Version", PRODUCTVERSION);
	weReg->setValue("libpath", usrSelectPath);
	//weReg->setValue("matlabVer", m_usrSltMatlabVer);
	wesleep->Delay_MSec_Suspend(1000, true);

	//拷贝文件到路径下
	ui.label_disp->setText(weChinese2LocalCode("KL simulink 库安装中 ......"));	
	QString strDrvPacket = QCoreApplication::applicationDirPath() + DRVPACKETS_SIMULINKPATH;
	strDrvPacket.replace(QString("/"), QString("//"));
	bool rt = false;
	QFolderAndFilesOp wecp;
	rt = wecp.qCopyDirectory(strDrvPacket, usrSelectPath, false);
	if (!rt)
	{
		//文件拷贝失败
	}
	wesleep->Delay_MSec_Suspend(1000, true);

	ui.label_disp->setText(weChinese2LocalCode("KL simulink 库安装中 ........."));
	m_MatlabInf->RegEng();
	wesleep->Delay_MSec_Suspend(1000, true);
	QStringList strMatlabBin = m_MatlabInf->m_matlabBinPath;
	
	for (int i = 0; i < strMatlabBin.size();i++)
	{
		QString str = m_MatlabInf->m_matlabBinPath.at(i);
		int first = str.lastIndexOf("bin");
		QString strroot = str.left(str.length() -3);

		QString strroot_start = strroot + "toolbox\\local\\startup.m";
		QFile f(strroot_start);
		if (!f.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			//TODO failed
		}
		else
		{
			QString  cmdStart = "try, setup_klslink; catch, disp('Unable to setup KL simulink correctly'); end\n";
			bool isExist = false;
			QTextStream in(&f);
			while (!in.atEnd())//TODO:没有起到作用
			{
				int idx = -1;
				QString line = in.readLine();
				idx = line.indexOf("setup_klslink");
				if (idx > -1)
				{
					isExist = true;
				}
			}
			if (!isExist)
			{
				f.write(cmdStart.toUtf8());
			}			
			f.close();			
		}
		QString strroot_setup = strroot + "toolbox\\local\\setup_klslink.m";
		QFile f1(strroot_setup);
		if (!f1.open(QIODevice::WriteOnly | QIODevice::Truncate))
		{
			//TODO failed
		}
		else
		{
			QString  cmd = "addpath('";
			QString tmp = usrSelectPath + SIMULNKPACK;
			//tmp.replace(QString("/"), QString("//"));
			cmd = cmd + tmp + "');\n";
			f1.write(cmd.toUtf8());
			cmd = "savepath;\n";
			f1.write(cmd.toUtf8());
			cmd = "rehash toolboxreset;\n";
			f1.write(cmd.toUtf8());
			f1.close();
		}
	}
	wesleep->Delay_MSec_Suspend(1000, true);

	//
	ui.label_disp->setText(weChinese2LocalCode("KL simulink 库安装中 ............"));
	wesleep->Delay_MSec_Suspend(1000, true);
	ui.label_disp->setText(weChinese2LocalCode("KL simulink 库安装完成 ！"));
	ui.pushButton_nextstp->setText(weChinese2LocalCode("完成"));
	ui.pushButton_cancel->setEnabled(false);

	if (wesleep != nullptr)
	{
		delete wesleep;
	}
}

void drv_kits::aboutMenuClicked()
{
	QDialog *about = new QDialog(this);
	about->setWindowFlags(about->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	about->setAttribute(Qt::WA_DeleteOnClose, true);
	about->setFixedWidth(220);
	about->setFixedHeight(80);

	QFont font("arial", 10, 75);
	QLabel *company = new QLabel;
	company->setFont(font);
	company->setText(weChinese2LocalCode("上海科梁信息工程股份有限公司"));
	QLabel *copyright = new QLabel;
	copyright->setFont(font);
	copyright->setText(QString("< font  color=#e9a96f;>Copyright &#169; 2018</font>"));
	QLabel *version = new QLabel;

	//HRSRC hsrc = FindResource(0, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	//HGLOBAL hgbl = LoadResource(0, hsrc);
	//BYTE *pBt = (BYTE *)LockResource(hgbl);
	//VS_FIXEDFILEINFO* pFinfo = (VS_FIXEDFILEINFO*)(pBt + 40);
	QString valStr;
	valStr.sprintf(("Pv%d.%d : Fv%d.%d.%d.%d"), PVERSION, FILEVERSION);

	version->setText(valStr);

	QVBoxLayout *lyt = new QVBoxLayout;
	lyt->addWidget(company);
	lyt->addWidget(copyright);
	lyt->addWidget(version);

	about->setLayout(lyt);
	about->setWindowModality(Qt::ApplicationModal);
	about->show();
}

void drv_kits::usrHelp()
{
	QString  path = "file:" + QCoreApplication::applicationDirPath() + "/help";
	QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void drv_kits::ftp()
{
	if (ftpinf == nullptr)
	{
		ftpinf = new TargetWinInfo();
		ftpinf->setAttribute(Qt::WA_DeleteOnClose);
		connect(ftpinf, SIGNAL(emitLoginInf(QStringList)), this, SLOT(weFtp(QStringList)));
	}
	ftpinf->setWindowModality(Qt::ApplicationModal);
	ftpinf->show();
}

void drv_kits::stepChangeResponse()
{
	QString dir = "";

	if (installedLibPath != "")
	{
		ui.label_disp->setText(weChinese2LocalCode("KL simulink 库卸载中 ..."));
		ui.pushButton_nextstp->setEnabled(false);

		//卸载
		QSettings sett(KEYPATH,QSettings::NativeFormat);
		sett.remove("");

		QSleepTimeSet wesleep;
		wesleep.Delay_MSec_Suspend(2000, true);

		ui.label_disp->setText(weChinese2LocalCode("KL simulink 库卸载中 ......"));
		QFolderAndFilesOp werm;		
		werm.clearFolder(installedLibPath + SIMULNKPACK,true);
		werm.clearFolder(installedLibPath + "/" + SOPACK, true);
		wesleep.Delay_MSec_Suspend(2000, true);

		ui.label_disp->setText(weChinese2LocalCode("KL simulink 库卸载完成，程序即将关闭，请重新打开安装 ！"));
		wesleep.Delay_MSec_Suspend(5000, true);
		qApp->quit();
		ui.pushButton_nextstp->setEnabled(true);
		ui.pushButton_nextstp->setText(weChinese2LocalCode("安装"));
		ui.pushButton_nextstp->setEnabled(true);
		installedLibPath = "";
		return;
	}
	
	now_installStep++;

	switch (now_installStep)
	{	
		case 1:
			ui.label_disp->setText(weChinese2LocalCode("发现如下 MATLAB 版本"));
			ui.stackedWidget_center->addWidget(m_ptrVerTree);
			sMatlabTreeIdx = ui.stackedWidget_center->count() - 1;
			ui.stackedWidget_center->setCurrentIndex(sMatlabTreeIdx);

			ui.pushButton_nextstp->setText(weChinese2LocalCode("下一步"));
			break;

		case 2:
			ui.pushButton_cancel->setEnabled(true);
			ui.label_disp->setText(weChinese2LocalCode("请选择 KL simulink 库的安装存储路径 ..."));
			dir = QFileDialog::getExistingDirectory(this, weChinese2LocalCode("选择安装存储路径"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			if (dir.isEmpty())
			{
				this->close();
			}
			usrSelectPath = dir;
			dir = weChinese2LocalCode("KL simulink 模块库将会安装在 ： ") + dir;			
			ui.label_disp->setText(dir);

			ui.pushButton_nextstp->setEnabled(false);
			execInstall();
			ui.pushButton_nextstp->setEnabled(true);

			break;

		case 3:
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, weChinese2LocalCode("so 文件传输"), weChinese2LocalCode("是否现在将模型 so 文件传输到指定下位机 ？"), QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes)
			{
				if (ftpinf == nullptr)
				{
					ftpinf = new TargetWinInfo();
					ftpinf->setAttribute(Qt::WA_DeleteOnClose);
					connect(ftpinf, SIGNAL(emitLoginInf(QStringList)), this, SLOT(weFtp(QStringList)));
				}
				ftpinf->setWindowModality(Qt::ApplicationModal);
				ftpinf->show();
			}
			else
			{
				QMessageBox::information(this, weChinese2LocalCode("关闭"), weChinese2LocalCode("安装完成，请重启 MATLAB 或 刷新 simulink 导航库！"));
				qApp->quit();
			}		
			break;

		case 4:
			qApp->quit();
			break;

		default:
			break;
	}
}

void drv_kits::getMatlabSelectSg(QString str)
{
	m_usrSltMatlabVer = str;
	ui.pushButton_nextstp->setEnabled(true);
}

void drv_kits::weFtp(QStringList inf)
{
	ui.pushButton_nextstp->setEnabled(false);

	QSleepTimeSet *wesleep = new QSleepTimeSet;
	ui.label_disp->setText(weChinese2LocalCode("文件传输中，请稍后 .."));	
	wesleep->Delay_MSec_Suspend(2, true);
	ui.label_disp->setText(weChinese2LocalCode("文件传输中，请稍后 ...."));
	wesleep->Delay_MSec_Suspend(4, true);
	ui.label_disp->setText(weChinese2LocalCode("文件传输中，请稍后 ......"));

	char ip[32] = { '\0' };
	int port = 22;
	char usr[128] = { '\0' };
	char pwd[128] = { '\0' };
	int rv = 0;
	char sopath[512] = { '\0' };

	Qstring2char(inf.at(0), ip);
	Qstring2char(inf.at(1), usr);
	Qstring2char(inf.at(2), pwd);
	
	rv = sftpOpen(ip,port,usr,pwd);
	QString strDrvPacket = QCoreApplication::applicationDirPath() + DRVPACKETS_SIMULINKPATH + SOPACK;
	strDrvPacket.replace(QString("/"), QString("\\"));
	Qstring2char(strDrvPacket, sopath);
	rv = sftpUpdateFileDir(SLAVESOPATH, sopath);
	rv = sftpClose();

	delete wesleep;
	ui.label_disp->setText(weChinese2LocalCode("文件传输中，完成 ！"));
	ui.pushButton_nextstp->setEnabled(true);
}

void drv_kits::cancelClicked()
{
	this->close();
}
