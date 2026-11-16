#include "../include/emergency.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QMessageBox>
#include <QDateTime>
#include <QAbstractItemView>

// ==========================================
// CONSTRUCTOR
// ==========================================

EmergencyQueue::EmergencyQueue(QWidget *parent)
    : QWidget(parent),
      mainLayout(nullptr),
      criticalCount(nullptr),
      mediumCount(nullptr),
      normalCount(nullptr),
      patientTable(nullptr),
      searchInput(nullptr),
      priorityFilter(nullptr),
      addButton(nullptr),
      removeButton(nullptr)
{
    setupUI();
}

// ==========================================
// MAIN UI
// ==========================================

void EmergencyQueue::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Page title
    QLabel *title = new QLabel("Emergency Queue");

    title->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #1E293B;");

    mainLayout->addWidget(title);

    // Priority cards
    setupPriorityCards();

    // Patient management title
    QLabel *managementTitle =
        new QLabel("Patient Management");

    managementTitle->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;"
        "color: #334155;");

    mainLayout->addWidget(managementTitle);

    // Search and Add section
    QHBoxLayout *controlLayout = new QHBoxLayout();

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText(
        "Search by ID or name...");
    searchInput->setMinimumHeight(38);

    addButton = new QPushButton("+ Add Patient");
    addButton->setMinimumHeight(38);

    addButton->setStyleSheet(
        "QPushButton {"
        "background-color: #16A34A;"
        "color: white;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 8px 18px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #15803D;"
        "}");

    controlLayout->addWidget(searchInput);
    controlLayout->addWidget(addButton);

    mainLayout->addLayout(controlLayout);

    // Priority filter
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *filterLabel = new QLabel("Filter Priority:");

    priorityFilter = new QComboBox();

    priorityFilter->addItems({"All Patients",
                              "Critical",
                              "Medium",
                              "Normal"});

    priorityFilter->setMinimumHeight(35);

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(priorityFilter);
    filterLayout->addStretch();

    mainLayout->addLayout(filterLayout);

    // Patient table title
    QLabel *tableTitle = new QLabel("All Patients");

    tableTitle->setStyleSheet(
        "font-size: 18px;"
        "font-weight: bold;");

    mainLayout->addWidget(tableTitle);

    // Patient table
    setupPatientTable();

    // Remove button
    removeButton =
        new QPushButton("Remove Selected Patient");

    removeButton->setMinimumHeight(36);

    removeButton->setStyleSheet(
        "QPushButton {"
        "background-color: #DC2626;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 8px 14px;"
        "}"
        "QPushButton:hover {"
        "background-color: #B91C1C;"
        "}");

    mainLayout->addWidget(removeButton);

    // Signal and slot connections
    connect(
        addButton,
        &QPushButton::clicked,
        this,
        &EmergencyQueue::addPatient);

    connect(
        removeButton,
        &QPushButton::clicked,
        this,
        &EmergencyQueue::removeSelectedPatient);

    connect(
        searchInput,
        &QLineEdit::textChanged,
        this,
        &EmergencyQueue::searchPatient);

    connect(
        priorityFilter,
        &QComboBox::currentTextChanged,
        this,
        &EmergencyQueue::applyFilters);
}

// ==========================================
// PRIORITY CARDS
// ==========================================

void EmergencyQueue::setupPriorityCards()
{
    QHBoxLayout *cardLayout = new QHBoxLayout();
    cardLayout->setSpacing(15);

    auto createCard = [](
                          const QString &title,
                          const QString &color,
                          QLabel *&countLabel) -> QFrame *
    {
        QFrame *card = new QFrame();

        card->setStyleSheet(
            QString(
                "QFrame {"
                "background-color: white;"
                "border: 1px solid %1;"
                "border-radius: 10px;"
                "}")
                .arg(color));

        QVBoxLayout *layout = new QVBoxLayout(card);

        QLabel *name = new QLabel(title);

        name->setStyleSheet(
            QString(
                "color: %1;"
                "font-size: 15px;"
                "font-weight: bold;")
                .arg(color));

        countLabel = new QLabel("0");

        countLabel->setStyleSheet(
            QString(
                "color: %1;"
                "font-size: 28px;"
                "font-weight: bold;")
                .arg(color));

        QLabel *description = new QLabel("Patients");

        description->setStyleSheet(
            "color: #64748B;");

        layout->addWidget(name);
        layout->addWidget(countLabel);
        layout->addWidget(description);

        card->setMinimumHeight(115);

        return card;
    };

    QFrame *criticalCard =
        createCard("CRITICAL", "#DC2626", criticalCount);

    QFrame *mediumCard =
        createCard("MEDIUM", "#D97706", mediumCount);

    QFrame *normalCard =
        createCard("NORMAL", "#16A34A", normalCount);

    cardLayout->addWidget(criticalCard);
    cardLayout->addWidget(mediumCard);
    cardLayout->addWidget(normalCard);

    mainLayout->addLayout(cardLayout);
}

