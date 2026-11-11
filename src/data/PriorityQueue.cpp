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
 
    if (checkEmpty()) 
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
                break; 
            } 
            else if (cur->patient->priority < patient->priority) 
            { 
                prev = cur; 
                cur = cur->next; 
            } 
            else 
            { 
                prev = cur; 
                cur = cur->next; 
            } 
        } 
 
        if (prev == nullptr) 
        { 
            queueNode->next = head; 
            head = queueNode; 
        } 
        else 
        { 
            prev->next = queueNode; 
            queueNode->next = cur; 
 
            if (cur == nullptr) 
            { 
                tail = queueNode; 
            } 
        } 
    } 
 
    count++; 
}

Patient* PriorityQueue::treatNextPatient()
{
    if (checkEmpty())
    {
        return nullptr;
    }

    QueueNode *temp = head;
    Patient *patient = head->patient;

    head = head->next;

    if (head == nullptr)
    {
        tail = nullptr;
    }

    delete temp;
    count--;

    return patient;
}


void PriorityQueue::displayQueue()
{
    if (checkEmpty())
    {
        std::cout << "Emergency queue is empty.\n";
        return;
    }

    QueueNode *cur = head;

    while (cur != nullptr)
    {
        std::cout << "ID: " << cur->patient->id << std::endl;
        std::cout << "Name: " << cur->patient->name << std::endl;
        std::cout << "Priority: " << cur->patient->priority << std::endl;
        std::cout << "Symptoms: " << cur->patient->symptoms << std::endl;

        cur = cur->next;
    }
}