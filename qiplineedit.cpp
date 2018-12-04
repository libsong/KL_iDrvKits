#include "qiplineedit.h"
#include <QRegExpValidator>
#include <QPainter>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

QIPLineEdit::QIPLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    QRegExp rx("(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})");
    QHBoxLayout *pHBox = new QHBoxLayout(this);
    pHBox->setSpacing(10);
    pHBox->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 4; i++)
    {
        m_lineEidt[i] = new QLineEdit(this);
        m_lineEidt[i]->setFrame(false);
		m_lineEidt[i]->setStyleSheet("background:transparent;border-width:0;");
        m_lineEidt[i]->setMaxLength(3);
        m_lineEidt[i]->setAlignment(Qt::AlignCenter);
        m_lineEidt[i]->installEventFilter(this);
        m_lineEidt[i]->setValidator(new QRegExpValidator(rx, this));
        m_lineEidt[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		if (i == 0)
		{
			connect(m_lineEidt[0], SIGNAL(textChanged(const QString &)), this, SLOT(checkFirstIsZero(const QString &)));
		}		

        pHBox->addWidget(m_lineEidt[i]);
    }
    this->setReadOnly(true);
}

QIPLineEdit::~QIPLineEdit()
{

}

void QIPLineEdit::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);
    QPainter painter(this);
    QBrush brush;
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);

    int width = 0;
    for (int i = 0; i < 3; i++)
    {
        width += m_lineEidt[i]->width() + (i == 0 ? 3 : 10);//布局的间隔
        painter.drawEllipse(width, height() / 2 - 2, 4, 4);
    }
}

int QIPLineEdit::getIndex(QLineEdit *pEdit)
{
    int index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (pEdit == m_lineEidt[i])
            index = i;
    }
    return index;
}

bool QIPLineEdit::eventFilter(QObject *obj, QEvent *ev)
{
    if (children().contains(obj) && QEvent::KeyPress == ev->type())
    {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(ev);
        QLineEdit *pEdit = qobject_cast<QLineEdit *>(obj);
        switch (keyEvent->key())
        {
        case Qt::Key_0:
        case Qt::Key_1:
        case Qt::Key_2:
        case Qt::Key_3:
        case Qt::Key_4:
        case Qt::Key_5:
        case Qt::Key_6:
        case Qt::Key_7:
        case Qt::Key_8:
        case Qt::Key_9:
        {
			QString strText = pEdit->text();
            if (pEdit->selectedText().length())
            {
				pEdit->text().replace(pEdit->selectedText(), QChar(keyEvent->key()));             
            }
            else if (strText.length() == 3 || strText.length() < 3 && strText.toInt() * 10 > 255)
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEidt[index + 1]->setFocus();
                    m_lineEidt[index + 1]->selectAll();
                }
            }
            else if (strText.length() == 2 && strText.toInt() * 10 < 255)
            {
                if (Qt::Key_0 == keyEvent->key() && strText.toInt())
                {
                    pEdit->setText(strText.insert(pEdit->cursorPosition(), QChar(Qt::Key_0)));
                }
            }
            return __super::eventFilter(obj, ev);
        }
        break;
        case Qt::Key_Backspace:
        {
            QString strText = pEdit->text();
            if (!strText.length() || strText.length() && !pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEidt[index - 1]->setFocus();
                    int length = m_lineEidt[index - 1]->text().length();
                    m_lineEidt[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        case Qt::Key_Left:
        {
            if (!pEdit->cursorPosition())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 0)
                {
                    m_lineEidt[index - 1]->setFocus();
                    int length = m_lineEidt[index - 1]->text().length();
                    m_lineEidt[index - 1]->setCursorPosition(length ? length : 0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        case Qt::Key_Right:
        {
            if (pEdit->cursorPosition() == pEdit->text().length())
            {
                int index = getIndex(pEdit);
                if (index != -1 && index != 3)
                {
                    m_lineEidt[index + 1]->setFocus();
                    m_lineEidt[index + 1]->setCursorPosition(0);
                }
            }
            return __super::eventFilter(obj, ev);
        }
        default:
            break;
        }
    }
    return false;
}

void QIPLineEdit::setText(const QString &strIP)
{
    if (!isTextValid(strIP))
    {
        QMessageBox::warning(this, "Attention", "Your IP Address is Invalid!", QMessageBox::StandardButton::Ok);
        return;
    }
    else
    {
        int i = 0;
        QStringList ipList = strIP.split(".");
        for each (QString ip in ipList)
        {
            m_lineEidt[i]->setText(ip);
            i++;
        }
    }
}

bool QIPLineEdit::isTextValid(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if (!rx2.exactMatch(strIP))
        return false;
    return true;
}

QString QIPLineEdit::text() const
{
    QString strIP;
	for (int i = 0; i < 4; i++) {
		strIP.append(m_lineEidt[i]->text());
		if (i<3)
		{
			strIP.append("."); //返回带小数点的可用 ip
		}
	}
        
    return strIP;
}

void QIPLineEdit::checkFirstIsZero(const QString &text) //检查 1 段是否在 1~223
{
	QString err = "First segment should be 1~223";
	QString tmp = text;

	if (tmp.length() == 1)
	{
		if (tmp == "0")
		{
			QMessageBox::warning(this, tr("IP ERROR"), err);
			m_lineEidt[0]->setText("1");
		}
	}
	if (tmp.length() == 3)
	{
		if (tmp.toInt() > 223)
		{
			QString t = "First segment should be 1~223";
			QMessageBox::warning(this, tr("IP ERROR"), err);
			m_lineEidt[0]->setText("223");
		}
	}
}
