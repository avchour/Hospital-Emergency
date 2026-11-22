#ifndef mainwindow_H
#define mainwindow_H

#include <QMainWindow>
#include <QList>

class QStackedWidget;
class QPushButton;
class PatientManager;
class PriorityQueue;
class PatientPage;
class EmergencyAssessment;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 🏗️ CONSTRUCTOR
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    // 🧭 NAVIGATION
    QStackedWidget *pages;

    // ✏️ MANUAL CHANGE:
    // Add or remove navigation buttons here if you change the menu.
    QList<QPushButton *> navButtons;

    PatientManager *patientManager;
    PriorityQueue *priorityQueue;
    PatientPage *patientPage;
    EmergencyAssessment *assessmentPage;

    // 🎨 GUI SETUP
    void setupUI();

    // 🔄 PAGE NAVIGATION
    void changePage(int index);

    // ⭐ SIDEBAR DESIGN
    void setActiveButton(int activeIndex);
};

#endif
