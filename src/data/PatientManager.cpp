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

        delete temp->patient;
        delete temp;
        //delete temp;
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
        if (current->patient->getId() == id)
            return current;

        current = current->next;
    }

    return nullptr;
}

int PatientManager::addPatient( Patient *patient)
{

  
    if (patient == nullptr)
        return 0;

    patient->setId(nextPatientID++);
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
    return patient->getId();
}

// void PatientManager::displayPatient() const
// {
//     if (checkEmpty())
//     {
//         std::cout << "No patients found.\n";
//         return;
//     }
//     PatientNode *curPatient = head;
//     while (curPatient != nullptr)
//     {
//         std::cout << "ID: " << curPatient->patient->id << '\n';
//         std::cout << "Name: " << curPatient->patient->name << '\n';
//         std::cout << "Age: " << curPatient->patient->age << '\n';
//         std::cout << "Gender: " << curPatient->patient->gender << '\n';
//         std::cout << "----------------------\n";

//         curPatient = curPatient->next;
//     }
// }

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

Patient *PatientManager::getPatientAt(int index) const
{
    if (index < 0 || index >= count)
        return nullptr;

    PatientNode *cur = head;
    for (int i = 0; i < index && cur != nullptr; i++)
    {
        cur = cur->next;
    }

    return cur ? cur->patient : nullptr;
}

std::array<int, 5> PatientManager::getPriorityCounts() const
{
    return {0, 0, 0, 0, 0};
}

bool PatientManager::removePatient(int id)
{
    PatientNode *cur = head;
    PatientNode *prev = nullptr;

    while (cur != nullptr)
    {
        if (cur->patient->getId() == id)
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

            delete cur->patient;
            delete cur;
            //delete cur;
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

        node->patient->setName(newValue);
        break;

    case AGE:
        if (!Utils::isValidNonNegativeInteger(newValue))
            return false;

        node->patient->setAge(std::stoi(newValue));
        break;

    case GENDER:
        if (newValue.empty())
            return false;

        node->patient->setGender(newValue);
        break;

    case PHONE:
        if (!Utils::isValidPhone(newValue))
            return false;

        node->patient->setPhone(newValue);
        break;

    case ADDRESS:
        node->patient->setAddress(newValue);
        break;

    default:
        return false;
    }

    return true;
}
