
#include "../include/triage.h"

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
Emergency::Emergency(QWidget *parent)
    : QWidget(parent)
{
    selectedPriority = "";

    loadSamplePatients();
    setupUI();
    displayPatients();
    updateCounts();
}

// Load sample patients
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

    // Page title
    QLabel *title = new QLabel("Emergency Triage");

    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #192F50;");

    QLabel *subtitle = new QLabel(
        "Manage and monitor emergency triage patients");

    subtitle->setStyleSheet(
        "font-size: 13px;"
        "color: #777777;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Priority category layout
    QHBoxLayout *categoryLayout = new QHBoxLayout();

    criticalButton = new QPushButton();
    urgentButton = new QPushButton();
    normalButton = new QPushButton();

    criticalButton->setMinimumHeight(110);
    urgentButton->setMinimumHeight(110);
    normalButton->setMinimumHeight(110);

    // Critical button style
    criticalButton->setStyleSheet(
        "QPushButton {"
        "background-color: #FDEAEA;"
        "color: #D93030;"
        "border: 1px solid #F2C5C5;"
        "border-radius: 10px;"
        "font-size: 18px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #FADADA;"
        "}");

    // Urgent button style
    urgentButton->setStyleSheet(
        "QPushButton {"
        "background-color: #FFF3DF;"
        "color: #D98A16;"
        "border: 1px solid #F3D8A9;"
        "border-radius: 10px;"
        "font-size: 18px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #FFE8C5;"
        "}");

    // Normal button style
    normalButton->setStyleSheet(
        "QPushButton {"
        "background-color: #E7F6ED;"
        "color: #269653;"
        "border: 1px solid #BFE5CC;"
        "border-radius: 10px;"
        "font-size: 18px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #D5EFDF;"
        "}");

    categoryLayout->addWidget(criticalButton);
    categoryLayout->addWidget(urgentButton);
    categoryLayout->addWidget(normalButton);

    mainLayout->addLayout(categoryLayout);

    // Patient table title
    QHBoxLayout *tableTitleLayout = new QHBoxLayout();

    QLabel *tableTitle = new QLabel("All Emergency Patients");

    tableTitle->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #192F50;");

    totalLabel = new QLabel();

    tableTitleLayout->addWidget(tableTitle);
    tableTitleLayout->addStretch();
    tableTitleLayout->addWidget(totalLabel);

    mainLayout->addLayout(tableTitleLayout);

    // Patient table
    patientTable = new QTableWidget();

    patientTable->setColumnCount(6);

    patientTable->setHorizontalHeaderLabels({"Patient ID",
                                             "Patient Name",
                                             "Priority",
                                             "Symptoms",
                                             "Arrival Time",
                                             "Status"});

    patientTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    patientTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    patientTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    patientTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    patientTable->verticalHeader()->setVisible(false);

    mainLayout->addWidget(patientTable);

    // Critical button click
    connect(criticalButton, &QPushButton::clicked, this, [this]()
            {
        selectedPriority =
            (selectedPriority == "Critical") ? "" : "Critical";

        displayPatients(); });

    // Urgent button click
    connect(urgentButton, &QPushButton::clicked, this, [this]()
            {
        selectedPriority =
            (selectedPriority == "Urgent") ? "" : "Urgent";

        displayPatients(); });

    // Normal button click
    connect(normalButton, &QPushButton::clicked, this, [this]()
            {
        selectedPriority =
            (selectedPriority == "Normal") ? "" : "Normal";

        displayPatients(); });
}

// Display and filter patients
void Emergency::displayPatients()
{
    patientTable->setRowCount(0);

    int displayedCount = 0;

    for (const EmergencyPatient &patient : patients)
    {
        // Filter by selected priority
        if (!selectedPriority.isEmpty() &&
            patient.priority != selectedPriority)
        {
            continue;
        }

        int row = patientTable->rowCount();

        patientTable->insertRow(row);

        // Patient ID
        patientTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(patient.patientID)));

        // Patient name
        patientTable->setItem(
            row, 1,
            new QTableWidgetItem(patient.name));

        // Patient priority
        QTableWidgetItem *priorityItem =
            new QTableWidgetItem(patient.priority);

        if (patient.priority == "Critical")
        {
            priorityItem->setForeground(QColor("#D93030"));
        }
        else if (patient.priority == "Urgent")
        {
            priorityItem->setForeground(QColor("#D98A16"));
        }
        else
        {
            priorityItem->setForeground(QColor("#269653"));
        }

        patientTable->setItem(row, 2, priorityItem);

        // Symptoms
        patientTable->setItem(
            row, 3,
            new QTableWidgetItem(patient.symptoms));

        // Arrival time
        patientTable->setItem(
            row, 4,
            new QTableWidgetItem(patient.arrivalTime));

        // Treatment status
        patientTable->setItem(
            row, 5,
            new QTableWidgetItem(patient.status));

        displayedCount++;
    }

    // Update patient count label
    if (selectedPriority.isEmpty())
    {
        totalLabel->setText(
            "Total Patients: " +
            QString::number(displayedCount));
    }
    else
    {
        totalLabel->setText(
            selectedPriority + " Patients: " +
            QString::number(displayedCount));
    }
}

// Count patients by priority
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

    criticalButton->setText(
        "CRITICAL\n" +
        QString::number(critical) +
        " Patients");

    urgentButton->setText(
        "URGENT\n" +
        QString::number(urgent) +
        " Patients");

    normalButton->setText(
        "NORMAL\n" +
        QString::number(normal) +
        " Patients");
}
