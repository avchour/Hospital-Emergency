
#include <QApplication>
#include <QFile>
#include <QDebug>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 🎨 DESIGN: Load embedded stylesheet
    // ✏️ MANUAL CHANGE: Change this path only if
    // you change its alias in resources.qrc.

    QFile styleFile(":/styles/main.qss");

    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        app.setStyleSheet(
            QString::fromUtf8(styleFile.readAll()));
    }
    else
    {
        qWarning() << "Unable to load stylesheet:"
                   << styleFile.errorString();
    }

    MainWindow window;
    window.show();

    return app.exec();
}
