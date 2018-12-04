#include "we_types.h"
#include <QByteArray>
#include <QTime>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QEventLoop>

//https://blog.csdn.net/fantasker/article/details/73199267
void stringToHtmlFilter(QString &str)
{
	//ע���⼸�д����˳�����ң��������ɶ���滻
	str.replace("&", "&amp;");
	str.replace(">", "&gt;");
	str.replace("<", "&lt;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&#39;");
	str.replace(" ", "&nbsp;");
	str.replace("\n", "<br>");
	str.replace("\r", "<br>");
}

void stringToHtml(QString &str, QColor crl)
{
	QByteArray array;
	array.append(crl.red());
	array.append(crl.green());
	array.append(crl.blue());
	QString strC(array.toHex());
	str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
}

QString weChinese2LocalCode(char *x)
{
	QTextCodec *codec = QTextCodec::codecForLocale();
	QString tmp;
	tmp = codec->toUnicode(x);

	return tmp;
}

void Qstring2char(QString  str,char *c)
{
	QByteArray ba = str.toLatin1(); // must
	memcpy(c,ba.data(),ba.size());
}

QSleepTimeSet::QSleepTimeSet()
{
}

QSleepTimeSet::~QSleepTimeSet()
{
}

void QSleepTimeSet::Delay_MSec_Suspend(unsigned int msec,bool suspend)
{
	if (suspend)
	{
		QElapsedTimer t;

		t.start();
		while (t.elapsed() < msec) {
			QCoreApplication::processEvents();
		}
	} 
	else
	{
		QTime dieTime = QTime::currentTime().addMSecs(msec);

		while (QTime::currentTime() < dieTime) {
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}
	}
}


QFolderAndFilesOp::QFolderAndFilesOp()
{
}

QFolderAndFilesOp::~QFolderAndFilesOp()
{
}

void QFolderAndFilesOp::clearFolder(const QString &folderFullPath, bool rmFolderSelf)
{
	QDir dir(folderFullPath);
	QFileInfoList fileList;
	QFileInfo curFile;
	QFileInfoList fileListTemp;
	int infoNum;
	int i;
	int j;
	/* ���Ȼ�ȡĿ���ļ����������ļ����ļ�����Ϣ */
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files
		| QDir::Readable | QDir::Writable
		| QDir::Hidden | QDir::NoDotAndDotDot
		, QDir::Name);

	while (fileList.size() > 0)
	{
		infoNum = fileList.size();

		for (i = infoNum - 1; i >= 0; i--)
		{
			curFile = fileList[i];
			if (curFile.isFile()) /* ������ļ���ɾ���ļ� */
			{
				QFile fileTemp(curFile.filePath());
				fileTemp.remove();
				fileList.removeAt(i);
			}

			if (curFile.isDir()) /* ������ļ��� */
			{
				QDir dirTemp(curFile.filePath());
				fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
					| QDir::Readable | QDir::Writable
					| QDir::Hidden | QDir::NoDotAndDotDot
					, QDir::Name);
				if (fileListTemp.size() == 0) /* �²�û���ļ����ļ��� ��ֱ��ɾ��*/
				{
					dirTemp.rmdir(".");
					fileList.removeAt(i);
				}
				else /* �²����ļ��л��ļ� ����Ϣ��ӵ��б�*/
				{
					for (j = 0; j < fileListTemp.size(); j++)
					{
						if (!(fileList.contains(fileListTemp[j])))
						{
							fileList.append(fileListTemp[j]);
						}
					}
				}
			}
		}
	}
	if (rmFolderSelf)
	{
		dir.rmdir(".");/*ɾ��Ŀ���ļ���*/
	} 	
}

bool QFolderAndFilesOp::copyFileToPath(QString sourceDir, QString toDir, bool coverFileIfExist)
{
	toDir.replace("\\", "/");
	if (sourceDir == toDir) {
		return true;
	}
	if (!QFile::exists(sourceDir)) {
		return false;
	}
	QDir *createfile = new QDir;
	bool exist = createfile->exists(toDir);
	if (exist) {
		if (coverFileIfExist) {
			createfile->remove(toDir);
		}
	}//end if

	if (!QFile::copy(sourceDir, toDir))
	{
		return false;
	}
	return true;
}

//qCopyDirectory -- ����Ŀ¼ 
//fromDir : ԴĿ¼ 
//toDir : Ŀ��Ŀ¼ 
//bCoverIfFileExists : ture:ͬ��ʱ���� ,false:ͬ��ʱ����false,��ֹ���� 
//����: ture�����ɹ� false:����δ��� 
bool QFolderAndFilesOp::qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< ���Ŀ��Ŀ¼�����ڣ�����д��� */
		if (!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    /**< ��ΪĿ¼ʱ���ݹ�Ľ���copy */
			if (!qCopyDirectory(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				bCoverIfFileExists))
				return false;
		}
		else {            /**< �������ǲ���ʱ�������ļ�����ɾ������ */
			if (bCoverIfFileExists && targetDir.exists(fileInfo.fileName())) {
				targetDir.remove(fileInfo.fileName());
			}

			/// �����ļ�copy
			if (!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))) {
				return false;
			}
		}
	}

	return true;
}



