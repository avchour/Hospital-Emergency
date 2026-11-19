#include "../include/treatment.h"
#include "../../include/PatientManager.h"
#include "../../include/Queue.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QDateTime>

TreatmentPage::TreatmentPage(PatientManager *mgr, Queue *normQueue, QWidget *parent)
    : QWidget(parent),
      patientManager(mgr),
      normalQueue(normQueue),
      waitingCountLabel(nullptr),
      queueTable(nullptr),
      historyTable(nullptr),
      callNextButton(nullptr),
      addNormalButton(nullptr)
{
    setupUI();
    refreshQueue();
}

void TreatmentPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *title = new QLabel("Normal Queue & Treatment Management");
    title->setStyleSheet("font-size: 26px; font-weight: bold; color: #172554;");

    QLabel *subtitle = new QLabel("Manage outpatient FIFO line, call waiting patients, and track clinical care");
    subtitle->setStyleSheet("font-size: 15px; color: #64748B;");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Section 1: Normal FIFO Queue
    QHBoxLayout *queueHeader = new QHBoxLayout();
    QLabel *qTitle = new QLabel("Outpatient Normal Queue (FIFO)");
    qTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #1E293B;");

    waitingCountLabel = new QLabel("Waiting: 0");
    waitingCountLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #2563EB;");

    addNormalButton = new QPushButton("+ Add to Normal Queue");
    addNormalButton->setStyleSheet(
        "QPushButton {"
        "background-color: #16A34A; color: white; font-weight: bold;"
        "border-radius: 6px; padding: 8px 14px;"
        "}"
        "QPushButton:hover { background-color: #15803D; }"
    );
    connect(addNormalButton, &QPushButton::clicked, this, &TreatmentPage::addNormalPatient);

    callNextButton = new QPushButton("Call Next Patient");
    callNextButton->setStyleSheet(
        "QPushButton {"
        "background-color: #2563EB; color: white; font-weight: bold;"
        "border-radius: 6px; padding: 8px 16px;"
        "}"
        "QPushButton:hover { background-color: #1D4ED8; }"
    );
    connect(callNextButton, &QPushButton::clicked, this, &TreatmentPage::callNextPatient);

    queueHeader->addWidget(qTitle);
    queueHeader->addWidget(waitingCountLabel);
    queueHeader->addStretch();
    queueHeader->addWidget(addNormalButton);
    queueHeader->addWidget(callNextButton);
    mainLayout->addLayout(queueHeader);

    // Queue Table
    queueTable = new QTableWidget();
    queueTable->setColumnCount(6);
    queueTable->setHorizontalHeaderLabels({
        "Queue Order", "Patient ID", "Full Name", "Age", "Gender", "Status"
    });
    queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    queueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    queueTable->setSelectionMode(QAbstractItemView::SingleSelection);
    queueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    queueTable->verticalHeader()->setVisible(false);
    queueTable->setMaximumHeight(260);
    queueTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white; border: 1px solid #E2E8F0; border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC; color: #475569; font-weight: bold; padding: 6px;"
        "border: none; border-bottom: 1px solid #E2E8F0;"
        "}"
    );
    mainLayout->addWidget(queueTable);

    // Section 2: Treatment History
    QLabel *histTitle = new QLabel("Clinical Treatment History & Discharges");
    histTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #1E293B; margin-top: 10px;");
    mainLayout->addWidget(histTitle);

    historyTable = new QTableWidget();
    historyTable->setColumnCount(5);
    historyTable->setHorizontalHeaderLabels({
        "Time", "Patient ID", "Patient Name", "Department / Type", "Clinical Outcome"
    });
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    historyTable->setSelectionMode(QAbstractItemView::SingleSelection);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->verticalHeader()->setVisible(false);
    historyTable->setStyleSheet(
        "QTableWidget {"
        "background-color: white; border: 1px solid #E2E8F0; border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "background-color: #F8FAFC; color: #475569; font-weight: bold; padding: 6px;"
        "border: none; border-bottom: 1px solid #E2E8F0;"
        "}"
    );
    mainLayout->addWidget(historyTable);
}

void TreatmentPage::refreshQueue()
{
    queueTable->setRowCount(0);

    if (!normalQueue || !patientManager)
        return;

    int pos = 1;
    for (Node *cur = normalQueue->head; cur != nullptr; cur = cur->next)
    {
        int row = queueTable->rowCount();
        queueTable->insertRow(row);

        Patient *p = patientManager->findPatientInfo(cur->patientId);

        queueTable->setItem(row, 0, new QTableWidgetItem("#" + QString::number(pos++)));
        queueTable->setItem(row, 1, new QTableWidgetItem(QString::number(cur->patientId)));
        queueTable->setItem(row, 2, new QTableWidgetItem(p ? QString::fromStdString(p->name) : "Unknown"));
        queueTable->setItem(row, 3, new QTableWidgetItem(p ? QString::number(p->age) : "-"));
        queueTable->setItem(row, 4, new QTableWidgetItem(p ? QString::fromStdString(p->gender) : "-"));
        queueTable->setItem(row, 5, new QTableWidgetItem("Waiting for Doctor Consultation"));
    }

    waitingCountLabel->setText("Waiting: " + QString::number(pos - 1));
}

