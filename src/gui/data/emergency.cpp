#include "../include/emergency.h"
#include "../../include/PatientManager.h"
#include "../../include/PriorityQueue.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QFont>

EmergencyAssessment::EmergencyAssessment(PatientManager *mgr, PriorityQueue *pq, QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      priorityQueue(pq),
      ownsBackend(false),
      selectedPriorityFilter(0),
      mainLayout(nullptr),
      resuscitationCount(nullptr),
      emergentCount(nullptr),
      urgentCount(nullptr),
      lessUrgentCount(nullptr),
      nonUrgentCount(nullptr),
      searchInput(nullptr),
      addButton(nullptr),
      treatButton(nullptr),
      filterAllBtn(nullptr),
      filterResuscitationBtn(nullptr),
      filterEmergentBtn(nullptr),
      filterUrgentBtn(nullptr),
      filterLessUrgentBtn(nullptr),
      filterNonUrgentBtn(nullptr),
      filterStatusLabel(nullptr),
      patientTable(nullptr),
      queueTotalLabel(nullptr),
      removeButton(nullptr)
{
    if (!patientManager || !priorityQueue)
    {
        patientManager = new PatientManager();
        priorityQueue = new PriorityQueue();
        ownsBackend = true;
    }

    loadSampleDataIfEmpty();
    setupUI();
    displayPatients();
    updatePriorityCounts();
}

EmergencyAssessment::~EmergencyAssessment()
{
    if (ownsBackend)
    {
        delete priorityQueue;
        delete patientManager;
    }
}

void EmergencyAssessment::setBackend(PatientManager *mgr, PriorityQueue *pq)
{
    if (ownsBackend)
    {
        delete priorityQueue;
        delete patientManager;
        ownsBackend = false;
    }
    patientManager = mgr;
    priorityQueue = pq;
    loadSampleDataIfEmpty();
    displayPatients();
    updatePriorityCounts();
}

void EmergencyAssessment::refreshAssessment()
{
    displayPatients();
    updatePriorityCounts();
}

void EmergencyAssessment::loadSampleDataIfEmpty()
{
    if (!priorityQueue || priorityQueue->getQueueCount() > 0)
        return;

    struct SampleEntry
    {
        std::string name;
        int age;
        std::string gender;
        std::string symptoms;
        TriagePriority priority;
        double temp;
        int hr;
        int sys;
        int dia;
        int o2;
    };

    SampleEntry samples[] = {
        {"Vannak Kim", 58, "Male", "Acute myocardial infarction, chest pressure", RESUSCITATION, 36.8, 142, 85, 50, 84},
        {"Dara Pich", 45, "Male", "Severe respiratory distress and cyanosis", EMERGENT, 38.5, 118, 155, 95, 89},
        {"Bopha Chann", 29, "Female", "Deep arterial laceration with continuous bleed", URGENT, 37.2, 98, 110, 72, 97},
        {"SreyLeak Meas", 62, "Female", "Persistent sharp abdominal pain and nausea", LESS_URGENT, 37.6, 84, 130, 85, 98},
        {"Rathana Heng", 24, "Male", "Closed ankle sprain and minor abrasions", NON_URGENT, 36.9, 72, 120, 80, 99}};

    for (const auto &s : samples)
    {
        int pid = 0;
        if (patientManager)
        {
            Patient *p = new Patient{0, s.name, s.age, s.gender, "012345678", "Phnom Penh"};
            pid = patientManager->addPatient(p);
        }

        TriageRecord *tr = new TriageRecord{
            pid, s.symptoms, s.temp, s.hr, s.sys, s.dia, 18, s.o2, s.priority};
        priorityQueue->enqueuePatient(tr);
    }
}

void EmergencyAssessment::setupUI()
{
    setObjectName("emergencyPage");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(14);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // ==========================================================
    // 1. EMERGENCY TRIAGE AS HEADER (TITLE + 5 PRIORITY CARDS)
    // ==========================================================
    setupTriageHeaderCards();

    // ==========================================================
    // 2. BELOW HEADER: EMERGENCY QUEUE (SEARCH, ADD, NEXT)
    // ==========================================================
    setupQueueActionControls();

    // ==========================================================
    // 3. AFTER THAT: FILTER CATEGORY FROM EMERGENCY TRIAGE
    // ==========================================================
    setupCategoryFilterButtons();

    // ==========================================================
    // 4. ACTIVE EMERGENCY QUEUE TABLE
    // ==========================================================
    setupQueueTable();

    // ==========================================================
    // 5. BOTTOM ACTION BAR
    // ==========================================================
    setupBottomActionBar();
}

void EmergencyAssessment::setupTriageHeaderCards()
{
    // Page Title & Subtitle
    QLabel *title = new QLabel("Emergency Assessment");
    title->setObjectName("pageTitle");

    QLabel *subtitle = new QLabel("Emergency Triage Evaluation & Active Queue Management");
    subtitle->setObjectName("pageSubtitle");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // 5-Level Priority Summary Cards
    QHBoxLayout *cardLayout = new QHBoxLayout();
    cardLayout->setSpacing(12);

    auto createCard = [this](const QString &levelTitle, const QString &timeStr,
                             const QString &color, QLabel *&countRef, int priorityLevel) -> QFrame *
    {
        QFrame *card = new QFrame();
        card->setObjectName("dashboardCard");
        card->setCursor(Qt::PointingHandCursor);
        card->setStyleSheet(
            "QFrame#dashboardCard {"
            "background-color: #FFFFFF;"
            "border: 1px solid #E6E0D2;"
            "border-radius: 12px;"
            "padding: 6px;"
            "}"
            "QFrame#dashboardCard:hover { border: 1.5px solid #D9AD52; }");

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setSpacing(4);
        layout->setContentsMargins(14, 10, 14, 10);

        QHBoxLayout *topRow = new QHBoxLayout();
        QLabel *dot = new QLabel("●");
        dot->setStyleSheet("color: " + color + "; font-size: 13px;");
        QLabel *name = new QLabel(levelTitle);
        name->setStyleSheet("font-size: 12px; font-weight: bold; color: " + color + "; letter-spacing: 0.5px;");
        topRow.addWidget(dot);
        topRow.addWidget(name);
        topRow.addStretch();

        countRef = new QLabel("0");
        countRef->setStyleSheet("font-size: 26px; font-weight: bold; color: #B58A35;");

        QLabel *desc = new QLabel("Target: " + timeStr);
        desc->setStyleSheet("font-size: 11px; color: #7B8798;");

        layout->addLayout(topRow);
        layout->addWidget(countRef);
        layout->addWidget(desc);

        // Click card to quick filter
        card->installEventFilter(this);

        return card;
    };

    cardLayout->addWidget(createCard("RESUSCITATION (L1)", "Immediate", "#EF4444", resuscitationCount, 1));
    cardLayout->addWidget(createCard("EMERGENT (L2)", "< 15 min", "#F97316", emergentCount, 2));
    cardLayout->addWidget(createCard("URGENT (L3)", "< 30 min", "#EAB308", urgentCount, 3));
    cardLayout->addWidget(createCard("LESS URGENT (L4)", "< 60 min", "#22C55E", lessUrgentCount, 4));
    cardLayout->addWidget(createCard("NON-URGENT (L5)", "< 120 min", "#3B82F6", nonUrgentCount, 5));

    mainLayout->addLayout(cardLayout);
}

void EmergencyAssessment::setupQueueActionControls()
{
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(12);

    // Search bar
    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search queue by Patient ID, Name, or Symptoms...");
    searchInput->setMinimumHeight(42);
    searchInput->setClearButtonEnabled(true);
    connect(searchInput, &QLineEdit::textChanged, this, &EmergencyAssessment::displayPatients);

    // Add Patient button
    addButton = new QPushButton("+ Add Emergency Patient");
    addButton->setObjectName("primaryGoldButton");
    addButton->setMinimumHeight(42);
    addButton->setCursor(Qt::PointingHandCursor);
    connect(addButton, &QPushButton::clicked, this, &EmergencyAssessment::addPatient);

    // Treat Next button
    treatButton = new QPushButton("⚡ Treat Next Patient");
    treatButton->setObjectName("secondaryNavyButton");
    treatButton->setMinimumHeight(42);
    treatButton->setCursor(Qt::PointingHandCursor);
    connect(treatButton, &QPushButton::clicked, this, &EmergencyAssessment::treatNextPatient);

    controlLayout->addWidget(searchInput, 3);
    controlLayout->addWidget(addButton, 1);
    controlLayout->addWidget(treatButton, 1);

    mainLayout->addLayout(controlLayout);
}

void EmergencyAssessment::setupCategoryFilterButtons()
{
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(8);

    QLabel *filterLabel = new QLabel("Filter Priority:");
    filterLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #172337; margin-right: 4px;");
    filterLayout->addWidget(filterLabel);

    filterAllBtn = new QPushButton("All Priorities");
    filterResuscitationBtn = new QPushButton("Level 1: Resuscitation");
    filterEmergentBtn = new QPushButton("Level 2: Emergent");
    filterUrgentBtn = new QPushButton("Level 3: Urgent");
    filterLessUrgentBtn = new QPushButton("Level 4: Less Urgent");
    filterNonUrgentBtn = new QPushButton("Level 5: Non-Urgent");

    QList<QPushButton *> buttons = {
        filterAllBtn, filterResuscitationBtn, filterEmergentBtn,
        filterUrgentBtn, filterLessUrgentBtn, filterNonUrgentBtn};

    for (int i = 0; i < buttons.size(); i++)
    {
        QPushButton *btn = buttons[i];
        btn->setMinimumHeight(34);
        btn->setCursor(Qt::PointingHandCursor);
        filterLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, i]()
                { setPriorityFilter(i); });
    }

    filterLayout->addStretch();

    filterStatusLabel = new QLabel("Showing: All Priorities");
    filterStatusLabel->setObjectName("goldBadge");
    filterLayout->addWidget(filterStatusLabel);

    mainLayout->addLayout(filterLayout);

    updateFilterButtonStyles();
}

