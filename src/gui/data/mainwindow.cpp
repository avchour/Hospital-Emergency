#include "../include/mainwindow.h"
#include "../include/dashboard.h"
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hospital Emergency Management System");
    resize(1280, 800);

    setupUI();
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(230);

    sidebar->setStyleSheet(
        "background-color: #172B4D;");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(15, 25, 15, 20);
    sidebarLayout->setSpacing(12);

    QLabel *logo = new QLabel("HOSPITAL EMS");

    logo->setStyleSheet(
        "color: white;"
        "font-size: 20px;"
        "font-weight: bold;"
        "padding: 10px;");

    sidebarLayout->addWidget(logo);

    QLabel *menuLabel = new QLabel("MAIN MENU");

    menuLabel->setStyleSheet(
        "color: #AAB7CF;"
        "font-size: 11px;"
        "font-weight: bold;"
        "padding: 10px;");

    sidebarLayout->addWidget(menuLabel);

    // Main content pages
    pages = new QStackedWidget();

    pages->addWidget(new Dashboard());

    QStringList pageNames = {
        "Patient Management",
        "Emergency Triage",
        "Emergency Queue",
        "Treatment Management"};

    for (const QString &name : pageNames)
    {
        QWidget *page = new QWidget();
        QVBoxLayout *layout = new QVBoxLayout(page);

        QLabel *title = new QLabel(name);
        title->setStyleSheet(
            "font-size: 26px;"
            "font-weight: bold;"
            "color: #172B4D;");

        layout->addWidget(title);
        layout->addStretch();

        pages->addWidget(page);
    }

    // Navigation buttons
    QStringList menuItems = {
        "Dashboard",
        "Patient Management",
        "Emergency Triage",
        "Emergency Queue",
        "Treatment Management"};

    for (int i = 0; i < menuItems.size(); i++)
    {
        QPushButton *button = new QPushButton(menuItems[i]);

        button->setMinimumHeight(45);
        button->setCursor(Qt::PointingHandCursor);

        button->setStyleSheet(
            "QPushButton {"
            "color: white;"
            "background-color: transparent;"
            "border: none;"
            "border-radius: 6px;"
            "text-align: left;"
            "padding-left: 15px;"
            "font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "background-color: #29446D;"
            "}");

        sidebarLayout->addWidget(button);

        connect(button, &QPushButton::clicked, this,
                [this, i]()
                {
                    changePage(i);
                });
    }

    sidebarLayout->addStretch();

    QLabel *footer = new QLabel("Hospital Management System");

    footer->setStyleSheet(
        "color: #AAB7CF;"
        "font-size: 10px;"
        "padding: 10px;");

    sidebarLayout->addWidget(footer);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pages);
}

void MainWindow::changePage(int index)
{
    pages->setCurrentIndex(index);
}
