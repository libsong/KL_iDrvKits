#include "targetwininfo.h"
#include "ui_targetwininfo.h"

#include <QLineEdit>
#include <QMessageBox>

#include "we_types.h"

extern quint8 g_NodeTotal;
extern QString cfg_INIPATH;

TargetWinInfo::TargetWinInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TargetWinInfo)
{
    ui->setupUi(this);

    ui->lineEdit_Tusr->setEchoMode(QLineEdit::Normal);
    ui->lineEdit_Tusr->setPlaceholderText("root");

    ui->lineEdit_Tpwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_Tpwd->setPlaceholderText("redhat");
}

TargetWinInfo::~TargetWinInfo()
{
    delete ui;
}

void TargetWinInfo::getTinfo(QString &ip,QString &usr,QString &pwd)
{
    ip = m_ip;
    usr = m_usr;
    pwd = m_pwd;
}

void TargetWinInfo::on_pushButton_Tyes_clicked()
{
    m_ip = ui->lineEdit_Tipaddr->text();
    m_usr = ui->lineEdit_Tusr->text();
    if (m_usr == "") {
        QMessageBox::warning (this,tr("WARNING"),weChinese2LocalCode("用户名 空 ！\n请重新输入"));
        return ;
    }
    m_pwd = ui->lineEdit_Tpwd->text();
    if (m_pwd == "") {
        QMessageBox::warning (this,tr("WARNING"), weChinese2LocalCode("密码 空 ！\n请重新输入"));
        return ;
    }

    this->close();

	QStringList inf;
	inf.append(m_ip);
	inf.append(m_usr);
	inf.append(m_pwd);
	emit emitLoginInf(inf);
}
