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

#define PRODUCTVERSION		"1,0"	//����ע����еİ汾��¼�������汾
#define SIMULNKPACK			"/ks_v1"
#define SOPACK				"ks_lib_v1"
#define REGKEY				"KL DrvKits"
#define SLINKKEYNAME		"KL Simulink"
#define SLAVESOPATH			"/usr/lib"
#define KEYPATH				"HKEY_CURRENT_USER\\Software\\KL DrvKits"

#define FILEVERSION			1,0,0,0	//about�������ʾ��Դ���ļ��汾
#define PVERSION			1,1	//about �������ʾ�Ĳ�Ʒ�汾


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

	ui.label_disp->setText(weChinese2LocalCode("��ӭʹ�� ����ģ��&���� ��װ���߼� ..."));

	//����Ƿ�װ matlab �Լ��汾
	if (!m_MatlabInf->Initlize())
	{
		QMessageBox::warning(this, tr("WARNING"), weChinese2LocalCode("δ��⵽��װ�� Matlab �汾\n��ȷ�Ϻ����´򿪱������"));
		qApp->quit();
	}
	else
	{
		m_ptrVerTree = new versionTree;
		connect(m_ptrVerTree, SIGNAL(treeItemNameSend(QString)), this, SLOT(getMatlabSelectSg(QString)));

		m_ptrVerTree->makeMatlabVersionTree(matlabverAddYear(m_MatlabInf->m_matlabVerLst));
	}

	//�˵����źš���
	connect(ui.action_about, SIGNAL(triggered()), this, SLOT(aboutMenuClicked()));
	connect(ui.action_usrdoc, SIGNAL(triggered()), this, SLOT(usrHelp()));
	connect(ui.action_ftp, SIGNAL(triggered()), this, SLOT(ftp()));

	//��ע����Ƿ�װ��
	//weReg = new QSettings (QSettings::NativeFormat, QSettings::SystemScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	weReg = new QSettings(QSettings::NativeFormat, QSettings::UserScope,REGKEY, SLINKKEYNAME);
	installedLibPath = weReg->value("libpath").toString();	
	if (installedLibPath != "")
	{
		QString installedLibVer = weReg->value("Version").toString();
		ui.label_disp->setText(weChinese2LocalCode("��⵽ KL simulink ���Ѱ�װ, ����ж�� ..."));
		ui.pushButton_nextstp->setText(weChinese2LocalCode("ж��"));
	} 
	else
	{
		ui.pushButton_nextstp->setText(weChinese2LocalCode("��װ"));
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

	//дע���
	ui.label_disp->setText(weChinese2LocalCode("KL simulink �ⰲװ�� ..."));
	if (weReg == nullptr)
	{
		weReg = new QSettings(QSettings::NativeFormat, QSettings::UserScope, REGKEY, SLINKKEYNAME);
	}
	weReg->setValue("Name", "simulink");
	weReg->setValue("Version", PRODUCTVERSION);
	weReg->setValue("libpath", usrSelectPath);
	//weReg->setValue("matlabVer", m_usrSltMatlabVer);
	wesleep->Delay_MSec_Suspend(1000, true);

	//�����ļ���·����
	ui.label_disp->setText(weChinese2LocalCode("KL simulink �ⰲװ�� ......"));	
	QString strDrvPacket = QCoreApplication::applicationDirPath() + DRVPACKETS_SIMULINKPATH;
	strDrvPacket.replace(QString("/"), QString("//"));
	bool rt = false;
	QFolderAndFilesOp wecp;
	rt = wecp.qCopyDirectory(strDrvPacket, usrSelectPath, false);
	if (!rt)
	{
		//�ļ�����ʧ��
	}
	wesleep->Delay_MSec_Suspend(1000, true);

	ui.label_disp->setText(weChinese2LocalCode("KL simulink �ⰲװ�� ........."));
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
			while (!in.atEnd())//TODO:û��������
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
	ui.label_disp->setText(weChinese2LocalCode("KL simulink �ⰲװ�� ............"));
	wesleep->Delay_MSec_Suspend(1000, true);
	ui.label_disp->setText(weChinese2LocalCode("KL simulink �ⰲװ��� ��"));
	ui.pushButton_nextstp->setText(weChinese2LocalCode("���"));
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
	company->setText(weChinese2LocalCode("�Ϻ�������Ϣ���̹ɷ����޹�˾"));
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
		ui.label_disp->setText(weChinese2LocalCode("KL simulink ��ж���� ..."));
		ui.pushButton_nextstp->setEnabled(false);

		//ж��
		QSettings sett(KEYPATH,QSettings::NativeFormat);
		sett.remove("");

		QSleepTimeSet wesleep;
		wesleep.Delay_MSec_Suspend(2000, true);

		ui.label_disp->setText(weChinese2LocalCode("KL simulink ��ж���� ......"));
		QFolderAndFilesOp werm;		
		werm.clearFolder(installedLibPath + SIMULNKPACK,true);
		werm.clearFolder(installedLibPath + "/" + SOPACK, true);
		wesleep.Delay_MSec_Suspend(2000, true);

		ui.label_disp->setText(weChinese2LocalCode("KL simulink ��ж����ɣ����򼴽��رգ������´򿪰�װ ��"));
		wesleep.Delay_MSec_Suspend(5000, true);
		qApp->quit();
		ui.pushButton_nextstp->setEnabled(true);
		ui.pushButton_nextstp->setText(weChinese2LocalCode("��װ"));
		ui.pushButton_nextstp->setEnabled(true);
		installedLibPath = "";
		return;
	}
	
	now_installStep++;

	switch (now_installStep)
	{	
		case 1:
			ui.label_disp->setText(weChinese2LocalCode("�������� MATLAB �汾"));
			ui.stackedWidget_center->addWidget(m_ptrVerTree);
			sMatlabTreeIdx = ui.stackedWidget_center->count() - 1;
			ui.stackedWidget_center->setCurrentIndex(sMatlabTreeIdx);

			ui.pushButton_nextstp->setText(weChinese2LocalCode("��һ��"));
			break;

		case 2:
			ui.pushButton_cancel->setEnabled(true);
			ui.label_disp->setText(weChinese2LocalCode("��ѡ�� KL simulink ��İ�װ�洢·�� ..."));
			dir = QFileDialog::getExistingDirectory(this, weChinese2LocalCode("ѡ��װ�洢·��"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			if (dir.isEmpty())
			{
				this->close();
			}
			usrSelectPath = dir;
			dir = weChinese2LocalCode("KL simulink ģ��⽫�ᰲװ�� �� ") + dir;			
			ui.label_disp->setText(dir);

			ui.pushButton_nextstp->setEnabled(false);
			execInstall();
			ui.pushButton_nextstp->setEnabled(true);

			break;

		case 3:
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, weChinese2LocalCode("so �ļ�����"), weChinese2LocalCode("�Ƿ����ڽ�ģ�� so �ļ����䵽ָ����λ�� ��"), QMessageBox::Yes | QMessageBox::No);
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
				QMessageBox::information(this, weChinese2LocalCode("�ر�"), weChinese2LocalCode("��װ��ɣ������� MATLAB �� ˢ�� simulink �����⣡"));
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
	ui.label_disp->setText(weChinese2LocalCode("�ļ������У����Ժ� .."));	
	wesleep->Delay_MSec_Suspend(2, true);
	ui.label_disp->setText(weChinese2LocalCode("�ļ������У����Ժ� ...."));
	wesleep->Delay_MSec_Suspend(4, true);
	ui.label_disp->setText(weChinese2LocalCode("�ļ������У����Ժ� ......"));

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
	ui.label_disp->setText(weChinese2LocalCode("�ļ������У���� ��"));
	ui.pushButton_nextstp->setEnabled(true);
}

void drv_kits::cancelClicked()
{
	this->close();
}
