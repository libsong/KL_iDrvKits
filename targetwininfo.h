#ifndef TARGETWININFO_H
#define TARGETWININFO_H

#include <QDialog>
#include "qiplineedit.h"

namespace Ui {
class TargetWinInfo;
}

class TargetWinInfo : public QDialog
{
    Q_OBJECT

public:
    explicit TargetWinInfo(QWidget *parent = 0);
    ~TargetWinInfo();

    void getTinfo(QString &ip,QString &usr,QString &pwd);

    QString m_usr;
    QString m_pwd;
    QString m_ip;

    QLineEdit *m_lineEidt[4];

signals:
	void emitLoginInf(QStringList inflst);	

private slots:
    void on_pushButton_Tyes_clicked();

private:
    Ui::TargetWinInfo *ui;
};

#endif // TARGETWININFO_H
