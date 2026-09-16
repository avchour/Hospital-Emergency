#ifndef PATIENT_H
#define PATIENT_H

#include <string>
enum Priority
{
    CRITICAL = 1,
    URGENT = 2,
    NORMAL = 3
};
struct Patient
{
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string symptoms;
    Priority priority;
    std::string arrivalTime;
    std::string status;
};

#endif