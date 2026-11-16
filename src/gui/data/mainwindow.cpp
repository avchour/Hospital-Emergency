
#include "../include/mainwindow.h"
#include "../include/dashboard.h"

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QStringList>

// ==========================================================
// CONSTRUCTOR
// ==========================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Hospital Emergency Management System");

    // ✏️ MANUAL CHANGE: Default application window size
    resize(1400, 850);

    setupUI();
}

// ==========================================================
// MAIN GUI SETUP
// ==========================================================

void MainWindow::setupUI()
{
    // ================= CENTRAL WIDGET =================

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");

    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ================= SIDEBAR =================

    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");

    // ✏️ MANUAL CHANGE: Adjust sidebar width
    sidebar->setFixedWidth(260);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 25, 18, 20);
    sidebarLayout->setSpacing(10);

    // ================= BRAND HEADER =================

    QWidget *brandWidget = new QWidget();
    QHBoxLayout *brandLayout = new QHBoxLayout(brandWidget);

    brandLayout->setContentsMargins(2, 0, 2, 10);
    brandLayout->setSpacing(12);

    // Gold hospital emblem
    QLabel *logoIcon = new QLabel("+");
    logoIcon->setObjectName("brandIcon");
    logoIcon->setAlignment(Qt::AlignCenter);
    logoIcon->setFixedSize(42, 42);

    brandLayout->addWidget(logoIcon);

    // Brand name and subtitle
    QVBoxLayout *brandTextLayout = new QVBoxLayout();
    brandTextLayout->setSpacing(3);

    QLabel *logo = new QLabel("HOSPITAL SYSTEM");
    logo->setObjectName("brandTitle");

    QLabel *subtitle = new QLabel("Emergency Care Management");
    subtitle->setObjectName("brandSubtitle");
    subtitle->setWordWrap(true);

    brandTextLayout->addWidget(logo);
    brandTextLayout->addWidget(subtitle);

    brandLayout->addLayout(brandTextLayout);
    brandLayout->addStretch();

    sidebarLayout->addWidget(brandWidget);

    // ================= MENU HEADING =================

    QLabel *menuLabel = new QLabel("MAIN MENU");
    menuLabel->setObjectName("menuHeading");

    // ✏️ MANUAL CHANGE: Space before navigation
    sidebarLayout->addSpacing(15);
    sidebarLayout->addWidget(menuLabel);
    sidebarLayout->addSpacing(5);

    // ================= MAIN PAGES =================

    pages = new QStackedWidget();

    // Page 0: Dashboard
    Dashboard *dashboard = new Dashboard(
        [this](int index)
        {
            changePage(index);
            setActiveButton(index);
        });

    pages->addWidget(dashboard);

    // ✏️ MANUAL CHANGE:
    // Add or remove pages here when your project grows.
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
        title->setObjectName("pageTitle");

        layout->addWidget(title);
        layout->addStretch();

        pages->addWidget(page);
    }

    // ================= NAVIGATION BUTTONS =================

    // ✏️ MANUAL CHANGE:
    // Edit these symbols and labels to customize navigation.
    QStringList menuItems = {
        "▦    Dashboard",
        "♙    Patient Management",
        "♥    Emergency Triage",
        "☷    Emergency Queue",
        "✚    Treatment Management"};

    navButtons.clear();

    for (int i = 0; i < menuItems.size(); i++)
    {
        QPushButton *button = new QPushButton(menuItems[i]);

        button->setObjectName("navButton");
        button->setCheckable(true);
        button->setMinimumHeight(48);
        button->setCursor(Qt::PointingHandCursor);

        sidebarLayout->addWidget(button);
        navButtons.append(button);

        connect(button, &QPushButton::clicked,
                this, [this, i]()
                {
            changePage(i);
            setActiveButton(i); });
    }

    // ================= INITIAL PAGE =================

    pages->setCurrentIndex(0);
    setActiveButton(0);

    // ================= SIDEBAR FOOTER =================

    sidebarLayout->addStretch();

    QFrame *footerCard = new QFrame();
    footerCard->setObjectName("sidebarFooterCard");

    QHBoxLayout *footerLayout = new QHBoxLayout(footerCard);
    footerLayout->setContentsMargins(8, 10, 8, 10);
    footerLayout->setSpacing(10);

    // Administrator avatar
    QLabel *avatar = new QLabel("A");
    avatar->setObjectName("adminAvatar");
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setFixedSize(36, 36);

    footerLayout->addWidget(avatar);

    // Administrator information
    QVBoxLayout *adminInfoLayout = new QVBoxLayout();
    adminInfoLayout->setSpacing(3);

    QLabel *adminName = new QLabel("Administrator");
    adminName->setObjectName("adminName");

    // System status row
    QHBoxLayout *statusLayout = new QHBoxLayout();
    statusLayout->setSpacing(5);

    QLabel *statusDot = new QLabel("●");
    statusDot->setObjectName("statusDot");

    QLabel *statusText = new QLabel("System Online");
    statusText->setObjectName("systemStatus");

    statusLayout->addWidget(statusDot);
    statusLayout->addWidget(statusText);
    statusLayout->addStretch();

    adminInfoLayout->addWidget(adminName);
    adminInfoLayout->addLayout(statusLayout);

    footerLayout->addLayout(adminInfoLayout);
    footerLayout->addStretch();

    sidebarLayout->addWidget(footerCard);

    // ================= MAIN CONTENT =================

    QWidget *mainContent = new QWidget();
    mainContent->setObjectName("mainContent");

    QVBoxLayout *contentLayout = new QVBoxLayout(mainContent);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    contentLayout->addWidget(pages);

    // ================= FINAL LAYOUT =================

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainContent);
}

// ==========================================================
// PAGE NAVIGATION
// ==========================================================

void MainWindow::changePage(int index)
{
    if (index < 0 || index >= pages->count())
        return;

    pages->setCurrentIndex(index);
}

// ==========================================================
// ACTIVE NAVIGATION DESIGN
// ==========================================================

void MainWindow::setActiveButton(int activeIndex)
{
    for (int i = 0; i < navButtons.size(); i++)
    {
        navButtons[i]->setChecked(i == activeIndex);
    }
}
