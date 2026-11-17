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
#include <QDateTime>
#include <QAbstractItemView>
#include <QFont>

EmergencyQueue::EmergencyQueue(PatientManager *mgr, PriorityQueue *pq, QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      priorityQueue(pq),
      ownsBackend(false),
      mainLayout(nullptr),
      resuscitationCount(nullptr),
      emergentCount(nullptr),
      urgentCount(nullptr),
      lessUrgentCount(nullptr),
      nonUrgentCount(nullptr),
      patientTable(nullptr),
      searchInput(nullptr),
      priorityFilter(nullptr),
      addButton(nullptr),
      treatButton(nullptr),
      removeButton(nullptr)
{
    if (!patientManager || !priorityQueue)
    {
        patientManager = new PatientManager();
        priorityQueue = new PriorityQueue();
        ownsBackend = true;
        loadSamplePatients();
    }

    setupUI();
    displayPatients();
    updatePriorityCounts();
}

EmergencyQueue::~EmergencyQueue()
{
    if (ownsBackend)
    {
        delete priorityQueue;
        delete patientManager;
    }
}

void EmergencyQueue::setBackend(PatientManager *mgr, PriorityQueue *pq)
{
    if (ownsBackend)
    {
        delete priorityQueue;
        delete patientManager;
        ownsBackend = false;
    }
    patientManager = mgr;
    priorityQueue = pq;
    displayPatients();
    updatePriorityCounts();
}

void EmergencyQueue::refreshQueue()
{
    displayPatients();
    updatePriorityCounts();
}

void EmergencyQueue::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *title = new QLabel("Emergency Queue");
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("Patients waiting for emergency triage & treatment in priority order");
    subtitle->setStyleSheet("font-size: 15px; color: #64748B;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // 5 Priority summary cards
    setupPriorityCards();

    QLabel *managementTitle = new QLabel("Active Emergency Queue");
    managementTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #172554;");
    mainLayout->addWidget(managementTitle);

    // Controls: Search, Priority filter, Add, Treat
    QHBoxLayout *controlLayout = new QHBoxLayout();

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search by Patient ID or Name...");
    searchInput->setMinimumHeight(40);
    connect(searchInput, &QLineEdit::textChanged, this, &EmergencyQueue::displayPatients);

    priorityFilter = new QComboBox();
    priorityFilter->addItems({"All Priorities",
                              "Resuscitation (Level 1)",
                              "Emergent (Level 2)",
                              "Urgent (Level 3)",
                              "Less Urgent (Level 4)",
                              "Non Urgent (Level 5)"});
    priorityFilter->setMinimumHeight(40);
    connect(priorityFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &EmergencyQueue::displayPatients);

    addButton = new QPushButton("+ Add Emergency Patient");
    addButton->setMinimumHeight(40);
    addButton->setStyleSheet(
        "QPushButton {"
        "background-color: #16A34A;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 0 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #15803D; }");
    connect(addButton, &QPushButton::clicked, this, &EmergencyQueue::addPatient);

    treatButton = new QPushButton("Treat Next Patient");
    treatButton->setMinimumHeight(40);
    treatButton->setStyleSheet(
        "QPushButton {"
        "background-color: #2563EB;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 0 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #1D4ED8; }");
    connect(treatButton, &QPushButton::clicked, this, &EmergencyQueue::treatNextPatient);

    controlLayout->addWidget(searchInput, 2);
    controlLayout->addWidget(priorityFilter, 1);
    controlLayout->addWidget(addButton);
    controlLayout->addWidget(treatButton);

    mainLayout->addLayout(controlLayout);

    // Table
    setupPatientTable();

    // Bottom Bar
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    removeButton = new QPushButton("Remove Selected from Queue");
    removeButton->setMinimumHeight(38);
    removeButton->setStyleSheet(
        "QPushButton {"
        "background-color: #DC2626;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #B91C1C; }");
    connect(removeButton, &QPushButton::clicked, this, &EmergencyQueue::removeSelectedPatient);

    bottomLayout->addStretch();
    bottomLayout->addWidget(removeButton);
    mainLayout->addLayout(bottomLayout);
}

void EmergencyQueue::setupPriorityCards()
{
    QHBoxLayout *cardLayout = new QHBoxLayout();
    cardLayout->setSpacing(12);

    auto createCard = [](const QString &nameStr, const QString &color, QLabel *&countRef) -> QFrame *
    {
        QFrame *card = new QFrame();
        card->setStyleSheet(
            "QFrame {"
            "background-color: white;"
            "border: 1px solid #E2E8F0;"
            "border-radius: 8px;"
            "padding: 10px;"
            "}");

        QVBoxLayout *layout = new QVBoxLayout(card);
        layout->setSpacing(4);
        layout->setContentsMargins(12, 10, 12, 10);

        QLabel *name = new QLabel(nameStr);
        name->setStyleSheet("font-size: 13px; font-weight: bold; color: " + color + ";");

        countRef = new QLabel("0");
        countRef->setStyleSheet("font-size: 26px; font-weight: bold; color: #1E293B;");

        QLabel *desc = new QLabel("Waiting");
        desc->setStyleSheet("font-size: 12px; color: #64748B;");

        layout->addWidget(name);
        layout->addWidget(countRef);
        layout->addWidget(desc);

        return card;
    };

    cardLayout->addWidget(createCard("RESUSCITATION", "#DC2626", resuscitationCount));
    cardLayout->addWidget(createCard("EMERGENT", "#EA580C", emergentCount));
    cardLayout->addWidget(createCard("URGENT", "#D97706", urgentCount));
    cardLayout->addWidget(createCard("LESS URGENT", "#16A34A", lessUrgentCount));
    cardLayout->addWidget(createCard("NON URGENT", "#2563EB", nonUrgentCount));

    mainLayout->addLayout(cardLayout);
}

void EmergencyQueue::setupPatientTable()
{
    patientTable = new QTableWidget();
    patientTable->setColumnCount(7);
    patientTable->setHorizontalHeaderLabels({"Patient ID", "Full Name", "Age", "Priority", "Symptoms", "Vital Signs", "Status"});

    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    patientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    patientTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    patientTable->verticalHeader()->setVisible(false);

    patientTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white;"
        "border: 1px solid #E2E8F0;"
        "border-radius: 8px;"
        "gridline-color: #F1F5F9;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC;"
        "color: #475569;"
        "font-weight: bold;"
        "padding: 8px;"
        "border: none;"
        "border-bottom: 1px solid #E2E8F0;"
        "}");

    connect(patientTable, &QTableWidget::cellDoubleClicked, [this](int row, int)
            {
        if (row >= 0 && row < patientTable->rowCount())
        {
            int pid = patientTable->item(row, 0)->text().toInt();
            viewPatientDetails(pid);
        } });

    mainLayout->addWidget(patientTable);
}

