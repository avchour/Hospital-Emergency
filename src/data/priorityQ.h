#ifndef PRIORITYQ_H
#define PRIORITYQ_H
 
#include "../data/Patient.h"
 
class PriorityQ {
    private:
        struct PriorityQNode {
            Patient* patient;
            PriorityQNode* next = nullptr;
        };
 
        PriorityQNode* head = nullptr;
        PriorityQNode* tail = nullptr;
        int count = 0;
        Patient* view = nullptr;
 
    public:
        PriorityQ();
        ~PriorityQ();
 
        // prevent copy — avoid double-free on linked list
        PriorityQ(const PriorityQ&) = delete;
        PriorityQ& operator=(const PriorityQ&) = delete;
 
        bool checkEmergency();
        void addPatient(Patient* patient);
        Patient* removePatient();
        Patient* getView() const;
        int patientCount() const;
        void displayQueue();
};
 
#endif