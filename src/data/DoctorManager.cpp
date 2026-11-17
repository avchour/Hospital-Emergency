#include "../include/DoctorManager.h"
using namespace std;

DoctorManager::DoctorManager()
{
    head = NULL;
    tail = NULL;
    doctorCount = 0;
}
DoctorManager::~DoctorManager()
{
    DoctorNode *current = head;
    while (current != NULL)
    {
        DoctorNode *nextNode = current->next;
        delete current;
        current = nextNode;
    }
}

void DoctorManager::addDoctor(const Doctor& d){
    DoctorNode *newNode = new DoctorNode;
    newNode->data = d;

    if(head == NULL){
        head = newNode;
        tail = newNode;
    }else{
        newNode->pre = tail;
        tail->next = newNode;
        tail = newNode;
    }
    doctorCount++;
}
bool DoctorManager::removeDoctor(int id){
    DoctorNode *current = head;

    while(current != NULL){
        if(current->data.Doc_id == id){
            if(current->pre != NULL){
                current->pre->next = current->next; // remove the current and let the pre point to next
            }else{
                head = current->next;
            }
            if(current->next != NULL){
                current->next->pre = current->pre;
            }else{
                tail = current->pre;
            }
            delete current;
            doctorCount--;
            return true;
            
        }
        current = current->next;
    }
    return false;
}

Doctor* DoctorManager::findById(int id) {
    DoctorNode *t = head;
    for (int i = 0; i < doctorCount; i++)
    {
        if(t->data.Doc_id == id){
            return &t->data;
        }
        t = t->next;
    }
    return NULL;
}

Doctor* DoctorManager::getFirstAvailable(){
    DoctorNode *t = head;
    for (int i = 0; i < doctorCount; i++){
        if(t->data.isAvailable){
            return &t->data;
        }
        t = t->next;
    }
    return NULL;
}

Doctor* DoctorManager::getAvailableBySpeciality(const string & speciality){
    DoctorNode *t = head;
    for (int i = 0; i < doctorCount; i++){
        if(t->data.isAvailable && t->data.speciality == speciality){
            return &t->data;
        }
    }
    return NULL;
}

bool DoctorManager::assignPatient(int doctorId, int patientId){
    Doctor *d = findById(doctorId);
    if ( d == NULL || !d->isAvailable){
        return false;
    }
    d->isAvailable = false;
    d->currentPatientId = patientId;
    return true;
}

bool DoctorManager::releaseDoctor(int doctorId){
    Doctor *d = findById(doctorId);
    if(d == NULL){
        return false;
    }
    d->isAvailable = true;
    d->currentPatientId = -1;
    return true;
}

Doctor* DoctorManager::getAt(int index){
    if(index < 0 || index >= doctorCount){
        return nullptr;
    }
    if (index < doctorCount / 2)
    {
        // closer to the front: walk forward from head
        DoctorNode *cur = head;
        for (int i = 0; i < index; i++)
            cur = cur->next;
        return &cur->data;
    }
    else
    {
        // closer to the back: walk backward from tail
        DoctorNode *cur = tail;
        for (int i = doctorCount - 1; i > index; i--)
            cur = cur->pre;
        return &cur->data;
    }
}