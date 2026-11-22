
#include "../include/mainwindow.h"
#include "../include/dashboard.h"
#include "../include/patient.h"
#include "../include/triage.h"
#include "../include/emergency.h"
#include "../../include/PatientManager.h"
#include "../../include/PriorityQueue.h"

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
    : QMainWindow(parent),
      pages(nullptr),
      patientManager(new PatientManager()),
      priorityQueue(new PriorityQueue()),
      patientPage(nullptr),
      assessmentPage(nullptr)
{
    setWindowTitle("Hospital Emergency Management System");

    // ✏️ MANUAL CHANGE: Default application window size
    resize(1400, 850);

    // Initial demo patient data if empty
    if (patientManager->checkEmpty())
    {
        patientManager->addPatient(new Patient(0, "Vannak Kim", 58, "Male", "012345678", "Phnom Penh"));
        patientManager->addPatient(new Patient(0, "Dara Pich", 45, "Male", "012345679", "Kandal"));
        patientManager->addPatient(new Patient(0, "Bopha Chann", 29, "Female", "012345680", "Siem Reap"));
        patientManager->addPatient(new Patient(0, "SreyLeak Meas", 62, "Female", "012345681", "Battambang"));
        patientManager->addPatient(new Patient(0, "Rathana Heng", 24, "Male", "012345682", "Phnom Penh"));
    }

    setupUI();
}

MainWindow::~MainWindow()
{
    delete priorityQueue;
    delete patientManager;
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

    // Page 1: Patient Management (connected to patientManager)
    patientPage = new PatientPage(patientManager, this);
    pages->addWidget(patientPage);

    // Page 2: Emergency Assessment (Merged Triage Header + Queue Table & Actions)
    assessmentPage = new EmergencyAssessment(patientManager, priorityQueue, this);
    pages->addWidget(assessmentPage);

    // Synchronize pages when patient data changes
    connect(patientPage, &PatientPage::dataChanged, assessmentPage, &EmergencyAssessment::refreshAssessment);
    connect(assessmentPage, &EmergencyAssessment::dataChanged, patientPage, &PatientPage::refreshTable);

    // Page 3: Treatment Management (styled placeholder card)
    QWidget *treatmentPage = new QWidget();
    QVBoxLayout *treatmentLayout = new QVBoxLayout(treatmentPage);
    treatmentLayout->setContentsMargins(20, 20, 20, 20);
    treatmentLayout->setSpacing(15);

    QLabel *treatmentTitle = new QLabel("Treatment Management");
    treatmentTitle->setObjectName("pageTitle");

    QLabel *treatmentSubtitle = new QLabel("Manage active patient treatments and clinical care records");
    treatmentSubtitle->setObjectName("pageSubtitle");

    treatmentLayout->addWidget(treatmentTitle);
    treatmentLayout->addWidget(treatmentSubtitle);

    QFrame *treatmentCard = new QFrame();
    treatmentCard->setObjectName("contentPanel");
    QVBoxLayout *cardLayout = new QVBoxLayout(treatmentCard);
    cardLayout->setContentsMargins(30, 40, 30, 40);
    cardLayout->setAlignment(Qt::AlignCenter);

    QLabel *placeholderIcon = new QLabel("✚");
    placeholderIcon->setStyleSheet("font-size: 42px; color: #B58A35;");
    placeholderIcon->setAlignment(Qt::AlignCenter);

    QLabel *placeholderText = new QLabel("Treatment module is ready for doctor assignment and clinical care tracking.");
    placeholderText->setStyleSheet("font-size: 15px; font-weight: bold; color: #172337; margin-top: 10px;");
    placeholderText->setAlignment(Qt::AlignCenter);

    QLabel *placeholderSub = new QLabel("Patients discharged from Emergency Queue will be monitored here.");
    placeholderSub->setStyleSheet("font-size: 13px; color: #7B8798; margin-top: 5px;");
    placeholderSub->setAlignment(Qt::AlignCenter);

    cardLayout->addWidget(placeholderIcon);
    cardLayout->addWidget(placeholderText);
    cardLayout->addWidget(placeholderSub);

    treatmentLayout->addWidget(treatmentCard);
    treatmentLayout->addStretch();
    pages->addWidget(treatmentPage);

    // ================= NAVIGATION BUTTONS =================

    // ✏️ MANUAL CHANGE:
    // Edit these symbols and labels to customize navigation.
    QStringList menuItems = {
        "▦    Dashboard",
        "♙    Patient Management",
        "♥    Emergency Assessment",
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
