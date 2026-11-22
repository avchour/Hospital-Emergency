#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>

class Doctor
{
public:
    int Doc_id;
    std::string Doc_name;
    std::string speciality;
    std::string gender;
    int age;
    std::string Doc_phone;
    std::string shift; // working time 
    bool isAvailable;
    int currentPatientId; // check current patient that Doctor is handling

    // Constructors
    Doctor() {
        Doc_id = 0;
        Doc_name = "";
        speciality = "";
        gender = "";
        age = 0;
        Doc_phone = "";
        shift = "";
        isAvailable = true;
        currentPatientId = -1;
    }

    Doctor(int id, const std::string &name, const std::string &speciality,
           const std::string &gender, int age, const std::string &phone,
           const std::string &shift, bool isAvailable = true, int currentPatientId = -1) {
        this->Doc_id = id;
        this->Doc_name = name;
        this->speciality = speciality;
        this->gender = gender;
        this->age = age;
        this->Doc_phone = phone;
        this->shift = shift;
        this->isAvailable = isAvailable;
        this->currentPatientId = currentPatientId;
    }

    // Getters
    int getDocId() const { return Doc_id; }
    std::string getDocName() const { return Doc_name; }
    std::string getSpeciality() const { return speciality; }
    std::string getGender() const { return gender; }
    int getAge() const { return age; }
    std::string getDocPhone() const { return Doc_phone; }
    std::string getShift() const { return shift; }
    bool getIsAvailable() const { return isAvailable; }
    int getCurrentPatientId() const { return currentPatientId; }

    // Setters
    void setDocId(int id) { this->Doc_id = id; }
    void setDocName(const std::string &name) { this->Doc_name = name; }
    void setSpeciality(const std::string &spec) { this->speciality = spec; }
    void setGender(const std::string &gen) { this->gender = gen; }
    void setAge(int a) { this->age = a; }
    void setDocPhone(const std::string &phone) { this->Doc_phone = phone; }
    void setShift(const std::string &s) { this->shift = s; }
    void setIsAvailable(bool avail) { this->isAvailable = avail; }
    void setCurrentPatientId(int pid) { this->currentPatientId = pid; }
};

#endif
