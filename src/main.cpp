#include "gui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationVersion(QStringLiteral(APP_VERSION));

    MainWindow window;
    window.show();
    return app.exec();
}
