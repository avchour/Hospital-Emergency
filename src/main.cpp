
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QDir>
#include <QFileInfoList>
#include <QStandardPaths>

#include "mainwindow.h"

static void configureApplicationTypography(QApplication &app)
{
    // Search standard system and user font paths for Gotham and Montserrat (Windows & Fedora)
    QStringList searchDirs;
    searchDirs << QApplication::applicationDirPath() + "/fonts";
    searchDirs << QApplication::applicationDirPath();

#if defined(Q_OS_LINUX)
    // Standard Fedora / Linux font locations
    searchDirs << "/usr/share/fonts/google-montserrat";
    searchDirs << "/usr/share/fonts/montserrat";
    searchDirs << "/usr/share/fonts";
    searchDirs << "/usr/local/share/fonts";
    searchDirs << QDir::homePath() + "/.local/share/fonts";
    searchDirs << QDir::homePath() + "/.fonts";
#elif defined(Q_OS_WIN)
    searchDirs << QStandardPaths::writableLocation(QStandardPaths::FontsLocation);
    searchDirs << "C:/Windows/Fonts";
#endif

    for (const QString &dirPath : searchDirs)
    {
        QDir dir(dirPath);
        if (dir.exists())
        {
            QStringList filters;
            filters << "*gotham*.ttf" << "*gotham*.otf"
                    << "*montserrat*.ttf" << "*montserrat*.otf";
            const QFileInfoList fontFiles = dir.entryInfoList(filters, QDir::Files | QDir::Readable);
            for (const QFileInfo &fileInfo : fontFiles)
            {
                QFontDatabase::addApplicationFont(fileInfo.absoluteFilePath());
            }
        }
    }

    // Check availability of Gotham and Montserrat in QFontDatabase
    bool hasGotham = false;
    bool hasMontserrat = false;
    const QStringList installedFamilies = QFontDatabase::families();
    for (const QString &family : installedFamilies)
    {
        if (family.compare("Gotham", Qt::CaseInsensitive) == 0 ||
            family.startsWith("Gotham ", Qt::CaseInsensitive))
        {
            hasGotham = true;
        }
        else if (family.compare("Montserrat", Qt::CaseInsensitive) == 0 ||
                 family.startsWith("Montserrat ", Qt::CaseInsensitive))
        {
            hasMontserrat = true;
        }
    }

    QString chosenPrimary;
    QStringList familyPreference;

    if (hasGotham)
    {
        chosenPrimary = "Gotham";
        familyPreference << "Gotham" << "Montserrat";
        qInfo() << "[Typography] Gotham detected on system. Using Gotham as primary application font.";
    }
    else if (hasMontserrat)
    {
        chosenPrimary = "Montserrat";
        familyPreference << "Montserrat" << "Gotham";
        qInfo() << "[Typography] Gotham not found. Using Montserrat fallback font.";
    }
    else
    {
        // Neither font is currently installed on host:
        // Set Gotham and Montserrat at the top of the preference chain so they are
        // automatically used if installed, with native platform fallbacks underneath.
        chosenPrimary = "Gotham";
        familyPreference << "Gotham" << "Montserrat";
        qInfo() << "[Typography] Neither Gotham nor Montserrat was detected as pre-installed."
                << "Configured font family chain: Gotham -> Montserrat -> platform fallbacks.";
    }

#if defined(Q_OS_WIN)
    familyPreference << "Segoe UI" << "Arial" << "sans-serif";
#elif defined(Q_OS_LINUX)
    // Fedora / Linux standard desktop font stack
    familyPreference << "Cantarell" << "Liberation Sans" << "DejaVu Sans" << "sans-serif";
#else
    familyPreference << "Helvetica Neue" << "Helvetica" << "Arial" << "sans-serif";
#endif

    QFont appFont(chosenPrimary);
    appFont.setFamilies(familyPreference);
    appFont.setStyleHint(QFont::SansSerif);
    appFont.setPointSize(10);
    app.setFont(appFont);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Configure application-wide font (Gotham with Montserrat fallback)
    configureApplicationTypography(app);

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
