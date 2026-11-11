#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include "../model/Patient.h"

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
    int nextPatientID = 1;
    void clearList();
    PatientNode *findNode(int id) const;

public:
    PatientManager();
    ~PatientManager();
    bool checkEmpty() const;
    int addPatient(Patient *patient);
    int getPatientCount() const;
    void displayPatient() const; // temporary
    Patient *findPatientInfo(int id);
    bool removePatient(int id);
    bool updatePatient(int id, const std::string &targetMember, const std::string &newValue);
};

#endif