// ==========================================
// PATIENT TABLE
// ==========================================

void EmergencyQueue::setupPatientTable()
{
    patientTable = new QTableWidget(this);

    patientTable->setColumnCount(5);

    patientTable->setHorizontalHeaderLabels({"ID",
                                             "Name",
                                             "Age",
                                             "Stay",
                                             "Details"});

    patientTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    patientTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    patientTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    patientTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    patientTable->verticalHeader()->setVisible(false);

    patientTable->setAlternatingRowColors(true);

    patientTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white;"
        "border: 1px solid #E2E8F0;"
        "gridline-color: #E2E8F0;"
        "}"
        "QHeaderView::section {"
        "background-color: #F1F5F9;"
        "padding: 10px;"
        "font-weight: bold;"
        "border: none;"
        "}");

    mainLayout->addWidget(patientTable);
}

// ==========================================
// CHECK DUPLICATE PATIENT ID
// ==========================================

bool EmergencyQueue::patientIDExists(
    const QString &patientID) const
{
    for (int row = 0;
         row < patientTable->rowCount();
         row++)
    {
        QTableWidgetItem *item =
            patientTable->item(row, 0);

        if (item && item->text() == patientID)
        {
            return true;
        }
    }

    return false;
}

// ==========================================
// ADD PATIENT
// ==========================================

void EmergencyQueue::addPatient()
{
    QDialog dialog(this);

    dialog.setWindowTitle("Add New Patient");
    dialog.setMinimumWidth(350);

    QFormLayout form(&dialog);

    QLineEdit *idInput = new QLineEdit();
    QLineEdit *nameInput = new QLineEdit();

    QSpinBox *ageInput = new QSpinBox();
    ageInput->setRange(1, 120);

    QLineEdit *stayInput = new QLineEdit();

    stayInput->setPlaceholderText("Example: 2 days");

    QComboBox *priorityInput = new QComboBox();

    priorityInput->addItems({"Critical",
                             "Medium",
                             "Normal"});

    QLineEdit *symptomsInput = new QLineEdit();

    QComboBox *statusInput = new QComboBox();

    statusInput->addItems({"Waiting",
                           "In Treatment",
                           "Admitted",
                           "Discharged"});

    form.addRow("Patient ID:", idInput);
    form.addRow("Full Name:", nameInput);
    form.addRow("Age:", ageInput);
    form.addRow("Stay:", stayInput);
    form.addRow("Priority:", priorityInput);
    form.addRow("Symptoms:", symptomsInput);
    form.addRow("Status:", statusInput);

    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    form.addRow(buttonLayout);

    connect(
        saveButton,
        &QPushButton::clicked,
        &dialog,
        &QDialog::accept);

    connect(
        cancelButton,
        &QPushButton::clicked,
        &dialog,
        &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
        return;

    QString id = idInput->text().trimmed();
    QString name = nameInput->text().trimmed();
    QString stay = stayInput->text().trimmed();

    // Validate required fields
    if (id.isEmpty() ||
        name.isEmpty() ||
        stay.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Invalid Input",
            "Please fill in Patient ID, Name and Stay.");

        return;
    }

    // Check duplicate ID
    if (patientIDExists(id))
    {
        QMessageBox::warning(
            this,
            "Duplicate ID",
            "This Patient ID already exists.");

        return;
    }

    // Add new row
    int row = patientTable->rowCount();

    patientTable->insertRow(row);

    QTableWidgetItem *idItem =
        new QTableWidgetItem(id);

    // Store additional patient information
    idItem->setData(
        Qt::UserRole,
        priorityInput->currentText());

    idItem->setData(
        Qt::UserRole + 1,
        symptomsInput->text());

    idItem->setData(
        Qt::UserRole + 2,
        statusInput->currentText());

    idItem->setData(
        Qt::UserRole + 3,
        QDateTime::currentDateTime().toString(
            "yyyy-MM-dd hh:mm AP"));

    patientTable->setItem(row, 0, idItem);

    patientTable->setItem(
        row,
        1,
        new QTableWidgetItem(name));

    patientTable->setItem(
        row,
        2,
        new QTableWidgetItem(
            QString::number(ageInput->value())));

    patientTable->setItem(
        row,
        3,
        new QTableWidgetItem(stay));

    // Details button
    QPushButton *detailsButton =
        new QPushButton("View");

    detailsButton->setStyleSheet(
        "QPushButton {"
        "background-color: #2563EB;"
        "color: white;"
        "border: none;"
        "border-radius: 4px;"
        "padding: 5px 12px;"
        "}"
        "QPushButton:hover {"
        "background-color: #1D4ED8;"
        "}");

    patientTable->setCellWidget(
        row,
        4,
        detailsButton);

    connect(
        detailsButton,
        &QPushButton::clicked,
        this,
        [this, id]()
        {
            viewPatientDetails(id);
        });

    updatePriorityCounts();
    applyFilters();
}