void EmergencyAssessment::setPriorityFilter(int priorityLevel)
{
    selectedPriorityFilter = priorityLevel;
    updateFilterButtonStyles();
    displayPatients();
}

void EmergencyAssessment::updateFilterButtonStyles()
{
    QList<QPushButton *> buttons = {
        filterAllBtn, filterResuscitationBtn, filterEmergentBtn,
        filterUrgentBtn, filterLessUrgentBtn, filterNonUrgentBtn};

    QList<QString> activeColors = {
        "#1E2B3E", // All
        "#EF4444", // L1
        "#F97316", // L2
        "#D97706", // L3
        "#16A34A", // L4
        "#2563EB"  // L5
    };

    for (int i = 0; i < buttons.size(); i++)
    {
        QPushButton *btn = buttons[i];
        if (i == selectedPriorityFilter)
        {
            btn->setStyleSheet(
                QString("QPushButton {"
                        "background-color: %1;"
                        "color: #FFFFFF;"
                        "border: 1px solid %1;"
                        "border-radius: 6px;"
                        "padding: 6px 14px;"
                        "font-weight: bold;"
                        "font-size: 12px;"
                        "}")
                    .arg(activeColors[i]));
        }
        else
        {
            btn->setStyleSheet(
                "QPushButton {"
                "background-color: #FFFFFF;"
                "color: #475569;"
                "border: 1px solid #DCD5C5;"
                "border-radius: 6px;"
                "padding: 6px 14px;"
                "font-weight: 500;"
                "font-size: 12px;"
                "}"
                "QPushButton:hover {"
                "background-color: #F8FAFC;"
                "border-color: #C49A45;"
                "color: #172337;"
                "}");
        }
    }

    if (filterStatusLabel)
    {
        if (selectedPriorityFilter == 0)
            filterStatusLabel->setText("Showing: All Priorities");
        else
            filterStatusLabel->setText("Filtered: " + getPriorityName(static_cast<TriagePriority>(selectedPriorityFilter)));
    }
}