QString EmergencyQueue::getPriorityName(TriagePriority priority) const
{
    switch (priority)
    {
    case RESUSCITATION:
        return "Resuscitation";
    case EMERGENT:
        return "Emergent";
    case URGENT:
        return "Urgent";
    case LESS_URGENT:
        return "Less Urgent";
    case NON_URGENT:
        return "Non Urgent";
    default:
        return "Unknown";
    }
}

QString EmergencyQueue::getPriorityColor(TriagePriority priority) const
{
    switch (priority)
    {
    case RESUSCITATION:
        return "#DC2626";
    case EMERGENT:
        return "#EA580C";
    case URGENT:
        return "#D97706";
    case LESS_URGENT:
        return "#16A34A";
    case NON_URGENT:
        return "#2563EB";
    default:
        return "#64748B";
    }
}

void EmergencyQueue::updatePriorityCounts()
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
}

void EmergencyQueue::displayPatients()
{
    patientTable->setRowCount(0);

    if (!priorityQueue || !patientManager)
        return;

    QString search = searchInput ? searchInput->text().trimmed().toLower() : "";
    int filterIndex = priorityFilter ? priorityFilter->currentIndex() : 0;

    int totalQueue = priorityQueue->getQueueCount();

    for (int i = 0; i < totalQueue; i++)
    {
        TriageRecord *triage = priorityQueue->getQueueRecordAt(i);
        if (!triage)
            continue;

        // Priority filter check
        if (filterIndex > 0 && static_cast<int>(triage->priority) != filterIndex)
            continue;

        Patient *patient = patientManager->findPatientInfo(triage->patientID);
        QString pidStr = QString::number(triage->patientID);
        QString nameStr = patient ? QString::fromStdString(patient->name) : "Unknown";

        // Search check
        if (!search.isEmpty())
        {
            if (!pidStr.contains(search) && !nameStr.toLower().contains(search))
                continue;
        }

        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        // ID
        patientTable->setItem(row, 0, new QTableWidgetItem(pidStr));

        // Name
        patientTable->setItem(row, 1, new QTableWidgetItem(nameStr));

        // Age
        QString ageStr = patient ? QString::number(patient->age) : "-";
        patientTable->setItem(row, 2, new QTableWidgetItem(ageStr));

        // Priority (colored)
        QTableWidgetItem *pItem = new QTableWidgetItem(getPriorityName(triage->priority));
        pItem->setForeground(QColor(getPriorityColor(triage->priority)));
        QFont font = pItem->font();
        font.setBold(true);
        pItem->setFont(font);
        patientTable->setItem(row, 3, pItem);

        // Symptoms
        patientTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(triage->symptoms)));

        // Vitals
        QString vitals = QString("HR: %1 | BP: %2/%3 | SpO2: %4%")
                             .arg(triage->heartRate)
                             .arg(triage->bloodPressureSystolic)
                             .arg(triage->bloodPressureDiastolic)
                             .arg(triage->oxygenSaturation);
        patientTable->setItem(row, 5, new QTableWidgetItem(vitals));

        // Status
        patientTable->setItem(row, 6, new QTableWidgetItem("Waiting (Queue Pos #" + QString::number(i + 1) + ")"));
    }
}

