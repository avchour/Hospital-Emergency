#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "../data/Patient.h"

class PriorityQueue
{
private:
    struct QueueNode
    {
        Patient *patient;
        QueueNode *next;
    };
    QueueNode *head = nullptr;
    QueueNode *tail = nullptr;
    int count = 0;

public:
    PriorityQueue();
    ~PriorityQueue();
    void enqueuePatient(Patient *patient);
    Patient *treatNextPatient();
    void displayQueue();
    bool checkEmpty();
};

#endif