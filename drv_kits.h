#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_drv_kits.h"

#include "targetwininfo.h"
#include "../matlabApi/matlab_Comm.h"
#include "version_select.h"
#include <QSettings>

class drv_kits : public QMainWindow
{
	Q_OBJECT

public:
	drv_kits(QWidget *parent = Q_NULLPTR);
	~drv_kits();

	int now_installStep;
	QString installedLibPath;
	QString installedLibVer;
	QString	usrSelectPath;

	QSettings *weReg;
	TargetWinInfo *ftpinf;

	weMatlabComm	*m_MatlabInf;
	QString			m_usrSltMatlabVer;

	versionTree *m_ptrVerTree;
	int sMatlabTreeIdx;

	QStringList matlabverAddYear(QStringList verlst);
	void execInstall();

public slots:
	void aboutMenuClicked();
	void usrHelp();

	void ftp();

	void stepChangeResponse();

	void getMatlabSelectSg(QString str);

	void weFtp(QStringList inf);

	void cancelClicked();

private:
	Ui::drv_kitsClass ui;
};
