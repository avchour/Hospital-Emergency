#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "./PatientManager.h"
#include "../model/triage.h"

class PriorityQueue
{
private:
    struct QueueNode
    {
        TriageRecord *triage;
        QueueNode *next;
    };
    QueueNode *head = nullptr;
    QueueNode *tail = nullptr;
    int count = 0;

public:
    PriorityQueue();
    ~PriorityQueue();
    void enqueuePatient(TriageRecord *triage);
    TriageRecord *treatNextPatient();

    bool updatePriority(int patientID, TriagePriority newPriority);

    void displayQueue(const PatientManager &manager) const;
    bool checkEmpty() const;
    int getQueueCount() const;
};

#endif