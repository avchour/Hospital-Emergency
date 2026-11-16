
#ifndef TRIAGE_GUI_H
#define TRIAGE_GUI_H

#include <QWidget>
#include <QVector>
#include <QString>

class QPushButton;
class QTableWidget;
class QLabel;

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
public:
    explicit Emergency(QWidget *parent = nullptr);

private:
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
