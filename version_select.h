#pragma once

#include <QStandardItemModel>
#include <QCheckBox>
#include <QDir>
#include <QTreeView>
#include <QVBoxLayout>

#define MAXCHILD 64				//�������ֵ
#define DRVPACKETS_SIMULINKPATH "/DrvPackets/simulink/" //simulink ģ���λ��
#define SIMULINKSTARTUP "/Startup/" //startup.m

class versionTree : public QWidget
{
	Q_OBJECT

public:
	versionTree(QWidget *parent = 0);
	~versionTree();

	void iDrvKits_findLocalDrv_simulink(QStringList *ver);
	void makeMatlabVersionTree(QStringList ver);
	void makeSimulinkVersionTree(QStringList ver);

	QTreeView *treeView_Drv;

	QStandardItemModel* m_treeModel;
	QStandardItem* m_itemMatlab;
	QStandardItem* m_itemMatlabChild[MAXCHILD];
	QStandardItem* m_itemSimulink;
	QStandardItem* m_itemSimulinkChild[MAXCHILD];

	QString m_strDrvPacket;	// simulink ģ���·��
	QString m_strSimulinkVer;
	QString m_strMatlabVer;


signals:
	void treeItemNameSend(QString str);

private slots :
	void checkBoxExclusive(QStandardItem *item);

private:
	QVBoxLayout m_VBoxLayout;

};