void EmergencyAssessment::setupQueueTable()
{
    patientTable = new QTableWidget();
    patientTable->setObjectName("patientTable");
    patientTable->setColumnCount(7);
    patientTable->setHorizontalHeaderLabels({
        "Patient ID",
        "Full Name",
        "Age / Gender",
        "Triage Priority",
        "Symptoms",
        "Vital Signs",
        "Queue Status"
    });

    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    patientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    patientTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    patientTable->verticalHeader()->setVisible(false);
    patientTable->setAlternatingRowColors(true);

    connect(patientTable, &QTableWidget::cellDoubleClicked, [this](int row, int)
            {
        if (row >= 0 && row < patientTable->rowCount())
        {
            int pid = patientTable->item(row, 0)->text().toInt();
            viewPatientDetails(pid);
        } });

    mainLayout->addWidget(patientTable);
}

void EmergencyAssessment::setupBottomActionBar()
{
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    queueTotalLabel = new QLabel("Active Queue: 0 Patients");
    queueTotalLabel->setObjectName("goldBadge");

    removeButton = new QPushButton("Remove Selected from Queue");
    removeButton->setObjectName("dangerButton");
    removeButton->setMinimumHeight(38);
    removeButton->setCursor(Qt::PointingHandCursor);
    connect(removeButton, &QPushButton::clicked, this, &EmergencyAssessment::removeSelectedPatient);

    bottomLayout->addWidget(queueTotalLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(removeButton);

    mainLayout->addLayout(bottomLayout);
}

QString EmergencyAssessment::getPriorityName(TriagePriority priority) const
{
    switch (priority)
    {
    case RESUSCITATION:
        return "Level 1: Resuscitation";
    case EMERGENT:
        return "Level 2: Emergent";
    case URGENT:
        return "Level 3: Urgent";
    case LESS_URGENT:
        return "Level 4: Less Urgent";
    case NON_URGENT:
        return "Level 5: Non-Urgent";
    default:
        return "Unknown";
    }
}

QString EmergencyAssessment::getPriorityColor(TriagePriority priority) const
{
    switch (priority)
    {
    case RESUSCITATION:
        return "#EF4444";
    case EMERGENT:
        return "#F97316";
    case URGENT:
        return "#EAB308";
    case LESS_URGENT:
        return "#22C55E";
    case NON_URGENT:
        return "#3B82F6";
    default:
        return "#64748B";
    }
}

QString EmergencyAssessment::getPriorityBgColor(TriagePriority priority) const
{
    switch (priority)
    {
    case RESUSCITATION:
        return "#FEF2F2";
    case EMERGENT:
        return "#FFF7ED";
    case URGENT:
        return "#FEFCE8";
    case LESS_URGENT:
        return "#F0FDF4";
    case NON_URGENT:
        return "#EFF6FF";
    default:
        return "#F8FAFC";
    }
}

void EmergencyAssessment::updatePriorityCounts()
{
    if (!priorityQueue)
        return;

    std::array<int, 5> counts = priorityQueue->getPriorityCounts();

    if (resuscitationCount)
        resuscitationCount->setText(QString::number(counts[0]));
    if (emergentCount)
        emergentCount->setText(QString::number(counts[1]));
    if (urgentCount)
        urgentCount->setText(QString::number(counts[2]));
    if (lessUrgentCount)
        lessUrgentCount->setText(QString::number(counts[3]));
    if (nonUrgentCount)
        nonUrgentCount->setText(QString::number(counts[4]));

    // Update counts inside category buttons
    if (filterAllBtn)
        filterAllBtn->setText(QString("All (%1)").arg(priorityQueue->getQueueCount()));
    if (filterResuscitationBtn)
        filterResuscitationBtn->setText(QString("L1: Resuscitation (%1)").arg(counts[0]));
    if (filterEmergentBtn)
        filterEmergentBtn->setText(QString("L2: Emergent (%1)").arg(counts[1]));
    if (filterUrgentBtn)
        filterUrgentBtn->setText(QString("L3: Urgent (%1)").arg(counts[2]));
    if (filterLessUrgentBtn)
        filterLessUrgentBtn->setText(QString("L4: Less Urgent (%1)").arg(counts[3]));
    if (filterNonUrgentBtn)
        filterNonUrgentBtn->setText(QString("L5: Non-Urgent (%1)").arg(counts[4]));
}

void EmergencyAssessment::displayPatients()
{
    patientTable->setRowCount(0);

    if (!priorityQueue || !patientManager)
        return;

    QString search = searchInput ? searchInput->text().trimmed().toLower() : "";
    int totalQueue = priorityQueue->getQueueCount();
    int matchCount = 0;

    for (int i = 0; i < totalQueue; i++)
    {
        TriageRecord *triage = priorityQueue->getQueueRecordAt(i);
        if (!triage)
            continue;

        // Apply 5-Level Priority Filter
        if (selectedPriorityFilter > 0 && static_cast<int>(triage->priority) != selectedPriorityFilter)
            continue;

        Patient *patient = patientManager->findPatientInfo(triage->patientID);
        QString pidStr = QString::number(triage->patientID);
        QString nameStr = patient ? QString::fromStdString(patient->name) : "Unknown Patient";
        QString symptomsStr = QString::fromStdString(triage->symptoms);

        // Apply Search Filter
        if (!search.isEmpty())
        {
            if (!pidStr.contains(search) &&
                !nameStr.toLower().contains(search) &&
                !symptomsStr.toLower().contains(search))
            {
                continue;
            }
        }

        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        // 0: Patient ID
        patientTable->setItem(row, 0, new QTableWidgetItem(pidStr));

        // 1: Full Name
        QTableWidgetItem *nameItem = new QTableWidgetItem(nameStr);
        QFont f = nameItem->font();
        f.setBold(true);
        nameItem->setFont(f);
        patientTable->setItem(row, 1, nameItem);

        // 2: Age / Gender
        QString ageGender = QString("%1 yrs / %2")
                                .arg(patient ? QString::number(patient->age) : "-")
                                .arg(patient ? QString::fromStdString(patient->gender) : "-");
        patientTable->setItem(row, 2, new QTableWidgetItem(ageGender));

        // 3: Triage Priority (Styled Colored Pill)
        QTableWidgetItem *pItem = new QTableWidgetItem(getPriorityName(triage->priority));
        pItem->setForeground(QColor(getPriorityColor(triage->priority)));
        pItem->setBackground(QColor(getPriorityBgColor(triage->priority)));
        QFont pFont = pItem->font();
        pFont.setBold(true);
        pItem->setFont(pFont);
        patientTable->setItem(row, 3, pItem);

        // 4: Symptoms
        patientTable->setItem(row, 4, new QTableWidgetItem(symptomsStr));

        // 5: Vital Signs
        QString vitals = QString("HR: %1 | BP: %2/%3 | SpO2: %4% | Temp: %5°C")
                             .arg(triage->heartRate)
                             .arg(triage->bloodPressureSystolic)
                             .arg(triage->bloodPressureDiastolic)
                             .arg(triage->oxygenSaturation)
                             .arg(triage->temperature);
        patientTable->setItem(row, 5, new QTableWidgetItem(vitals));

        // 6: Queue Status
        QString queueState = (i == 0) ? "Queue #1 (Next to Treat)" : QString("Queue #%1 (Waiting)").arg(i + 1);
        QTableWidgetItem *statusItem = new QTableWidgetItem(queueState);
        if (i == 0)
        {
            statusItem->setForeground(QColor("#B58A35"));
            QFont sf = statusItem->font();
            sf.setBold(true);
            statusItem->setFont(sf);
        }
        patientTable->setItem(row, 6, statusItem);

        matchCount++;
    }

    if (queueTotalLabel)
    {
        queueTotalLabel->setText(
            QString("Total Queue: %1 Patients (Showing: %2)")
                .arg(totalQueue)
                .arg(matchCount));
    }
}

void EmergencyAssessment::addPatient()
{
    if (!patientManager || !priorityQueue)
    {
        QMessageBox::critical(this, "Error", "Backend data services are not initialized.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Triage Assessment & Enqueue Patient");
    dialog.setMinimumWidth(460);

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setPlaceholderText("e.g. Sok Dara");

    QSpinBox ageSpin;
    ageSpin.setRange(0, 130);
    ageSpin.setValue(35);

    QComboBox genderCombo;
    genderCombo.addItems({"Male", "Female", "Other"});

    QLineEdit symptomsEdit;
    symptomsEdit.setPlaceholderText("e.g. Chest pain radiating to left arm");

    QComboBox priorityCombo;
    priorityCombo.addItems({
        "Level 1: Resuscitation (Immediate)",
        "Level 2: Emergent (< 15 min)",
        "Level 3: Urgent (< 30 min)",
        "Level 4: Less Urgent (< 60 min)",
        "Level 5: Non-Urgent (< 120 min)"
    });
    priorityCombo.setCurrentIndex(0);

    QSpinBox hrSpin;
    hrSpin.setRange(30, 240);
    hrSpin.setValue(85);

    QSpinBox sysSpin;
    sysSpin.setRange(50, 260);
    sysSpin.setValue(120);

    QSpinBox diaSpin;
    diaSpin.setRange(30, 160);
    diaSpin.setValue(80);

    QDoubleSpinBox tempSpin;
    tempSpin.setRange(30.0, 45.0);
    tempSpin.setValue(37.0);
    tempSpin.setSingleStep(0.1);

    QSpinBox o2Spin;
    o2Spin.setRange(50, 100);
    o2Spin.setValue(98);

    form.addRow("Patient Name *:", &nameEdit);
    form.addRow("Age *:", &ageSpin);
    form.addRow("Gender:", &genderCombo);
    form.addRow("Symptoms / Complaint *:", &symptomsEdit);
    form.addRow("Triage Priority Level *:", &priorityCombo);
    form.addRow("Heart Rate (BPM):", &hrSpin);
    form.addRow("Blood Pressure (Systolic):", &sysSpin);
    form.addRow("Blood Pressure (Diastolic):", &diaSpin);
    form.addRow("Temperature (°C):", &tempSpin);
    form.addRow("Oxygen Saturation (%):", &o2Spin);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Enqueue Emergency Patient");
    QPushButton cancelBtn("Cancel");
    saveBtn.setStyleSheet("background-color: #C49A45; color: white; font-weight: bold; padding: 7px 18px; border: 1px solid #B58A35; border-radius: 6px;");
    cancelBtn.setStyleSheet("background-color: #FFFFFF; color: #667085; font-weight: bold; padding: 7px 18px; border: 1px solid #DCD5C5; border-radius: 6px;");
    btnLayout.addStretch();
    btnLayout.addWidget(&cancelBtn);
    btnLayout.addWidget(&saveBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]()
            {
        if (nameEdit.text().trimmed().isEmpty())
        {
            QMessageBox::warning(&dialog, "Input Error", "Patient name cannot be empty.");
            return;
        }

        // 1. Register in PatientManager
        Patient *p = new Patient{
            0,
            nameEdit.text().trimmed().toStdString(),
            ageSpin.value(),
            genderCombo.currentText().toStdString(),
            "012345678",
            "Phnom Penh"
        };
        int pid = patientManager->addPatient(p);

        // 2. Create TriageRecord and Enqueue to PriorityQueue
        TriagePriority prio = static_cast<TriagePriority>(priorityCombo.currentIndex() + 1);
        TriageRecord *tr = new TriageRecord{
            pid,
            symptomsEdit.text().trimmed().toStdString(),
            tempSpin.value(),
            hrSpin.value(),
            sysSpin.value(),
            diaSpin.value(),
            18,
            o2Spin.value(),
            prio
        };

        priorityQueue->enqueuePatient(tr);

        QMessageBox::information(
            this, "Patient Enqueued",
            QString("Patient #%1 (%2) successfully registered and enqueued under %3.")
                .arg(pid)
                .arg(nameEdit.text().trimmed())
                .arg(getPriorityName(prio)));

        dialog.accept();
        displayPatients();
        updatePriorityCounts();
        emit dataChanged(); });

    dialog.exec();
}

void EmergencyAssessment::treatNextPatient()
{
    if (!priorityQueue || priorityQueue->checkEmpty())
    {
        QMessageBox::information(this, "Empty Queue", "There are currently no patients waiting in the emergency queue.");
        return;
    }

    TriageRecord *triage = priorityQueue->treatNextPatient();
    if (!triage)
        return;

    Patient *p = patientManager ? patientManager->findPatientInfo(triage->patientID) : nullptr;
    QString name = p ? QString::fromStdString(p->name) : "Unknown";
    QString prioName = getPriorityName(triage->priority);

    QMessageBox::information(
        this, "Treating Patient",
        QString("Now Treating Patient #%1 (%2)\n"
                "Priority: %3\n"
                "Symptoms: %4\n\n"
                "Patient record successfully transferred to Treatment Management.")
            .arg(triage->patientID)
            .arg(name)
            .arg(prioName)
            .arg(QString::fromStdString(triage->symptoms)));

    delete triage;

    displayPatients();
    updatePriorityCounts();
    emit dataChanged();
}

void EmergencyAssessment::removeSelectedPatient()
{
    int row = patientTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient from the queue table to remove.");
        return;
    }

    int pid = patientTable->item(row, 0)->text().toInt();
    QString name = patientTable->item(row, 1)->text();

    auto reply = QMessageBox::question(
        this, "Confirm Removal",
        QString("Are you sure you want to remove patient #%1 (%2) from the emergency queue?").arg(pid).arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (priorityQueue->removePatient(pid))
        {
            QMessageBox::information(this, "Removed", QString("Patient #%1 removed from queue.").arg(pid));
        }
        else
        {
            QMessageBox::warning(this, "Notice", "Could not remove patient from queue.");
        }

        displayPatients();
        updatePriorityCounts();
        emit dataChanged();
    }
}

