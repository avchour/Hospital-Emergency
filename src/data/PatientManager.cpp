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

    if (checkEmpty == nullptr)
    {
        head = patientNode;
        tail = patientNode;
        std::cout << "First Node Added.\n";
        count++;
        return;
    }

    tail->next = patientNode;
    tail = patientNode;
    count++;
};

void PatientManager::displayPatient()
{
    PatientNode *curPatient = head;
    while (curPatient != nullptr)
    {
        std::cout << curPatient->patient->name << std::endl;
        curPatient = curPatient->next;
    }
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

