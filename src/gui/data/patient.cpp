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
#include <QCheckBox>

PatientPage::PatientPage(PatientManager *manager, QWidget *parent)
    : QWidget(parent),
      patientManager(manager),
      searchInput(nullptr),
      patientTable(nullptr),
      addButton(nullptr),
      editButton(nullptr),
      removeButton(nullptr),
      totalLabel(nullptr),
      selectedPatientId(-1),
      isUpdatingSelection(false)
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
    title->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 26px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("View, register, and update patient records");
    subtitle->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 15px; color: #64748B;");

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
        "font-family: 'Gotham', 'Montserrat', sans-serif;"
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

    // Table with Select checkbox column
    patientTable = new QTableWidget();
    patientTable->setColumnCount(7);
    patientTable->setHorizontalHeaderLabels({
        "Select", "Patient ID", "Full Name", "Age", "Gender", "Phone", "Address"
    });
    patientTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    patientTable->setColumnWidth(0, 65);
    for (int col = 1; col < 7; ++col)
    {
        patientTable->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
    }

    patientTable->setSelectionMode(QAbstractItemView::NoSelection);
    patientTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    patientTable->verticalHeader()->setVisible(false);
    patientTable->setStyleSheet(
        "QTableWidget {"
        "font-family: 'Gotham', 'Montserrat', sans-serif;"
        "background-color: white;"
        "border: 1px solid #E2E8F0;"
        "border-radius: 8px;"
        "gridline-color: #F1F5F9;"
        "}"
        "QHeaderView::section {"
        "font-family: 'Gotham', 'Montserrat', sans-serif;"
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
    totalLabel->setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; font-size: 14px; font-weight: bold; color: #475569;");

    editButton = new QPushButton("Edit Patient Details");
    editButton->setMinimumHeight(38);
    editButton->setEnabled(false);
    editButton->setStyleSheet(
        "QPushButton {"
        "font-family: 'Gotham', 'Montserrat', sans-serif;"
        "background-color: #2563EB;"
        "color: white;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #1D4ED8; }"
        "QPushButton:disabled { background-color: #94A3B8; color: #E2E8F0; }"
    );
    connect(editButton, &QPushButton::clicked, this, &PatientPage::editPatient);

    removeButton = new QPushButton("Remove Selected Patient");
    removeButton->setMinimumHeight(38);
    removeButton->setEnabled(false);
    removeButton->setStyleSheet(
        "QPushButton {"
        "font-family: 'Gotham', 'Montserrat', sans-serif;"
        "background-color: #DC2626;"
        "color: white;"
        "border-radius: 6px;"
        "padding: 8px 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #B91C1C; }"
        "QPushButton:disabled { background-color: #94A3B8; color: #E2E8F0; }"
    );
    connect(removeButton, &QPushButton::clicked, this, &PatientPage::removeSelectedPatient);

    bottomLayout->addWidget(totalLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(editButton);
    bottomLayout->addWidget(removeButton);

    mainLayout->addLayout(bottomLayout);
}

void PatientPage::highlightRow(int row, bool selected)
{
    QColor bg = selected ? QColor("#EFF6FF") : QColor(row % 2 == 0 ? "#FFFFFF" : "#F8FAFC");
    for (int c = 1; c < patientTable->columnCount(); ++c)
    {
        QTableWidgetItem *it = patientTable->item(row, c);
        if (it)
        {
            it->setBackground(bg);
        }
    }
}

void PatientPage::updateActionButtons()
{
    bool hasSelection = (selectedPatientId >= 0);
    if (editButton)
        editButton->setEnabled(hasSelection);
    if (removeButton)
        removeButton->setEnabled(hasSelection);
}

void PatientPage::onPatientCheckboxToggled(int patientId, bool checked, QCheckBox *senderCheckBox)
{
    isUpdatingSelection = true;

    if (checked)
    {
        selectedPatientId = patientId;

        // Uncheck all other checkboxes in the table
        for (int r = 0; r < patientTable->rowCount(); ++r)
        {
            QWidget *cellWidget = patientTable->cellWidget(r, 0);
            if (cellWidget)
            {
                QCheckBox *cb = cellWidget->findChild<QCheckBox *>();
                if (cb && cb != senderCheckBox)
                {
                    if (cb->isChecked())
                    {
                        cb->setChecked(false);
                    }
                    highlightRow(r, false);
                }
                else if (cb == senderCheckBox)
                {
                    highlightRow(r, true);
                }
            }
        }
    }
    else
    {
        if (selectedPatientId == patientId)
        {
            selectedPatientId = -1;
        }

        for (int r = 0; r < patientTable->rowCount(); ++r)
        {
            QWidget *cellWidget = patientTable->cellWidget(r, 0);
            if (cellWidget)
            {
                QCheckBox *cb = cellWidget->findChild<QCheckBox *>();
                if (cb == senderCheckBox)
                {
                    highlightRow(r, false);
                }
            }
        }
    }

    updateActionButtons();
    isUpdatingSelection = false;
}

void PatientPage::refreshTable()
{
    patientTable->setRowCount(0);

    if (!patientManager)
    {
        selectedPatientId = -1;
        updateActionButtons();
        return;
    }

    int count = patientManager->getPatientCount();
    totalLabel->setText("Total Registered: " + QString::number(count));

    bool selectedStillExists = false;
    isUpdatingSelection = true;

    for (int i = 0; i < count; i++)
    {
        Patient *p = patientManager->getPatientAt(i);
        if (!p) continue;

        int row = patientTable->rowCount();
        patientTable->insertRow(row);

        // Column 0: Checkbox
        QWidget *cellWidget = new QWidget();
        QHBoxLayout *cellLayout = new QHBoxLayout(cellWidget);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        cellLayout->setAlignment(Qt::AlignCenter);

        QCheckBox *cb = new QCheckBox(cellWidget);
        cb->setCursor(Qt::PointingHandCursor);
        bool isChecked = (p->id == selectedPatientId);
        if (isChecked)
        {
            cb->setChecked(true);
            selectedStillExists = true;
        }

        connect(cb, &QCheckBox::toggled, this, [this, pId = p->id, cb](bool checked) {
            if (isUpdatingSelection) return;
            onPatientCheckboxToggled(pId, checked, cb);
        });

        cellLayout->addWidget(cb);
        patientTable->setCellWidget(row, 0, cellWidget);

        // Columns 1-6: Patient data items (non-selectable to prevent row-click selection)
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(p->id));
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::fromStdString(p->name));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(p->age));
        QTableWidgetItem *item3 = new QTableWidgetItem(QString::fromStdString(p->gender));
        QTableWidgetItem *item4 = new QTableWidgetItem(QString::fromStdString(p->phone));
        QTableWidgetItem *item5 = new QTableWidgetItem(QString::fromStdString(p->address));

        item0->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);

        QList<QTableWidgetItem *> items = {item0, item1, item2, item3, item4, item5};
        for (int col = 0; col < items.size(); ++col)
        {
            items[col]->setFlags(Qt::ItemIsEnabled);
            patientTable->setItem(row, col + 1, items[col]);
        }

        highlightRow(row, isChecked);
    }

    isUpdatingSelection = false;

    if (!selectedStillExists)
    {
        selectedPatientId = -1;
    }
    updateActionButtons();
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
    bool foundSelectedInSearch = false;

    isUpdatingSelection = true;
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

            // Column 0: Checkbox
            QWidget *cellWidget = new QWidget();
            QHBoxLayout *cellLayout = new QHBoxLayout(cellWidget);
            cellLayout->setContentsMargins(0, 0, 0, 0);
            cellLayout->setAlignment(Qt::AlignCenter);

            QCheckBox *cb = new QCheckBox(cellWidget);
            cb->setCursor(Qt::PointingHandCursor);
            bool isChecked = (p->id == selectedPatientId);
            if (isChecked)
            {
                cb->setChecked(true);
                foundSelectedInSearch = true;
            }

            connect(cb, &QCheckBox::toggled, this, [this, pId = p->id, cb](bool checked) {
                if (isUpdatingSelection) return;
                onPatientCheckboxToggled(pId, checked, cb);
            });

            cellLayout->addWidget(cb);
            patientTable->setCellWidget(row, 0, cellWidget);

            // Columns 1-6: Patient data items
            QTableWidgetItem *item0 = new QTableWidgetItem(idStr);
            QTableWidgetItem *item1 = new QTableWidgetItem(nameStr);
            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(p->age));
            QTableWidgetItem *item3 = new QTableWidgetItem(QString::fromStdString(p->gender));
            QTableWidgetItem *item4 = new QTableWidgetItem(QString::fromStdString(p->phone));
            QTableWidgetItem *item5 = new QTableWidgetItem(QString::fromStdString(p->address));

            item0->setTextAlignment(Qt::AlignCenter);
            item2->setTextAlignment(Qt::AlignCenter);
            item3->setTextAlignment(Qt::AlignCenter);

            QList<QTableWidgetItem *> items = {item0, item1, item2, item3, item4, item5};
            for (int col = 0; col < items.size(); ++col)
            {
                items[col]->setFlags(Qt::ItemIsEnabled);
                patientTable->setItem(row, col + 1, items[col]);
            }

            highlightRow(row, isChecked);
            matchCount++;
        }
    }
    isUpdatingSelection = false;

    if (!foundSelectedInSearch)
    {
        selectedPatientId = -1;
    }
    updateActionButtons();

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
    saveBtn.setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; background-color: #16A34A; color: white; font-weight: bold; padding: 6px 14px;");
    cancelBtn.setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; padding: 6px 14px;");
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
    if (selectedPatientId < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient using the checkbox to edit.");
        return;
    }

    int patientId = selectedPatientId;
    Patient *p = patientManager ? patientManager->findPatientInfo(patientId) : nullptr;
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
    saveBtn.setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; background-color: #2563EB; color: white; font-weight: bold; padding: 6px 14px;");
    cancelBtn.setStyleSheet("font-family: 'Gotham', 'Montserrat', sans-serif; padding: 6px 14px;");
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
    if (selectedPatientId < 0)
    {
        QMessageBox::warning(this, "Select Patient", "Please select a patient using the checkbox to remove.");
        return;
    }

    int patientId = selectedPatientId;
    Patient *p = patientManager ? patientManager->findPatientInfo(patientId) : nullptr;
    QString name = p ? QString::fromStdString(p->name) : ("#" + QString::number(patientId));

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
            selectedPatientId = -1;
            refreshTable();
            emit dataChanged();
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to remove patient.");
        }
    }
}
