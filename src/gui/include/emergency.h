#ifndef EMERGENCY_ASSESSMENT_H
#define EMERGENCY_ASSESSMENT_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <array>

#include "../../model/Patient.h"
#include "../../model/triage.h"

class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QTableWidget;
class QLineEdit;
class QPushButton;
class QFrame;
class PatientManager;
class PriorityQueue;

class EmergencyAssessment : public QWidget
{
    Q_OBJECT

public:
    explicit EmergencyAssessment(
        PatientManager *mgr = nullptr,
        PriorityQueue *pq = nullptr,
        QWidget *parent = nullptr);
    ~EmergencyAssessment() override;

    void setBackend(PatientManager *mgr, PriorityQueue *pq);

signals:
    void dataChanged();

public slots:
    void refreshAssessment();
    void displayPatients();
    void updatePriorityCounts();

private:
    PatientManager *patientManager;
    PriorityQueue *priorityQueue;
    bool ownsBackend;

    int selectedPriorityFilter; // 0 = All, 1..5 = TriagePriority levels

    // Main layout
    QVBoxLayout *mainLayout;

    // Header: 5-level priority cards & count labels
    QLabel *resuscitationCount;
    QLabel *emergentCount;
    QLabel *urgentCount;
    QLabel *lessUrgentCount;
    QLabel *nonUrgentCount;

    // Below Header: Queue controls
    QLineEdit *searchInput;
    QPushButton *addButton;
    QPushButton *treatButton;

    // Filter Category: 5 Levels from triage
    QPushButton *filterAllBtn;
    QPushButton *filterResuscitationBtn;
    QPushButton *filterEmergentBtn;
    QPushButton *filterUrgentBtn;
    QPushButton *filterLessUrgentBtn;
    QPushButton *filterNonUrgentBtn;
    QLabel *filterStatusLabel;

    // Queue Table & Bottom bar
    QTableWidget *patientTable;
    QLabel *queueTotalLabel;
    QPushButton *removeButton;

    // UI setup
    void setupUI();
    void setupTriageHeaderCards();
    void setupQueueActionControls();
    void setupCategoryFilterButtons();
    void setupQueueTable();
    void setupBottomActionBar();

    // Data & Actions
    void loadSampleDataIfEmpty();
    void addPatient();
    void treatNextPatient();
    void removeSelectedPatient();
    void setPriorityFilter(int priorityLevel);
    void updateFilterButtonStyles();
    void viewPatientDetails(int patientID);

    // Helpers
    QString getPriorityName(TriagePriority priority) const;
    QString getPriorityColor(TriagePriority priority) const;
    QString getPriorityBgColor(TriagePriority priority) const;
};

// Alias for backwards compatibility
using EmergencyQueue = EmergencyAssessment;

#endif
