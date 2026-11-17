#ifndef TRIAGE_GUI_H
#define TRIAGE_GUI_H

#include <QWidget>
#include <QVector>
#include <QString>

class QPushButton;
class QTableWidget;
class QLabel;
class PatientManager;
class PriorityQueue;

struct EmergencyPatient
{
    int patientID;
    QString name;
    QString priority;
    QString symptoms;
    QString arrivalTime;
    QString status;
};

class Emergency : public QWidget
{
    Q_OBJECT

public:
    explicit Emergency(
        PatientManager *mgr = nullptr,
        PriorityQueue *pq = nullptr,
        QWidget *parent = nullptr);

public slots:
    void refreshData();

private:
    PatientManager *patientManager;
    PriorityQueue *priorityQueue;

    QVector<EmergencyPatient> patients;

    QPushButton *criticalButton;
    QPushButton *urgentButton;
    QPushButton *normalButton;

    QTableWidget *patientTable;
    QLabel *totalLabel;

    QString selectedPriority;

    void setupUI();
    void loadSamplePatients();
    void displayPatients();
    void updateCounts();

    int getPriorityOrder(const QString &priority);
};

#endif