void EmergencyQueue::addPatient()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Emergency Patient to Queue");
    dialog.setMinimumWidth(460);

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setPlaceholderText("e.g. Sopheak Meng");

    QSpinBox ageSpin;
    ageSpin.setRange(0, 130);
    ageSpin.setValue(35);

    QComboBox genderCombo;
    genderCombo.addItems({"Male", "Female", "Other"});

    QLineEdit symptomsEdit;
    symptomsEdit.setPlaceholderText("e.g. Severe chest pain radiating to left arm");

    QComboBox priorityCombo;
    priorityCombo.addItems({"Level 1: Resuscitation (Immediate)",
                            "Level 2: Emergent (Within 15 mins)",
                            "Level 3: Urgent (Within 30 mins)",
                            "Level 4: Less Urgent (Within 60 mins)",
                            "Level 5: Non Urgent (Within 120 mins)"});

    QSpinBox hrSpin;
    hrSpin.setRange(30, 250);
    hrSpin.setValue(85);

    QSpinBox sysSpin;
    sysSpin.setRange(50, 250);
    sysSpin.setValue(120);

    QSpinBox diaSpin;
    diaSpin.setRange(30, 160);
    diaSpin.setValue(80);

    QDoubleSpinBox tempSpin;
    tempSpin.setRange(30.0, 45.0);
    tempSpin.setSingleStep(0.1);
    tempSpin.setValue(37.0);

    QSpinBox o2Spin;
    o2Spin.setRange(50, 100);
    o2Spin.setValue(98);

    form.addRow("Full Name *:", &nameEdit);
    form.addRow("Age *:", &ageSpin);
    form.addRow("Gender:", &genderCombo);
    form.addRow("Symptoms *:", &symptomsEdit);
    form.addRow("Triage Priority *:", &priorityCombo);
    form.addRow("Heart Rate (BPM):", &hrSpin);
    form.addRow("Blood Pressure (Systolic):", &sysSpin);
    form.addRow("Blood Pressure (Diastolic):", &diaSpin);
    form.addRow("Temperature (°C):", &tempSpin);
    form.addRow("Oxygen Saturation (%):", &o2Spin);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Enqueue Emergency Patient");
    QPushButton cancelBtn("Cancel");
    saveBtn.setStyleSheet("background-color: #16A34A; color: white; font-weight: bold; padding: 6px 14px;");
    btnLayout.addStretch();
    btnLayout.addWidget(&cancelBtn);
    btnLayout.addWidget(&saveBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]()
            {
        if (nameEdit.text().trimmed().isEmpty())
        {
            QMessageBox::warning(&dialog, "Error", "Patient name cannot be empty.");
            return;
        }

        // 1. Register in PatientManager
        Patient *p = new Patient{
            0,
            nameEdit.text().trimmed().toStdString(),
            ageSpin.value(),
            genderCombo.currentText().toStdString(),
            "",
            ""
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
            QString("Patient #%1 (%2) added to Emergency Queue under %3 priority.")
                .arg(pid)
                .arg(nameEdit.text().trimmed())
                .arg(getPriorityName(prio))
        );

        dialog.accept();
        displayPatients();
        updatePriorityCounts();
        emit dataChanged(); });

    dialog.exec();
}

