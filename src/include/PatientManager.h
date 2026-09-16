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
    PatientManager(/* args */);
    ~PatientManager();
    bool checkEmpty ();
    void addPatient(Patient *patient);
    int getPatientCount();
    void displayPatient();
    void displayQueue();
    void treatNextPatient();
    Patient *findPatient(int id);
    
};
