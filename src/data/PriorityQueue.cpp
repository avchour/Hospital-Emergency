#include "../include/PriorityQueue.h"
#include "../model/Patient.h"
#include <iostream>

PriorityQueue::PriorityQueue() = default;

PriorityQueue::~PriorityQueue()
{
    QueueNode *current = head;

    while (current != nullptr)
    {
        QueueNode *temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
};

bool PriorityQueue::checkEmpty()const 
{
    return head == nullptr;
};

int PriorityQueue::getQueueCount() const
{
    return count;
}

void PriorityQueue::enqueuePatient(TriageRecord *triage) 
{ 
    if (triage == nullptr) 
        return; 
 
    QueueNode *queueNode = new QueueNode{triage, nullptr}; 
    QueueNode *cur = head; 
    QueueNode *prev = nullptr; 
 
    if (checkEmpty()) 
    { 
        head = queueNode; 
        tail = queueNode; 
    } 
    else 
    {
        while (cur != nullptr && cur->triage->priority <= triage->priority)
        {
            prev = cur;
            cur = cur->next;
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

TriageRecord *PriorityQueue::treatNextPatient()
{
    if (checkEmpty())
    {
        return nullptr;
    }

    QueueNode *temp = head;
    TriageRecord *triage = temp->triage;

    head = head->next;

    if (head == nullptr)
    {
        tail = nullptr;
    }

    delete temp;
    count--;

    return triage;
}

void PriorityQueue::displayQueue(const PatientManager &manager) const
{
    if (checkEmpty())
    {
        std::cout << "Emergency queue is empty.\n";
        return;
    }

    QueueNode *cur = head;

    while (cur != nullptr)
    {
        Patient *patient = manager.findPatientInfo(cur->triage->patientID);

        if (patient != nullptr)
        {
            // Patient information
            std::cout << "\n===== PATIENT INFORMATION =====\n";
            std::cout << "ID: " << patient->id << '\n';
            std::cout << "Name: " << patient->name << '\n';
            std::cout << "Age: " << patient->age << '\n';
            std::cout << "Gender: " << patient->gender << '\n';
            std::cout << "Phone: " << patient->phone << '\n';

            // Triage information
            std::cout << "\n===== TRIAGE ASSESSMENT =====\n";
            std::cout << "Priority: " << cur->triage->priority << '\n';
            std::cout << "Symptoms: " << cur->triage->symptoms << '\n';
            std::cout << "Temperature: " << cur->triage->temperature << '\n';
            std::cout << "Heart Rate: " << cur->triage->heartRate << '\n';

            std::cout << "Blood Pressure: "
                      << cur->triage->bloodPressureSystolic << "/"
                      << cur->triage->bloodPressureDiastolic << '\n';

            std::cout << "Oxygen Saturation: "
                      << cur->triage->oxygenSaturation << "%\n";
        }
        else
        {
            std::cout << "Patient record not found. ID: "
                      << cur->triage->patientID << '\n';
        }

        std::cout << "================================\n";

        cur = cur->next;
    }
}