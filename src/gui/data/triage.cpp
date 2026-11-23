#include "../include/triage.h"
#include "../../include/PatientManager.h"
#include "../../include/PriorityQueue.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLabel>
#include <QAbstractItemView>
#include <QColor>

#include <algorithm>

// Constructor
Emergency::Emergency(PatientManager *mgr, PriorityQueue *pq, QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      priorityQueue(pq)
{
    selectedPriority = "";

    if (!priorityQueue)
    {
        loadSamplePatients();
    }

    setupUI();
    displayPatients();
    updateCounts();
}

void Emergency::refreshData()
{
    displayPatients();
    updateCounts();
}

// Load sample patients (fallback if no backend passed)
void Emergency::loadSamplePatients()
{
    patients = {
        {1001, "Sok Dara", "Critical", "Severe chest pain", "09:30", "Waiting"},
        {1002, "Sok Tara", "Urgent", "High fever", "09:35", "Waiting"},
        {1003, "Sok Vanna", "Normal", "Headache", "09:40", "Waiting"},
        {1004, "David", "Critical", "Severe bleeding", "09:45", "Waiting"},
        {1005, "Chan", "Urgent", "Difficulty breathing", "09:50", "Waiting"},
        {1006, "Lina", "Normal", "Stomach pain", "09:55", "Waiting"}};

    std::stable_sort(
        patients.begin(),
        patients.end(),
        [this](const EmergencyPatient &a, const EmergencyPatient &b)
        {
            return getPriorityOrder(a.priority) <
                   getPriorityOrder(b.priority);
        });
}

// Define priority order
int Emergency::getPriorityOrder(const QString &priority)
{
    if (priority == "Critical")
        return 1;

    if (priority == "Urgent")
        return 2;

    if (priority == "Normal")
        return 3;

    return 4;
}

// Create GUI
void Emergency::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Page title
    QLabel *title = new QLabel("Emergency Triage Categories");
    title->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 26px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("Monitor emergency triage patients by severity group");
    subtitle->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 15px; color: #64748B;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Priority category layout
    QHBoxLayout *categoryLayout = new QHBoxLayout();

    criticalButton = new QPushButton();
    urgentButton = new QPushButton();
    normalButton = new QPushButton();

    criticalButton->setMinimumHeight(100);
    urgentButton->setMinimumHeight(100);
    normalButton->setMinimumHeight(100);

    criticalButton->setStyleSheet(
        "QPushButton {"
        "background-color: #FDEAEA; color: #D93030; border: 1px solid #F2C5C5;"
        "border-radius: 8px; font-size: 17px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #FADADA; }"
    );

    urgentButton->setStyleSheet(
        "QPushButton {"
        "background-color: #FFF3DF; color: #D98A16; border: 1px solid #F3D8A9;"
        "border-radius: 8px; font-size: 17px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #FFE8C5; }"
    );

    normalButton->setStyleSheet(
        "QPushButton {"
        "background-color: #E7F6ED; color: #269653; border: 1px solid #BFE5CC;"
        "border-radius: 8px; font-size: 17px; font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #D5EFDF; }"
    );

    categoryLayout->addWidget(criticalButton);
    categoryLayout->addWidget(urgentButton);
    categoryLayout->addWidget(normalButton);

    mainLayout->addLayout(categoryLayout);

    // Patient table title
    QHBoxLayout *tableTitleLayout = new QHBoxLayout();
    QLabel *tableTitle = new QLabel("Filtered Triage Patients");
    tableTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #1E293B;");

    totalLabel = new QLabel();
    totalLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #475569;");

    tableTitleLayout->addWidget(tableTitle);
    tableTitleLayout->addStretch();
    tableTitleLayout->addWidget(totalLabel);

    mainLayout->addLayout(tableTitleLayout);

    // Patient table
    patientTable = new QTableWidget();
    patientTable->setColumnCount(6);
    patientTable->setHorizontalHeaderLabels({
        "Patient ID", "Patient Name", "Priority Group", "Symptoms", "Queue State", "Status"
    });
    patientTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    patientTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    patientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    patientTable->verticalHeader()->setVisible(false);
    patientTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white; border: 1px solid #E2E8F0; border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC; color: #475569; font-weight: bold; padding: 8px;"
        "border: none; border-bottom: 1px solid #E2E8F0;"
        "}"
    );

    mainLayout->addWidget(patientTable);

    // Connect filters
    connect(criticalButton, &QPushButton::clicked, this, [this]() {
        selectedPriority = (selectedPriority == "Critical") ? "" : "Critical";
        displayPatients();
    });

    connect(urgentButton, &QPushButton::clicked, this, [this]() {
        selectedPriority = (selectedPriority == "Urgent") ? "" : "Urgent";
        displayPatients();
    });

    connect(normalButton, &QPushButton::clicked, this, [this]() {
        selectedPriority = (selectedPriority == "Normal") ? "" : "Normal";
        displayPatients();
    });
}

