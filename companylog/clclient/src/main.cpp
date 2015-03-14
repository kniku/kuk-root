#include <QApplication>
//#include <QtPlugin>

//Q_IMPORT_PLUGIN(qjpeg)

#include "MainWindow.h"
#include <QDir>


#if 0
class MyApplication:public QApplication {
	Q_OBJECT
	public:
	void commitData(QSessionManager& sm);
};

void MyApplication::commitData(QSessionManager& sm) {
	
}
#endif

int main(int argc, char *argv[])
{
  //  Q_INIT_RESOURCE(application);

    freopen("test.log","w",stderr);
    freopen("test.log","w",stdout);
	fprintf(stderr,"main entered...");
	QApplication app(argc, argv);

	QDir dir(QApplication::applicationDirPath());
 	//dir.cdUp();
 	//dir.cd("plugins");
 	//QApplication::setLibraryPaths(QStringList(dir.absolutePath()));
	QApplication::addLibraryPath(dir.absolutePath());

    MainWindow mainWin(&app);
	mainWin.setWindowTitle("Companylog");
    mainWin.show();
    return app.exec();
}
