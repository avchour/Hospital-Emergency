#ifdef DOCTOR_H
#define DOCTOR_H

#include<string>

struct Doctor{
    int Doc_id;
    std::string Doc_name;
    std::string speciality;
    std::string gender;
    int age;
    std::string Doc_phone;
    std::string shift; //working time 
    bool isAvailable;
    int currentPatientId; //check current patient that Doctor is handling
};

#endif 