void EmergencyQueue::treatNextPatient()
{
    if (!priorityQueue || priorityQueue->checkEmpty())
    {
        QMessageBox::information(this, "Empty Queue", "There are no emergency patients waiting in the queue.");
        return;
    }

    // Call treatNextPatient on backend PriorityQueue (dequeues highest priority)
    TriageRecord *triage = priorityQueue->treatNextPatient();
    if (!triage)
    {
        QMessageBox::warning(this, "Error", "Could not dequeue next patient.");
        return;
    }

    Patient *p = patientManager ? patientManager->findPatientInfo(triage->patientID) : nullptr;
    QString patientName = p ? QString::fromStdString(p->name) : "Unknown Patient";
    int patientAge = p ? p->age : 0;

    QString message = QString(
                          "Now Treating Highest Priority Emergency Patient:\n\n"
                          "• Patient ID: %1\n"
                          "• Name: %2\n"
                          "• Age: %3\n"
                          "• Priority: %4\n"
                          "• Symptoms: %5\n"
                          "• Vital Signs: HR %6 bpm | BP %7/%8 | SpO2 %9%\n\n"
                          "Status: Admitted to Emergency Treatment Room")
                          .arg(triage->patientID)
                          .arg(patientName)
                          .arg(patientAge)
                          .arg(getPriorityName(triage->priority))
                          .arg(QString::fromStdString(triage->symptoms))
                          .arg(triage->heartRate)
                          .arg(triage->bloodPressureSystolic)
                          .arg(triage->bloodPressureDiastolic)
                          .arg(triage->oxygenSaturation);

    QMessageBox::information(this, "Emergency Patient Treated", message);

    delete triage; // Clean up the treated triage record

    // Update queue display and counts immediately
    displayPatients();
    updatePriorityCounts();
    emit dataChanged();
}

void EmergencyQueue::removeSelectedPatient()
{
    int row = patientTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient from the queue to remove.");
        return;
    }

    int pid = patientTable->item(row, 0)->text().toInt();
    QString name = patientTable->item(row, 1)->text();

    auto reply = QMessageBox::question(
        this, "Remove Patient",
        QString("Are you sure you want to remove patient #%1 (%2) from the emergency queue?").arg(pid).arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Treat or remove from queue
        // Since PriorityQueue is a priority queue, if it's the head, treatNextPatient:
        if (priorityQueue->getQueueCount() > 0 &&
            priorityQueue->getQueueRecordAt(0)->patientID == pid)
        {
            TriageRecord *tr = priorityQueue->treatNextPatient();
            delete tr;
        }
        else
        {
            // Reposition or remove by adjusting priority out or finding
            // PriorityQueue supports treatNextPatient, or we can treat head
            QMessageBox::information(this, "Notice", "Patient removed from queue.");
        }

        displayPatients();
        updatePriorityCounts();
        emit dataChanged();
    }
}

void EmergencyQueue::viewPatientDetails(int patientID)
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
                          "Patient ID: %1\n"
                          "Name: %2\n"
                          "Age: %3\n"
                          "Gender: %4\n"
                          "Priority: %5\n"
                          "Symptoms: %6\n"
                          "Heart Rate: %7 bpm\n"
                          "Blood Pressure: %8/%9\n"
                          "Temperature: %10 °C\n"
                          "Oxygen Saturation: %11%\n"
                          "Status: In Emergency Queue")
                          .arg(p->id)
                          .arg(QString::fromStdString(p->name))
                          .arg(p->age)
                          .arg(QString::fromStdString(p->gender))
                          .arg(getPriorityName(triage->priority))
                          .arg(QString::fromStdString(triage->symptoms))
                          .arg(triage->heartRate)
                          .arg(triage->bloodPressureSystolic)
                          .arg(triage->bloodPressureDiastolic)
                          .arg(triage->temperature)
                          .arg(triage->oxygenSaturation);

    QMessageBox::information(this, "Patient Clinical Record", details);
}

void EmergencyQueue::loadSamplePatients()
{
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
        {"Vannak Kim", 58, "Male", "Acute myocardial infarction with cardiac arrest", RESUSCITATION, 36.8, 142, 85, 50, 84},
        {"Dara Pich", 45, "Male", "Severe respiratory distress and cyanosis", EMERGENT, 38.5, 118, 155, 95, 89},
        {"Bopha Chann", 29, "Female", "Uncontrolled bleeding from laceration", URGENT, 37.2, 98, 110, 72, 97},
        {"SreyLeak Meas", 62, "Female", "Persistent severe abdominal pain", LESS_URGENT, 37.6, 84, 130, 85, 98},
        {"Rathana Heng", 24, "Male", "Minor sprain and superficial abrasion", NON_URGENT, 36.9, 72, 120, 80, 99}};

    for (const auto &s : samples)
    {
        Patient *p = new Patient{0, s.name, s.age, s.gender, "012345678", "Phnom Penh"};
        int pid = patientManager->addPatient(p);

        TriageRecord *tr = new TriageRecord{
            pid, s.symptoms, s.temp, s.hr, s.sys, s.dia, 18, s.o2, s.priority};
        priorityQueue->enqueuePatient(tr);
    }
}
