#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "./PatientManager.h"
#include "../model/triage.h"
#include <array>

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
    bool enqueuePatient(TriageRecord *triage);
    TriageRecord *treatNextPatient();

    bool updatePriority(int patientID, TriagePriority newPriority);

    void displayQueue(const PatientManager &manager) const;
    bool checkEmpty() const;
    int getQueueCount() const;
    TriageRecord *getQueueRecordAt(int index) const;
    std::array<int, 5> getPriorityCounts() const;
};

#endif