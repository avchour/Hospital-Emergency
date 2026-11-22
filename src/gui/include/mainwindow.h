#ifndef mainwindow_H
#define mainwindow_H

#include <QMainWindow>
#include <QList>

#include "../../include/PatientManager.h"
#include "../../include/PriorityQueue.h"
#include "../../include/Queue.h"
#include "../../include/RoomManager.h"

class QStackedWidget;
class QPushButton;
class Dashboard;
class PatientPage;
class Emergency;
class EmergencyQueue;
class TreatmentPage;
class RoomPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // Shared backend instances
    PatientManager patientManager;
    PriorityQueue priorityQueue;
    Queue normalQueue;
    RoomManager roomManager;

    // GUI Pages
    Dashboard *dashboardPage;
    PatientPage *patientPage;
    Emergency *triagePage;
    EmergencyQueue *emergencyQueuePage;
    TreatmentPage *treatmentPage;
    RoomPage *roomPage;

    // Navigation
    QStackedWidget *pages;
    QList<QPushButton *> navButtons;

    void setupUI();
    void initSampleData();
    void changePage(int index);
    void setActiveButton(int activeIndex);
};

#endif
