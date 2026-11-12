#include "../include/PatientManager.h"
#include "../include/utils.h"
#include <iostream>
#include <string>
PatientManager::PatientManager() = default;

PatientManager::~PatientManager()
{
    // clar list = destructor
    clearList();
}

bool PatientManager::checkEmpty() const
{
    return head == nullptr;
}

void PatientManager::clearList()
{
    PatientNode *current = head;

    while (current != nullptr)
    {
        PatientNode *temp = current;
        current = current->next;

        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

PatientManager::PatientNode *PatientManager::findNode(int id) const
{
    PatientNode *current = head;

    while (current != nullptr)
    {
        if (current->patient->id == id)
            return current;

        current = current->next;
    }

    return nullptr;
}

int PatientManager::addPatient( Patient *patient)
{

  
    if (patient == nullptr)
        return 0;

    patient->id = nextPatientID++;
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
    return patient->id;
}

void PatientManager::displayPatient() const
{
    if (checkEmpty())
    {
        std::cout << "No patients found.\n";
        return;
    }
    PatientNode *curPatient = head;
    while (curPatient != nullptr)
    {
        std::cout << "ID: " << curPatient->patient->id << '\n';
        std::cout << "Name: " << curPatient->patient->name << '\n';
        std::cout << "Age: " << curPatient->patient->age << '\n';
        std::cout << "Gender: " << curPatient->patient->gender << '\n';
        std::cout << "----------------------\n";

        curPatient = curPatient->next;
    }
}

int PatientManager::getPatientCount() const
{
    return count;
}

Patient *PatientManager::findPatientInfo(int id)const
{
    PatientNode *node = findNode(id);

    if (node == nullptr){
        return nullptr;
    }
    return node->patient;
}

bool PatientManager::removePatient(int id)
{
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
            else
            {
                prev->next = cur->next;
            }
            if (cur == tail)
            {
                tail = prev;
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

bool PatientManager::updatePatient(int id, PatientField field, const std::string &newValue)
{
    PatientNode *node = findNode(id);

    if (node == nullptr)
        return false;

    switch (field)
    {
    case NAME:
        if (newValue.empty())
            return false;

        node->patient->name = newValue;
        break;

    case AGE:
        if (!Utils::isValidNonNegativeInteger(newValue))
            return false;

        node->patient->age = std::stoi(newValue);
        break;

    case GENDER:
        if (newValue.empty())
            return false;

        node->patient->gender = newValue;
        break;

    case PHONE:
        if (!Utils::isValidPhone(newValue))
            return false;

        node->patient->phone = newValue;
        break;

    case ADDRESS:
        node->patient->address = newValue;
        break;

    default:
        return false;
    }

    return true;
}
