#include <QApplication>
#include <QFile>
#include <QDebug>

#include "emergency.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load stylesheet from Qt resources if available
    QFile styleFile(":/styles/main.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
    }

    EmergencyQueue window;
    window.setWindowTitle("Hospital System - Emergency Queue");
    window.resize(1200, 800);
    window.show();

    return app.exec();
}