// ==========================================
// SEARCH PATIENT
// ==========================================

void EmergencyQueue::searchPatient()
{
    applyFilters();
}

// ==========================================
// FILTER PATIENTS
// ==========================================

void EmergencyQueue::applyFilters()
{
    QString searchText =
        searchInput->text().trimmed();

    QString selectedPriority =
        priorityFilter->currentText();

    for (int row = 0;
         row < patientTable->rowCount();
         row++)
    {
        QTableWidgetItem *idItem =
            patientTable->item(row, 0);

        QTableWidgetItem *nameItem =
            patientTable->item(row, 1);

        if (!idItem || !nameItem)
            continue;

        QString id = idItem->text();
        QString name = nameItem->text();

        QString priority =
            idItem->data(Qt::UserRole).toString();

        bool matchesSearch =
            id.contains(
                searchText,
                Qt::CaseInsensitive) ||
            name.contains(
                searchText,
                Qt::CaseInsensitive);

        bool matchesPriority =
            selectedPriority == "All Patients" ||
            selectedPriority == priority;

        patientTable->setRowHidden(
            row,
            !(matchesSearch && matchesPriority));
    }
}

// ==========================================
// UPDATE PRIORITY COUNTS
// ==========================================

void EmergencyQueue::updatePriorityCounts()
{
    int critical = 0;
    int medium = 0;
    int normal = 0;

    for (int row = 0;
         row < patientTable->rowCount();
         row++)
    {
        QTableWidgetItem *item =
            patientTable->item(row, 0);

        if (!item)
            continue;

        QString priority =
            item->data(Qt::UserRole).toString();

        if (priority == "Critical")
            critical++;

        else if (priority == "Medium")
            medium++;

        else if (priority == "Normal")
            normal++;
    }

    criticalCount->setText(
        QString::number(critical));

    mediumCount->setText(
        QString::number(medium));

    normalCount->setText(
        QString::number(normal));
}

// ==========================================
// REMOVE PATIENT
// ==========================================

void EmergencyQueue::removeSelectedPatient()
{
    int row = patientTable->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a patient first.");

        return;
    }

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Remove Patient",
            "Are you sure you want to remove this patient?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        patientTable->removeRow(row);

        updatePriorityCounts();
    }
}

// ==========================================
// VIEW PATIENT DETAILS
// ==========================================

void EmergencyQueue::viewPatientDetails(
    const QString &patientID)
{
    int row = -1;

    for (int i = 0;
         i < patientTable->rowCount();
         i++)
    {
        QTableWidgetItem *item =
            patientTable->item(i, 0);

        if (item && item->text() == patientID)
        {
            row = i;
            break;
        }
    }

    if (row < 0)
        return;

    QTableWidgetItem *idItem =
        patientTable->item(row, 0);

    QString name =
        patientTable->item(row, 1)->text();

    QString age =
        patientTable->item(row, 2)->text();

    QString stay =
        patientTable->item(row, 3)->text();

    QString priority =
        idItem->data(Qt::UserRole).toString();

    QString symptoms =
        idItem->data(Qt::UserRole + 1).toString();

    QString status =
        idItem->data(Qt::UserRole + 2).toString();

    QString arrival =
        idItem->data(Qt::UserRole + 3).toString();

    QString details =
        "Patient ID: " + patientID +
        "\nName: " + name +
        "\nAge: " + age +
        "\nStay: " + stay +
        "\nPriority: " + priority +
        "\nSymptoms: " + symptoms +
        "\nStatus: " + status +
        "\nArrival Time: " + arrival;

    QMessageBox::information(
        this,
        "Patient Details",
        details);
}