// Display and filter patients
void Emergency::displayPatients()
{
    if (priorityQueue && patientManager)
    {
        patients.clear();
        for (int i = 0; i < priorityQueue->getQueueCount(); i++)
        {
            TriageRecord *tr = priorityQueue->getQueueRecordAt(i);
            if (!tr) continue;

            Patient *p = patientManager->findPatientInfo(tr->patientID);
            QString pr;
            if (tr->priority == RESUSCITATION || tr->priority == EMERGENT)
                pr = "Critical";
            else if (tr->priority == URGENT)
                pr = "Urgent";
            else
                pr = "Normal";

            patients.append({
                tr->patientID,
                p ? QString::fromStdString(p->name) : "Unknown",
                pr,
                QString::fromStdString(tr->symptoms),
                "Queue #" + QString::number(i + 1),
                "Waiting"
            });
        }
    }

    patientTable->setRowCount(0);
    int displayedCount = 0;

    for (const EmergencyPatient &patient : patients)
    {
        if (!selectedPriority.isEmpty() && patient.priority != selectedPriority)
            continue;

        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        patientTable->setItem(row, 0, new QTableWidgetItem(QString::number(patient.patientID)));
        patientTable->setItem(row, 1, new QTableWidgetItem(patient.name));

        QTableWidgetItem *priorityItem = new QTableWidgetItem(patient.priority);
        QFont f = priorityItem->font();
        f.setBold(true);
        priorityItem->setFont(f);

        if (patient.priority == "Critical")
            priorityItem->setForeground(QColor("#D93030"));
        else if (patient.priority == "Urgent")
            priorityItem->setForeground(QColor("#D98A16"));
        else
            priorityItem->setForeground(QColor("#269653"));

        patientTable->setItem(row, 2, priorityItem);
        patientTable->setItem(row, 3, new QTableWidgetItem(patient.symptoms));
        patientTable->setItem(row, 4, new QTableWidgetItem(patient.arrivalTime));
        patientTable->setItem(row, 5, new QTableWidgetItem(patient.status));

        displayedCount++;
    }

    if (selectedPriority.isEmpty())
        totalLabel->setText("Total Showing: " + QString::number(displayedCount));
    else
        totalLabel->setText(selectedPriority + " Group: " + QString::number(displayedCount));
}

void Emergency::updateCounts()
{
    int critical = 0;
    int urgent = 0;
    int normal = 0;

    for (const EmergencyPatient &patient : patients)
    {
        if (patient.priority == "Critical")
            critical++;
        else if (patient.priority == "Urgent")
            urgent++;
        else if (patient.priority == "Normal")
            normal++;
    }

    criticalButton->setText("CRITICAL / EMERGENT\n" + QString::number(critical) + " Patients");
    urgentButton->setText("URGENT\n" + QString::number(urgent) + " Patients");
    normalButton->setText("NORMAL / LESS URGENT\n" + QString::number(normal) + " Patients");
}
