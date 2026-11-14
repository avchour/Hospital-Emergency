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

bool PriorityQueue::enqueuePatient(TriageRecord *triage) 
{ 
    if (triage == nullptr) 
        return false; 
 
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
    return true;
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

bool PriorityQueue::updatePriority(int patientID,TriagePriority newPriority)
{
    // Validate priority
    if (newPriority < RESUSCITATION ||
        newPriority > NON_URGENT)
        return false;

    QueueNode *cur = head;
    QueueNode *prev = nullptr;

    // Find patient
    while (cur != nullptr &&
           cur->triage->patientID != patientID)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr)
        return false;

    // No repositioning needed
    if (cur->triage->priority == newPriority)
        return true;

    // Remove node from current position
    if (prev == nullptr)
        head = cur->next;
    else
        prev->next = cur->next;

    if (cur == tail)
        tail = prev;

    // Update priority
    cur->triage->priority = newPriority;
    cur->next = nullptr;

    // Find new position
    prev = nullptr;
    QueueNode *position = head;

    while (position != nullptr &&
           position->triage->priority <= newPriority)
    {
        prev = position;
        position = position->next;
    }

    // Insert node into new position
    if (prev == nullptr)
    {
        cur->next = head;
        head = cur;

        if (tail == nullptr)
            tail = cur;
    }
    else
    {
        prev->next = cur;
        cur->next = position;

        if (position == nullptr)
            tail = cur;
    }

    return true;
}
 TriageRecord *PriorityQueue::getQueueRecordAt(int index) const

{

    if (index < 0 || index >= count)

        return nullptr;

    QueueNode *cur = head;

    for (int i = 0; i < index; i++)

    {

        cur = cur->next;

    }

    return cur->triage;

}