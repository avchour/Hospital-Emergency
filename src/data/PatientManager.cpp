#include "../include/PatientManager.h"

#include <iostream>

PatientManager::PatientManager() {}

PatientManager::~PatientManager()
{
    PatientNode *current = head;

    while (current != nullptr)
    {
        PatientNode *temp = current;
        current = current->next;
       // delete temp->patient;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

bool PatientManager::checkEmpty()
{
    return head == nullptr;
}

void PatientManager::addPatient(Patient *patient)
{

    if (patient == nullptr)
        return;

    PatientNode *patientNode = new PatientNode;

    patientNode->patient = patient;
    patientNode->next = nullptr;

    if (checkEmpty())
    {
        head = patientNode;
        tail = patientNode;
    }
    else
    {
        tail->next = patientNode;
        tail = patientNode;
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

int PatientManager::getPatientCount()
{
    return count;
}

Patient *PatientManager::findPatientInfo(int id)
{
    PatientNode *curPatient = head;

    while (curPatient != nullptr)
    {
        if (curPatient->patient->id == id)
        {
            return curPatient->patient;
        }
        curPatient = curPatient->next;
    }
    return nullptr;
}

bool PatientManager::removePatient(int id)
{
    if (checkEmpty())
        return false;

    PatientNode *cur = head;
    PatientNode *prev = nullptr;

    while (cur != nullptr)
    {
        if (cur->patient->id == id)
        {
            if (cur == head)
            {
                head = cur->next;
                if (head == nullptr)
                    tail = nullptr;
            }
            else if (cur == tail)
            {
                tail = prev;
                tail->next = nullptr;
            }
            else
            {
                prev->next = cur->next;
            }

            delete cur;
            count--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }

    return false;
}
