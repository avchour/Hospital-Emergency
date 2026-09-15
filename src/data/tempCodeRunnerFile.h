#ifndef PRIORITYQ_H
#define PRIORITYQ_H

#include "../data/Patient.h"



class PriorityQ {
    private:
        struct PriorityQNode
{
    Patient* patient;
    PriorityQNode* next = nullptr;
};
PriorityQNode* front = nullptr;
PriorityQNode* tail = nullptr;
int count =0;

public:

    PriorityQ();
    ~PriorityQ();

    bool checkEmergency();
    void addPatient(Patient* patient);
    Patient* removepatient();
    Patient* view;
    int PataientCount();
    void displayQueue();


};

#endif