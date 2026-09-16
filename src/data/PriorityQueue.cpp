#include "../include/PriorityQueue.h"

#include <iostream>

PriorityQueue::PriorityQueue() {};

PriorityQueue::~PriorityQueue()
{
    QueueNode *current = head;

    while (current != nullptr)
    {
        QueueNode *temp = current;
        current = current->next;
        delete temp;
    }
};

bool PriorityQueue::checkEmpty() //"Is the emergency treatment queue currently empty?"
{
    return head == nullptr;
};

void PriorityQueue::enqueuePatient(Patient *patient)
{
    if (patient == nullptr)
        return;
    QueueNode *queueNode = new QueueNode{patient, nullptr};
    QueueNode *cur = head;
    QueueNode *prev = nullptr;
    if (checkEmpty()) // correct case1
    {
        head = queueNode;
        tail = queueNode;
    }
    else
    {
        while (cur != nullptr)
        {

            if (cur->patient->priority > patient->priority)
            {

            }
            else if (cur->patient->priority < patient->priority)
            {
            }
            else
            {
            }
        }

        count++;
    };
};