#ifndef DOCTORMANAGER_H
#define DOCTORMANAGER_H

#include <string>
#include "../model/Doctor.h"

struct DoctorNode{
    Doctor data;
    DoctorNode *next = NULL;
    DoctorNode *pre = NULL;
};

class DoctorManager{
      private:
        DoctorNode *head;
        DoctorNode *tail; // NEW: points to the last node
        int doctorCount;

        DoctorManager(const DoctorManager &) = delete;
        DoctorManager &operator=(const DoctorManager &) = delete; 
    public:
        DoctorManager();
        ~DoctorManager();
        void addDoctor(const Doctor &d);
        bool updateDoctor(int id, const Doctor &newData);
        bool removeDoctor(int id);
        Doctor *findById(int id);
        Doctor *getFirstAvailable();
        Doctor *getAvailableBySpeciality(const std::string &speciality);

        bool assignPatient(int doctorId, int patientId);
        bool releaseDoctor(int doctorId);

        Doctor *getAt(int index); // for looping in the GUI table
        int count() const { return doctorCount; }

 
};
#endif