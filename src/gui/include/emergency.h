#ifndef EMERGENCY_H
#define EMERGENCY_H
#include <QWidget>
#include <QString>
#include <QVector>

#include "../../model/Patient.h"
#include "../../model/triage.h"
class QVBoxLayout;
class QLabel;
class QTableWidget;
class QLineEdit;
class QComboBox;
class QPushButton;
class PatientManager;
class PriorityQueue;

class EmergencyQueue : public QWidget
{
    Q_OBJECT

public:
    struct EmergencyPatient
    {
        Patient patient;
        TriageRecord triage;

        QString stay;
        QString status;
        QString arrivalTime;
    };

    explicit EmergencyQueue(
        PatientManager *mgr = nullptr,
        PriorityQueue *pq = nullptr,
        QWidget *parent = nullptr);
    ~EmergencyQueue() override;

    void setBackend(PatientManager *mgr, PriorityQueue *pq);

signals:
    void dataChanged();

public slots:
    void refreshQueue();

private:
    PatientManager *patientManager;
    PriorityQueue *priorityQueue;
    bool ownsBackend;

    // Main layout
    QVBoxLayout *mainLayout;

    // Priority counters
    QLabel *resuscitationCount;
    QLabel *emergentCount;
    QLabel *urgentCount;
    QLabel *lessUrgentCount;
    QLabel *nonUrgentCount;

    // Patient controls
    QTableWidget *patientTable;
    QLineEdit *searchInput;
    QComboBox *priorityFilter;

    QPushButton *addButton;
    QPushButton *treatButton;
    QPushButton *removeButton;

    // UI setup
    void setupUI();
    void setupPriorityCards();
    void setupPatientTable();

    // Data & Actions
    void loadSamplePatients();
    void addPatient();
    void treatNextPatient();
    void removeSelectedPatient();
    void searchPatient();
    void applyFilters();
    void displayPatients();
    void updatePriorityCounts();
    void viewPatientDetails(int patientID);

    // Priority helpers
    QString getPriorityName(TriagePriority priority) const;
    QString getPriorityColor(TriagePriority priority) const;
};

#endif