void EmergencyAssessment::viewPatientDetails(int patientID)
{
    if (!priorityQueue || !patientManager)
        return;

    Patient *p = patientManager->findPatientInfo(patientID);
    TriageRecord *triage = nullptr;

    for (int i = 0; i < priorityQueue->getQueueCount(); i++)
    {
        TriageRecord *r = priorityQueue->getQueueRecordAt(i);
        if (r && r->patientID == patientID)
        {
            triage = r;
            break;
        }
    }

    if (!p || !triage)
        return;

    QString details = QString(
                          "================ CLINICAL TRIAGE RECORD ================\n\n"
                          "Patient ID: %1\n"
                          "Name: %2\n"
                          "Age / Gender: %3 yrs / %4\n"
                          "Phone: %5\n"
                          "Address: %6\n\n"
                          "Triage Priority: %7\n"
                          "Primary Symptoms: %8\n\n"
                          "Vital Signs:\n"
                          "  • Heart Rate: %9 BPM\n"
                          "  • Blood Pressure: %10/%11 mmHg\n"
                          "  • Body Temperature: %12 °C\n"
                          "  • Oxygen Saturation (SpO2): %13 %")
                          .arg(p->id)
                          .arg(QString::fromStdString(p->name))
                          .arg(p->age)
                          .arg(QString::fromStdString(p->gender))
                          .arg(QString::fromStdString(p->phone))
                          .arg(QString::fromStdString(p->address))
                          .arg(getPriorityName(triage->priority))
                          .arg(QString::fromStdString(triage->symptoms))
                          .arg(triage->heartRate)
                          .arg(triage->bloodPressureSystolic)
                          .arg(triage->bloodPressureDiastolic)
                          .arg(triage->temperature)
                          .arg(triage->oxygenSaturation);

    QMessageBox::information(this, "Patient Clinical Record", details);
}