void TreatmentPage::refreshHistoryTable()
{
    historyTable->setRowCount(0);

    for (int i = treatmentHistory.size() - 1; i >= 0; i--)
    {
        const auto &log = treatmentHistory[i];
        int row = historyTable->rowCount();
        historyTable->insertRow(row);

        historyTable->setItem(row, 0, new QTableWidgetItem(log.timestamp));
        historyTable->setItem(row, 1, new QTableWidgetItem(QString::number(log.patientId)));
        historyTable->setItem(row, 2, new QTableWidgetItem(log.patientName));
        historyTable->setItem(row, 3, new QTableWidgetItem(log.serviceType));
        historyTable->setItem(row, 4, new QTableWidgetItem(log.outcome));
    }
}

void TreatmentPage::addTreatmentLog(int patientId, const QString &name, const QString &type, const QString &outcome)
{
    TreatmentLog log;
    log.timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    log.patientId = patientId;
    log.patientName = name;
    log.serviceType = type;
    log.outcome = outcome;

    treatmentHistory.append(log);
    refreshHistoryTable();
}

void TreatmentPage::addNormalPatient()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Enqueue Patient to Normal Queue");
    dialog.setMinimumWidth(400);

    QFormLayout form(&dialog);

    QComboBox existingCombo;
    existingCombo.addItem("-- Register New Patient Below --", -1);

    if (patientManager)
    {
        for (int i = 0; i < patientManager->getPatientCount(); i++)
        {
            Patient *p = patientManager->getPatientAt(i);
            if (p)
            {
                existingCombo.addItem(
                    QString("#%1 - %2 (Age %3)").arg(p->id).arg(QString::fromStdString(p->name)).arg(p->age),
                    p->id
                );
            }
        }
    }

    QLineEdit nameEdit;
    QSpinBox ageSpin;
    ageSpin.setRange(0, 130);
    ageSpin.setValue(30);

    QComboBox genderCombo;
    genderCombo.addItems({"Male", "Female", "Other"});

    form.addRow("Select Registered Patient:", &existingCombo);
    form.addRow("--- Or New Patient ---", new QLabel());
    form.addRow("Full Name:", &nameEdit);
    form.addRow("Age:", &ageSpin);
    form.addRow("Gender:", &genderCombo);

    QHBoxLayout btnLayout;
    QPushButton enqueueBtn("Enqueue Patient");
    QPushButton cancelBtn("Cancel");
    enqueueBtn.setStyleSheet("background-color: #16A34A; color: white; font-weight: bold; padding: 6px 14px;");
    btnLayout.addStretch();
    btnLayout.addWidget(&cancelBtn);
    btnLayout.addWidget(&enqueueBtn);
    form.addRow(&btnLayout);

    connect(&cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(&enqueueBtn, &QPushButton::clicked, [&]() {
        int selectedId = existingCombo.currentData().toInt();

        if (selectedId == -1)
        {
            if (nameEdit.text().trimmed().isEmpty())
            {
                QMessageBox::warning(&dialog, "Error", "Please select an existing patient or enter a name.");
                return;
            }

            Patient *p = new Patient{
                0,
                nameEdit.text().trimmed().toStdString(),
                ageSpin.value(),
                genderCombo.currentText().toStdString(),
                "",
                ""
            };
            selectedId = patientManager->addPatient(p);
        }

        enqueue(*normalQueue, selectedId);

        Patient *p = patientManager->findPatientInfo(selectedId);
        QString name = p ? QString::fromStdString(p->name) : "Patient";

        QMessageBox::information(
            this, "Patient Enqueued",
            QString("Patient #%1 (%2) added to Normal Queue.").arg(selectedId).arg(name)
        );

        dialog.accept();
        refreshQueue();
        emit dataChanged();
    });

    dialog.exec();
}

void TreatmentPage::callNextPatient()
{
    if (!normalQueue || isEmpty(*normalQueue))
    {
        QMessageBox::information(this, "Empty Queue", "No patients are waiting in the normal queue.");
        return;
    }

    int nextPatientId = peek(*normalQueue);
    dequeue(*normalQueue);

    Patient *p = patientManager ? patientManager->findPatientInfo(nextPatientId) : nullptr;
    QString name = p ? QString::fromStdString(p->name) : "Patient";

    QMessageBox::information(
        this, "Patient Called",
        QString("Now Calling Patient #%1 (%2) to Consultation Room 1.")
            .arg(nextPatientId).arg(name)
    );

    addTreatmentLog(nextPatientId, name, "General Outpatient Consultation", "Seen by Doctor / Prescription Issued");

    refreshQueue();
    emit dataChanged();
}
