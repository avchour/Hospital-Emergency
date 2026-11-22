#ifndef PATIENT_H
#define PATIENT_H

#include <string>

struct Patient
{
    int id;
    std::string name;
    int age;
    std::string gender;
    std::string phone;
    std::string address;

public:
    // Constructors
    Patient()
        : id(0), name(""), age(0), gender(""), phone(""), address("") {}

    Patient(int id, const std::string &name, int age, const std::string &gender,
            const std::string &phone = "", const std::string &address = "")
        : id(id), name(name), age(age), gender(gender), phone(phone), address(address) {}

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getGender() const { return gender; }
    std::string getPhone() const { return phone; }
    std::string getAddress() const { return address; }

    // Setters
    void setId(int newId) { id = newId; }
    void setName(const std::string &newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setGender(const std::string &newGender) { gender = newGender; }
    void setPhone(const std::string &newPhone) { phone = newPhone; }
    void setAddress(const std::string &newAddress) { address = newAddress; }
};

#endif
