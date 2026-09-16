#include "../include/PatientManager.h"

#include <iostream>

PatientManager::PatientManager()
{
    head = nullptr;
}

PatientManager::~PatientManager()
{
    PatientNode *current = head;

    while (current != nullptr)
    {
        PatientNode *temp = current;

        current = current->next;

        delete temp->patient;
        delete temp;
    }

    head = nullptr;
}

bool PatientManager::checkEmpty (){
    return head== nullptr;
}

void PatientManager::addPatient(Patient *patient)
{
    PatientNode *patientNode = new PatientNode;
    patientNode->patient = patient;
    patientNode->next = nullptr;

    if (checkEmpty())
    {
        head = patientNode;
        tail = patientNode;
        count++;
        return;
    }

    // Insert based on priority (1 = high, 2 = medium, 3 = low)
    if (patient->priority < head->patient->priority)
    {
        patientNode->next = head;
        head = patientNode;
    }
    else
    {
        PatientNode *current = head;
        while (current->next != nullptr && current->next->patient->priority <= patient->priority)
        {
            current = current->next;
        }
        patientNode->next = current->next;
        current->next = patientNode;
        if (patientNode->next == nullptr)
        {
            tail = patientNode;
        }
    }
    count++;
}

void PatientManager::displayPatient()
{
    PatientNode *curPatient = head;
    while (curPatient != nullptr)
    {
        std::cout << curPatient->patient->name << std::endl;
        curPatient = curPatient->next;
    }
}

void PatientManager::displayQueue()
{
    if (checkEmpty())
    {
        std::cout << "Priority Queue is empty." << std::endl;
        return;
    }

    std::cout << "--- Hospital Emergency Priority Queue (Total Patients: " << count << ") ---" << std::endl;
    PatientNode *curPatient = head;
    while (curPatient != nullptr)
    {
        std::cout << "[ID: " << curPatient->patient->id 
                  << "] " << curPatient->patient->name 
                  << " | Age: " << curPatient->patient->age 
                  << " | Gender: " << curPatient->patient->gender 
                  << " | Priority: " << curPatient->patient->priority 
                  << " | Symptoms: " << curPatient->patient->symptoms 
                  << " | Status: " << curPatient->patient->status 
                  << std::endl;
        curPatient = curPatient->next;
    }
}

void PatientManager::treatNextPatient()
{
    if (checkEmpty())
    {
        std::cout << "Queue is empty. Cannot dequeue / treat next patient." << std::endl;
        return;
    }

    PatientNode *temp = head;
    Patient *patientToTreat = temp->patient;

    head = head->next;
    if (head == nullptr)
    {
        tail = nullptr;
    }
    count--;

    std::cout << "Treating Patient [ID: " << patientToTreat->id << "] " << patientToTreat->name << " (Priority: " << patientToTreat->priority << ")" << std::endl;

    delete patientToTreat;
    delete temp;
}

int PatientManager::getPatientCount()
{
    return count;
}

Patient *PatientManager::findPatient(int id)
{
    PatientNode *curPatient = head;

    while (curPatient != nullptr)
    {
        if (curPatient->patient->id == id)
        {
            return curPatient->patient;
        }
    }
    return nullptr;
};

