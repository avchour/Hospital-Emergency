#ifndef PATIENT_PAGE_H
#define PATIENT_PAGE_H

#include <QWidget>
#include <QString>

class QTableWidget;
class QLineEdit;
class QPushButton;
class QLabel;
class QCheckBox;
class QResizeEvent;
class PatientManager;

class PatientPage : public QWidget
{
    Q_OBJECT

public:
    explicit PatientPage(PatientManager *manager, QWidget *parent = nullptr);

signals:
    void dataChanged();

public slots:
    void refreshTable();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    PatientManager *patientManager;

    QLineEdit *searchInput;
    QTableWidget *patientTable;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QLabel *totalLabel;

    int selectedPatientId;
    bool isUpdatingSelection;

    void setupUI();
    void addPatient();
    void editPatient();
    void removeSelectedPatient();
    void searchPatient();
    void onPatientCheckboxToggled(int patientId, bool checked, QCheckBox *senderCheckBox);
    void updateActionButtons();
    void highlightRow(int row, bool selected);
    void adjustTableColumns();
};

#endif
