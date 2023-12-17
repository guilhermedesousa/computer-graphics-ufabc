#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
int main(int argc, char *argv[])
{
    QSurfaceFormat format;
#ifdef Q_OS_MAC
    format.setVersion(4,1);
#else
    format.setVersion(4,0);
#endif

    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    //format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
