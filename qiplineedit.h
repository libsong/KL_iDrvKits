#ifndef QIPLINEEDIT_H
#define QIPLINEEDIT_H

#include <QLineEdit>
#include <QEvent>

//实现 ipv4 的 ip地址输入框，类似于Windows设置ip地址的效果
//另外添加右键功能

class QIPLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QIPLineEdit(QWidget *parent = 0);
    ~QIPLineEdit();

    void setText(const QString &strIP);
    QString text() const;

public slots:
	void checkFirstIsZero(const QString &text);

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);

    int getIndex(QLineEdit *pEdit);
    bool isTextValid(const QString &strIP);
private:
    QLineEdit *m_lineEidt[4];
};


#endif // QIPLINEEDIT_H
