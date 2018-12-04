#include "we_types.h"
#include <QByteArray>
#include <QTime>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QEventLoop>

//https://blog.csdn.net/fantasker/article/details/73199267
void stringToHtmlFilter(QString &str)
{
	//注意这几行代码的顺序不能乱，否则会造成多次替换
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
	/* 首先获取目标文件夹内所有文件及文件夹信息 */
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
			if (curFile.isFile()) /* 如果是文件，删除文件 */
			{
				QFile fileTemp(curFile.filePath());
				fileTemp.remove();
				fileList.removeAt(i);
			}

			if (curFile.isDir()) /* 如果是文件夹 */
			{
				QDir dirTemp(curFile.filePath());
				fileListTemp = dirTemp.entryInfoList(QDir::Dirs | QDir::Files
					| QDir::Readable | QDir::Writable
					| QDir::Hidden | QDir::NoDotAndDotDot
					, QDir::Name);
				if (fileListTemp.size() == 0) /* 下层没有文件或文件夹 则直接删除*/
				{
					dirTemp.rmdir(".");
					fileList.removeAt(i);
				}
				else /* 下层有文件夹或文件 则将信息添加到列表*/
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
		dir.rmdir(".");/*删除目标文件夹*/
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

//qCopyDirectory -- 拷贝目录 
//fromDir : 源目录 
//toDir : 目标目录 
//bCoverIfFileExists : ture:同名时覆盖 ,false:同名时返回false,终止拷贝 
//返回: ture拷贝成功 false:拷贝未完成 
bool QFolderAndFilesOp::qCopyDirectory(const QString& fromDir, const QString& toDir, bool bCoverIfFileExists)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
		if (!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */
			if (!qCopyDirectory(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()),
				bCoverIfFileExists))
				return false;
		}
		else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if (bCoverIfFileExists && targetDir.exists(fileInfo.fileName())) {
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy
			if (!QFile::copy(fileInfo.filePath(),
				targetDir.filePath(fileInfo.fileName()))) {
				return false;
			}
		}
	}

	return true;
}



