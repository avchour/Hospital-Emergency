#ifndef PAYMENT
#define PAYMENT
#include <string>

struct Payment
{
    int Pay_id;
    int patientId;
    std::string patientName;
    int patientAge;
    std::string patientSex;
    int doctorId; // add this for revenue per doctor
    double amount;
    std::string method; // Cash, Card, Insurance
    std::string date;   // "YYYY-MM-DD"
    bool isPaid = false;
};

#endif