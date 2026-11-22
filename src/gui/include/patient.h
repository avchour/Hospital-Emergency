#ifndef PATIENT_PAGE_H
#define PATIENT_PAGE_H

#include <QWidget>
#include <QString>

class QTableWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class PatientManager;

class PatientPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientPage(PatientManager *manager, QWidget *parent = nullptr);
    void setPatientManager(PatientManager *manager);
    PatientManager *getPatientManager() const { return patientManager; }

signals:
    void dataChanged();

public slots:
    void refreshTable();

private:
    PatientManager *patientManager;

    QLineEdit *searchInput;
    QTableWidget *patientTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QLabel *totalLabel;

    void setupUI();
    void addPatient();
    void editPatient();
    void removeSelectedPatient();
    void searchPatient();
};

#endif
