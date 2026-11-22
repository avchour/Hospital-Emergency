#ifndef PATIENT_H
#define PATIENT_H

#include <string>

class Patient
{
public:
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string phone;
    std::string address;

    // Constructors
    Patient() {
        id = 0;
        name = "";
        age = 0;
        gender = "";
        phone = "";
        address = "";
    }

    Patient(int id, const std::string &name, int age, const std::string &gender,
            const std::string &phone = "", const std::string &address = "") {
        this->id = id;
        this->name = name;
        this->age = age;
        this->gender = gender;
        this->phone = phone;
        this->address = address;
    }

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }

    // Setters
    void setId(int newId) { this->id = newId; }
    void setName(const std::string &newName) { this->name = newName; }
    void setAge(int newAge) { this->age = newAge; }
    void setGender(const std::string &newGender) { this->gender = newGender; }
    void setPhone(const std::string &newPhone) { this->phone = newPhone; }
    void setAddress(const std::string &newAddress) { this->address = newAddress; }
};

#endif
