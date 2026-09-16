#ifndef PATIENT_H
#define PATIENT_H

#include <string>

struct Patient
{
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string symptoms;
    int priority;
    std::string arrivalTime;
    std::string status;
};

#endif