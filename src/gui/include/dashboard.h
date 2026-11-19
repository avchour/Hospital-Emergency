#ifndef dashboard_H
#define dashboard_H

#include <QWidget>
#include <functional>
#include <QVector>

class QLabel;
class QTableWidget;
class PatientManager;
class PriorityQueue;
struct Queue;
class PriorityDonutChart;

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit Dashboard(
        PatientManager *mgr = nullptr,
        PriorityQueue *pq = nullptr,
        Queue *normQueue = nullptr,
        std::function<void(int)> navigate = {},
        QWidget *parent = nullptr
    );

public slots:
    void refreshStats();

private:
    PatientManager *patientManager;
    PriorityQueue *priorityQueue;
    Queue *normalQueue;
    std::function<void(int)> navigateFn;

    QLabel *totalPatientsVal;
    QLabel *emergencyWaitingVal;
    QLabel *normalWaitingVal;
    QLabel *treatedCountVal;

    PriorityDonutChart *donutChart;
    QLabel *priorityCountLabels[5];
    QTableWidget *queueTable;

    void setupUI();
    void updateData();
};

#endif
