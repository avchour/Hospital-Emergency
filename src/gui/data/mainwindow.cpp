#include "../include/mainwindow.h"
#include "../include/dashboard.h"
#include "../include/patient.h"
#include "../include/triage.h"
#include "../include/emergency.h"
#include "../include/treatment.h"
#include "../include/room.h"

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dashboardPage(nullptr),
      patientPage(nullptr),
      triagePage(nullptr),
      emergencyQueuePage(nullptr),
      treatmentPage(nullptr),
      roomPage(nullptr),
      pages(nullptr)
{
    setWindowTitle("Hospital Emergency Management System");
    resize(1400, 850);

    initSampleData();
    setupUI();
}

MainWindow::~MainWindow()
{
    clearQueue(normalQueue);
}

void MainWindow::initSampleData()
{
    // Register initial core patients
    Patient *p1 = new Patient{0, "Vannak Kim", 58, "Male", "012888999", "Phnom Penh"};
    int id1 = patientManager.addPatient(p1);
    priorityQueue.enqueuePatient(new TriageRecord{
        id1, "Acute myocardial infarction with cardiac arrest", 36.8, 142, 85, 50, 24, 84, RESUSCITATION
    });

    Patient *p2 = new Patient{0, "Dara Pich", 45, "Male", "011223344", "Kandal"};
    int id2 = patientManager.addPatient(p2);
    priorityQueue.enqueuePatient(new TriageRecord{
        id2, "Severe respiratory distress and cyanosis", 38.5, 118, 155, 95, 22, 89, EMERGENT
    });

    Patient *p3 = new Patient{0, "Bopha Chann", 29, "Female", "077889900", "Siem Reap"};
    int id3 = patientManager.addPatient(p3);
    priorityQueue.enqueuePatient(new TriageRecord{
        id3, "Uncontrolled bleeding from forearm laceration", 37.2, 98, 110, 72, 18, 97, URGENT
    });

    Patient *p4 = new Patient{0, "SreyLeak Meas", 62, "Female", "092112233", "Battambang"};
    int id4 = patientManager.addPatient(p4);
    priorityQueue.enqueuePatient(new TriageRecord{
        id4, "Persistent severe abdominal pain with guarding", 37.6, 84, 130, 85, 18, 98, LESS_URGENT
    });

    Patient *p5 = new Patient{0, "Rathana Heng", 24, "Male", "099445566", "Kampot"};
    int id5 = patientManager.addPatient(p5);
    priorityQueue.enqueuePatient(new TriageRecord{
        id5, "Minor ankle sprain and superficial abrasion", 36.9, 72, 120, 80, 16, 99, NON_URGENT
    });

    // Outpatient Normal Queue
    Patient *p6 = new Patient{0, "Chan Thou", 34, "Female", "015667788", "Phnom Penh"};
    int id6 = patientManager.addPatient(p6);
    enqueue(normalQueue, id6);

    Patient *p7 = new Patient{0, "Sokha Chea", 51, "Male", "088776655", "Takeo"};
    int id7 = patientManager.addPatient(p7);
    enqueue(normalQueue, id7);

    Patient *p8 = new Patient{0, "Kosal Voeun", 40, "Male", "012334455", "Kampong Cham"};
    int id8 = patientManager.addPatient(p8);
    enqueue(normalQueue, id8);
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Sidebar
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(260);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(18, 25, 18, 20);
    sidebarLayout->setSpacing(10);

    // Brand Header
    QWidget *brandWidget = new QWidget();
    QHBoxLayout *brandLayout = new QHBoxLayout(brandWidget);
    brandLayout->setContentsMargins(2, 0, 2, 10);
    brandLayout->setSpacing(12);

    QLabel *logoIcon = new QLabel("+");
    logoIcon->setObjectName("brandIcon");
    logoIcon->setStyleSheet("font-size: 26px; font-weight: bold; color: #DC2626;");

    QVBoxLayout *brandTextLayout = new QVBoxLayout();
    brandTextLayout->setSpacing(2);

    QLabel *brandTitle = new QLabel("HOSPITAL-EMERGENCY");
    brandTitle->setStyleSheet("font-size: 13px; font-weight: bold; color: #1E293B; letter-spacing: 0.5px;");

    QLabel *brandSubtitle = new QLabel("Emergency & Queue System");
    brandSubtitle->setStyleSheet("font-size: 11px; color: #64748B;");

    brandTextLayout->addWidget(brandTitle);
    brandTextLayout->addWidget(brandSubtitle);

    brandLayout->addWidget(logoIcon);
    brandLayout->addLayout(brandTextLayout);
    brandLayout->addStretch();

    sidebarLayout->addWidget(brandWidget);

    QLabel *menuLabel = new QLabel("MAIN MENU");
    menuLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #94A3B8; margin-top: 10px;");
    sidebarLayout->addWidget(menuLabel);

    // Pages stack
    pages = new QStackedWidget();

    // 0: Dashboard
    dashboardPage = new Dashboard(
        &patientManager,
        &priorityQueue,
        &normalQueue,
        [this](int index) {
            changePage(index);
            setActiveButton(index);
        }
    );
    pages->addWidget(dashboardPage);

    // 1: Patient Management
    patientPage = new PatientPage(&patientManager);
    pages->addWidget(patientPage);

    // 2: Emergency Triage
    triagePage = new Emergency(&patientManager, &priorityQueue);
    pages->addWidget(triagePage);

    // 3: Emergency Queue
    emergencyQueuePage = new EmergencyQueue(&patientManager, &priorityQueue);
    pages->addWidget(emergencyQueuePage);

    // 4: Treatment Management
    treatmentPage = new TreatmentPage(&patientManager, &normalQueue);
    pages->addWidget(treatmentPage);

    // 5: Room Management
    roomPage = new RoomPage(&patientManager);
    pages->addWidget(roomPage);

    // Connect cross-page synchronization
    connect(patientPage, &PatientPage::dataChanged, [this]() {
        dashboardPage->refreshStats();
        emergencyQueuePage->refreshQueue();
        triagePage->refreshData();
        treatmentPage->refreshQueue();
        roomPage->refreshPatients();
    });

    connect(emergencyQueuePage, &EmergencyQueue::dataChanged, [this]() {
        dashboardPage->refreshStats();
        triagePage->refreshData();
    });

    connect(treatmentPage, &TreatmentPage::dataChanged, [this]() {
        dashboardPage->refreshStats();
        patientPage->refreshTable();
    });

    connect(roomPage, &RoomPage::dataChanged, [this]() {
        dashboardPage->refreshStats();
    });

    // Navigation buttons
    QStringList menuItems = {
        "▦    Dashboard",
        "♙    Patient Management",
        "♥    Emergency Triage",
        "☷    Emergency Queue",
        "✚    Treatment Management",
        "🛏    Room Management"
    };

    navButtons.clear();

    for (int i = 0; i < menuItems.size(); i++)
    {
        QPushButton *button = new QPushButton(menuItems[i]);
        button->setObjectName("navButton");
        button->setCheckable(true);
        button->setMinimumHeight(48);
        button->setCursor(Qt::PointingHandCursor);
        button->setStyleSheet(
            "QPushButton {"
            "text-align: left; padding-left: 18px; font-size: 14px; font-weight: 500;"
            "color: #475569; border: none; border-radius: 8px; background: transparent;"
            "}"
            "QPushButton:hover { background-color: #F1F5F9; color: #1E293B; }"
            "QPushButton:checked { background-color: #EFF6FF; color: #2563EB; font-weight: bold; }"
        );

        sidebarLayout->addWidget(button);
        navButtons.append(button);

        connect(button, &QPushButton::clicked, this, [this, i]() {
            changePage(i);
            setActiveButton(i);
        });
    }

    sidebarLayout->addStretch();

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(pages);

    pages->setCurrentIndex(0);
    setActiveButton(0);
}

void MainWindow::changePage(int index)
{
    if (!pages || index < 0 || index >= pages->count())
        return;

    pages->setCurrentIndex(index);

    // Dynamic data refresh on page arrival
    switch (index)
    {
    case 0:
        if (dashboardPage) dashboardPage->refreshStats();
        break;
    case 1:
        if (patientPage) patientPage->refreshTable();
        break;
    case 2:
        if (triagePage) triagePage->refreshData();
        break;
    case 3:
        if (emergencyQueuePage) emergencyQueuePage->refreshQueue();
        break;
    case 4:
        if (treatmentPage) treatmentPage->refreshQueue();
        break;
    }
}

void MainWindow::setActiveButton(int activeIndex)
{
    for (int i = 0; i < navButtons.size(); i++)
    {
        navButtons[i]->setChecked(i == activeIndex);
    }
}
