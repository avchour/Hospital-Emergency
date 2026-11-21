#ifndef PAYMENT_H
#define PAYMENT_H
#include <string>

class Payment
{
    public:
    int pay_id;
    int patientId;
    int doctorId; // add this for revenue per doctor
    double amount;
    std::string method; // Cash, Card, Insurance
    std::string service;
    std::string date; // "YYYY-MM-DD"
    bool isPaid;

    Payment(){
        pay_id = 0;
        patientId = 0;
        doctorId = 0;
        amount = 0.0;
        method = "Unknown";
        date = "Unknown";
        isPaid = false;
    }
    Payment(int pay_id, int patientId, int doctorId, double amount, std::string method, std::string date, bool isPaid){
        this->pay_id = pay_id;
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->amount = amount;
        this->method = method;
        this->date = date;
        this->isPaid = isPaid;
    }
};

#endif