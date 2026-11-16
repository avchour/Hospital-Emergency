#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <QWidget>
#include <QString>

class QTableWidget;
class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QVBoxLayout;

class EmergencyQueue : public QWidget
{
    Q_OBJECT

public:
    explicit EmergencyQueue(QWidget *parent = nullptr);

private:
    QLabel *criticalCount;
    QLabel *mediumCount;
    QLabel *normalCount;

    QVBoxLayout *mainLayout;

    QTableWidget *patientTable;
    QLineEdit *searchInput;
    QComboBox *priorityFilter;

    QPushButton *addButton;
    QPushButton *removeButton;

    void setupUI();
    void setupPriorityCards();
    void setupPatientTable();

    void addPatient();
    void removeSelectedPatient();
    void searchPatient();
    void applyFilters();
    void updatePriorityCounts();
    void viewPatientDetails(const QString &patientID);

    bool patientIDExists(const QString &patientID) const;
};

#endif
