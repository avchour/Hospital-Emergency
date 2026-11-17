#include "../include/patient.h"
#include "../../include/PatientManager.h"
#include "../../include/utils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QMessageBox>

PatientPage::PatientPage(PatientManager *manager, QWidget *parent)
    : QWidget(parent),
      patientManager(manager),
      searchInput(nullptr),
      patientTable(nullptr),
      addButton(nullptr),
      editButton(nullptr),
      removeButton(nullptr),
      totalLabel(nullptr)
{
    setupUI();
    refreshTable();
}

void PatientPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title & Subtitle
    QLabel *title = new QLabel("Patient Management");
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("View, register, and update patient records");
    subtitle->setStyleSheet("font-size: 15px; color: #64748B;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Controls: Search & Add
    QHBoxLayout *controls = new QHBoxLayout();

    searchInput = new QLineEdit();
    searchInput->setPlaceholderText("Search by Patient ID or Name...");
    searchInput->setMinimumHeight(40);
    connect(searchInput, &QLineEdit::textChanged, this, &PatientPage::searchPatient);

    addButton = new QPushButton("+ Register New Patient");
    addButton->setMinimumHeight(40);
    addButton->setStyleSheet(
        "QPushButton {"
        "background-color: #16A34A;"
        "color: white;"
        "font-weight: bold;"
        "border: none;"
        "border-radius: 6px;"
        "padding: 0 16px;"
        "}"
        "QPushButton:hover { background-color: #15803D; }"
    );
    connect(addButton, &QPushButton::clicked, this, &PatientPage::addPatient);

    controls->addWidget(searchInput);
    controls->addWidget(addButton);
    mainLayout->addLayout(controls);

    // Table
    patientTable = new QTableWidget();
    patientTable->setColumnCount(6);
    patientTable->setHorizontalHeaderLabels({
        "Patient ID", "Full Name", "Age", "Gender", "Phone", "Address"
    });
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
        "}"
    );
    mainLayout->addWidget(patientTable);

    // Bottom action bar
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    totalLabel = new QLabel("Total Registered: 0");
    totalLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #475569;");

    editButton = new QPushButton("Edit Patient Details");
    editButton->setMinimumHeight(38);
    editButton->setStyleSheet(
        "QPushButton {"
        "background-color: #2563EB;"
        "color: white;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #1D4ED8; }"
    );
    connect(editButton, &QPushButton::clicked, this, &PatientPage::editPatient);

    removeButton = new QPushButton("Remove Selected Patient");
    removeButton->setMinimumHeight(38);
    removeButton->setStyleSheet(
        "QPushButton {"
        "background-color: #DC2626;"
        "color: white;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #B91C1C; }"
    );
    connect(removeButton, &QPushButton::clicked, this, &PatientPage::removeSelectedPatient);

    bottomLayout->addWidget(totalLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(editButton);
    bottomLayout->addWidget(removeButton);

    mainLayout->addLayout(bottomLayout);
}

void PatientPage::refreshTable()
{
    patientTable->setRowCount(0);

    if (!patientManager)
        return;

    int count = patientManager->getPatientCount();
    totalLabel->setText("Total Registered: " + QString::number(count));

    for (int i = 0; i < count; i++)
    {
        Patient *p = patientManager->getPatientAt(i);
        if (!p) continue;

        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        patientTable->setItem(row, 0, new QTableWidgetItem(QString::number(p->id)));
        patientTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p->name)));
        patientTable->setItem(row, 2, new QTableWidgetItem(QString::number(p->age)));
        patientTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(p->gender)));
        patientTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(p->phone)));
        patientTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(p->address)));
    }
}

void PatientPage::searchPatient()
{
    QString query = searchInput->text().trimmed();
    if (query.isEmpty())
    {
        refreshTable();
        return;
    }

    patientTable->setRowCount(0);
    if (!patientManager)
        return;

    int total = patientManager->getPatientCount();
    int matchCount = 0;

    for (int i = 0; i < total; i++)
    {
        Patient *p = patientManager->getPatientAt(i);
        if (!p) continue;

        QString idStr = QString::number(p->id);
        QString nameStr = QString::fromStdString(p->name);

        if (idStr.contains(query, Qt::CaseInsensitive) ||
            nameStr.contains(query, Qt::CaseInsensitive))
        {
            int row = patientTable->rowCount();
            patientTable->insertRow(row);

            patientTable->setItem(row, 0, new QTableWidgetItem(idStr));
            patientTable->setItem(row, 1, new QTableWidgetItem(nameStr));
            patientTable->setItem(row, 2, new QTableWidgetItem(QString::number(p->age)));
            patientTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(p->gender)));
            patientTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(p->phone)));
            patientTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(p->address)));
            matchCount++;
        }
    }

    totalLabel->setText("Matches found: " + QString::number(matchCount));
}

