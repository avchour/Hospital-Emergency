#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include "../data/Patient.h"

class PatientManager
{
private:
    struct PatientNode
    {
        Patient *patient;
        PatientNode *next;
    };

    PatientNode *head = nullptr;
    PatientNode *tail = nullptr;
    int count = 0;

public:
    PatientManager();
    ~PatientManager();
    bool checkEmpty();
    void addPatient(Patient *patient);
    int getPatientCount();
    void displayPatient();
    Patient *findPatientInfo(int id);
    bool removePatient(int id);
};

#endif
