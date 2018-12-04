#include "drv_kits.h"
#include <QApplication>
#include <QSettings>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//使用QSharedMemory防止多开  
	QSharedMemory shared_memory;
	shared_memory.setKey(QString("main_window_drvkits"));
	if (shared_memory.attach())
	{
		return 0;
	}

	if (shared_memory.create(1))
	{

		QCoreApplication::setOrganizationName(QString("KL DrvKits"));
		QCoreApplication::setApplicationName(QString("KL Simulink"));


		drv_kits w;
		w.setWindowTitle("Keliang iDriver Kits");
		w.setWindowIcon(QIcon(":/drv_kits/pic/proc.ico"));
		w.setWindowFlags(w.windowFlags() & ~(Qt::WindowMaximizeButtonHint));
		w.setWindowOpacity(0.95);
		w.show();

		return a.exec();
	}
}