void PatientPage::addPatient()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Register New Patient");
    dialog.setMinimumWidth(420);

    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setPlaceholderText("e.g. John Doe");

    QSpinBox ageSpin;
    ageSpin.setRange(0, 130);
    ageSpin.setValue(30);

    QComboBox genderCombo;
    genderCombo.addItems({"Male", "Female", "Other"});

    QLineEdit phoneEdit;
    phoneEdit.setPlaceholderText("e.g. 012345678");

    QLineEdit addressEdit;
    addressEdit.setPlaceholderText("e.g. Phnom Penh");

    form.addRow("Full Name *:", &nameEdit);
    form.addRow("Age *:", &ageSpin);
    form.addRow("Gender:", &genderCombo);
    form.addRow("Phone Number:", &phoneEdit);
    form.addRow("Home Address:", &addressEdit);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Register Patient");
    QPushButton cancelBtn("Cancel");
    saveBtn.setStyleSheet("background-color: #16A34A; color: white; font-weight: bold; padding: 6px 14px;");
    btnLayout.addStretch();
    btnLayout.addWidget(&cancelBtn);
    btnLayout.addWidget(&saveBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().trimmed().isEmpty())
        {
            QMessageBox::warning(&dialog, "Input Error", "Patient name cannot be empty.");
            return;
        }

        Patient *newPatient = new Patient{
            0,
            nameEdit.text().trimmed().toStdString(),
            ageSpin.value(),
            genderCombo.currentText().toStdString(),
            phoneEdit.text().trimmed().toStdString(),
            addressEdit.text().trimmed().toStdString()
        };

        int newId = patientManager->addPatient(newPatient);
        QMessageBox::information(this, "Success", "Registered patient successfully with ID: " + QString::number(newId));
        dialog.accept();
        refreshTable();
        emit dataChanged();
    });

    dialog.exec();
}

void PatientPage::editPatient()
{
    int row = patientTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient from the table to edit.");
        return;
    }

    int patientId = patientTable->item(row, 0)->text().toInt();
    Patient *p = patientManager->findPatientInfo(patientId);
    if (!p)
    {
        QMessageBox::critical(this, "Error", "Could not find patient with ID: " + QString::number(patientId));
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Patient Details - ID: " + QString::number(patientId));
    dialog.setMinimumWidth(420);

    QFormLayout form(&dialog);

    QLineEdit nameEdit(QString::fromStdString(p->name));
    QSpinBox ageSpin;
    ageSpin.setRange(0, 130);
    ageSpin.setValue(p->age);

    QComboBox genderCombo;
    genderCombo.addItems({"Male", "Female", "Other"});
    genderCombo.setCurrentText(QString::fromStdString(p->gender));

    QLineEdit phoneEdit(QString::fromStdString(p->phone));
    QLineEdit addressEdit(QString::fromStdString(p->address));

    form.addRow("Full Name *:", &nameEdit);
    form.addRow("Age *:", &ageSpin);
    form.addRow("Gender:", &genderCombo);
    form.addRow("Phone Number:", &phoneEdit);
    form.addRow("Home Address:", &addressEdit);

    QHBoxLayout btnLayout;
    QPushButton saveBtn("Save Changes");
    QPushButton cancelBtn("Cancel");
    saveBtn.setStyleSheet("background-color: #2563EB; color: white; font-weight: bold; padding: 6px 14px;");
    btnLayout.addStretch();
    btnLayout.addWidget(&cancelBtn);
    btnLayout.addWidget(&saveBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&saveBtn, &QPushButton::clicked, [&]() {
        if (nameEdit.text().trimmed().isEmpty())
        {
            QMessageBox::warning(&dialog, "Input Error", "Patient name cannot be empty.");
            return;
        }

        patientManager->updatePatient(patientId, NAME, nameEdit.text().trimmed().toStdString());
        patientManager->updatePatient(patientId, AGE, std::to_string(ageSpin.value()));
        patientManager->updatePatient(patientId, GENDER, genderCombo.currentText().toStdString());
        patientManager->updatePatient(patientId, PHONE, phoneEdit.text().trimmed().toStdString());
        patientManager->updatePatient(patientId, ADDRESS, addressEdit.text().trimmed().toStdString());

        QMessageBox::information(this, "Success", "Patient information updated.");
        dialog.accept();
        refreshTable();
        emit dataChanged();
    });

    dialog.exec();
}

void PatientPage::removeSelectedPatient()
{
    int row = patientTable->currentRow();
    if (row < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient to remove.");
        return;
    }

    int patientId = patientTable->item(row, 0)->text().toInt();
    QString name = patientTable->item(row, 1)->text();

    auto reply = QMessageBox::question(
        this,
        "Confirm Deletion",
        "Are you sure you want to remove patient #" + QString::number(patientId) + " (" + name + ")?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes)
    {
        if (patientManager->removePatient(patientId))
        {
            QMessageBox::information(this, "Removed", "Patient record successfully removed.");
            refreshTable();
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to remove patient.");
        }
    }
}
