#ifndef TREATMENT_PAGE_H
#define TREATMENT_PAGE_H

#include <QWidget>
#include <QString>
#include <QVector>

class QTableWidget;
class QPushButton;
class QLabel;
class QLineEdit;
class PatientManager;
struct Queue;

struct TreatmentLog
{
    QString timestamp;
    int patientId;
    QString patientName;
    QString serviceType;
    QString outcome;
};

class TreatmentPage : public QWidget
{
    Q_OBJECT

public:
    explicit TreatmentPage(
        PatientManager *mgr,
        Queue *normQueue,
        QWidget *parent = nullptr
    );

    void addTreatmentLog(int patientId, const QString &name, const QString &type, const QString &outcome);

signals:
    void dataChanged();

public slots:
    void refreshQueue();

private:
    PatientManager *patientManager;
    Queue *normalQueue;

    QLabel *waitingCountLabel;
    QTableWidget *queueTable;
    QTableWidget *historyTable;
    QPushButton *callNextButton;
    QPushButton *addNormalButton;

    QVector<TreatmentLog> treatmentHistory;

    void setupUI();
    void addNormalPatient();
    void callNextPatient();
    void refreshHistoryTable();
};

#endif
