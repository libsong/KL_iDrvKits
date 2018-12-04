#include "version_select.h"
#include <QObject>
#include <QCoreApplication>

versionTree::versionTree(QWidget *parent)
	: QWidget(parent)
{
	m_strMatlabVer = "";
	m_strSimulinkVer = "";
	treeView_Drv = new QTreeView();
	m_VBoxLayout.addWidget(treeView_Drv);

	//驱动包树 https://blog.csdn.net/czyt1988/article/details/18996407
	m_treeModel = new QStandardItemModel(treeView_Drv);
	connect(m_treeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(checkBoxExclusive(QStandardItem*)));
	m_treeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("版本列表"));

	QString styleTree = "QTreeView{\
                border: 1px solid lightgray;\
				outline:none;\
        }\
        QTreeView::item {\
                height: 25px;\
                border: none;\
                background: transparent;\
                color: black;\
        }\
        QTreeView::item:hover {\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(255, 220, 130), stop: 0.5 rgb(255, 220, 130), stop: 1.0 rgb(255, 230, 150));\
        }\
        QTreeView::item:selected {\
                background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(255, 200, 100), stop: 0.5 rgb(255, 230, 150), stop: 1.0 rgb(255, 240, 150));\
        }\
        QTreeView::branch:open:has-children {\
                image: url(展开时图片path);\
        }\
        QTreeView::branch:closed:has-children {\
                image: url(折叠时图片path);\
        }";//暂无图片
	treeView_Drv->setStyleSheet(styleTree);
	treeView_Drv->setModel(m_treeModel);

	treeView_Drv->expandAll();

	this->setLayout(&m_VBoxLayout);
}

versionTree::~versionTree()
{

}

void versionTree::iDrvKits_findLocalDrv_simulink(QStringList *t)
{
	m_strDrvPacket = QCoreApplication::applicationDirPath() + DRVPACKETS_SIMULINKPATH;
	m_strDrvPacket.replace(QString("/"),QString("//"));
	QStringList strings;
	QDir drvDir(m_strDrvPacket);
	drvDir.setFilter(QDir::Dirs);	
	QFileInfoList lst = drvDir.entryInfoList(strings);
	
	for (int i = 0; i < lst.count();i++)
	{
		QFileInfo tmpFileInf = lst.at(i);
		if ((tmpFileInf.isDir()))
		{
			QString name = tmpFileInf.fileName();
			if (name == "." || name == "..")
			{
				continue;
			}
			*t << name;
		}
	}
}

void versionTree::makeMatlabVersionTree(QStringList ver)
{
	//本地MATLAB版本创建子树
	QString strTmp;
	m_itemMatlab = new QStandardItem("MATLAB");
	m_treeModel->appendRow(m_itemMatlab);
	
	QList<QString>::Iterator it = ver.begin(), itend = ver.end();
	for (int i = 0; it != itend;it++,i++)
	{
		strTmp = *it;
		m_itemMatlabChild[i] = new QStandardItem(strTmp);
		m_itemMatlabChild[i]->setCheckable(false); //暂时丢弃matlab版本选择
		m_itemMatlabChild[i]->setTristate(false);
		m_itemMatlab->appendRow(m_itemMatlabChild[i]);
	}
	treeView_Drv->expandAll();
}

void versionTree::makeSimulinkVersionTree(QStringList ver)
{
	//simulink 安装包驱动版本树
	QString strTmp;
	m_itemSimulink = new QStandardItem(QStringLiteral("Keliang Simulink"));
	m_treeModel->appendRow(m_itemSimulink);

	QList<QString>::Iterator it = ver.begin(), itend = ver.end();
	for (int i = 0; it != itend; it++, i++)
	{
		strTmp = *it;
		m_itemSimulinkChild[i] = new QStandardItem(strTmp);
		m_itemSimulinkChild[i]->setEditable(false);
		m_itemSimulinkChild[i]->setCheckable(true);
		m_itemSimulinkChild[i]->setTristate(false);
		m_itemSimulink->appendRow(m_itemSimulinkChild[i]);
	}
	treeView_Drv->expandAll();
}

void versionTree::checkBoxExclusive(QStandardItem *item) //控件互斥
{
	disconnect(m_treeModel, SIGNAL(itemChanged(QStandardItem*)) ,0,0);//otherwise go in dead loop

	QStandardItem *parent = item->parent();
	if (parent == nullptr)
	{
		return;
	}
	int brothercount = parent->rowCount();
	for (int i = 0; i < brothercount;i++)
	{
		QStandardItem *itm = parent->child(i);
		itm->setCheckState(Qt::Unchecked);
	}
	item->setCheckState(Qt::Checked);

	if (parent->text() == "MATLAB")
	{
		m_strMatlabVer = item->text();
		emit treeItemNameSend(m_strMatlabVer);
	} 
	else if (parent->text() == "Keliang Simulink")
	{
		m_strSimulinkVer = item->text();
	}
	else {
	}	

	connect(m_treeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(checkBoxExclusive(QStandardItem*)));
}

