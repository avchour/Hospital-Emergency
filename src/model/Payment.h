#ifndef PAYMENT
#define PAYMENT
#include <string>

using namespace std;

class Payment
{
    public:
    int pay_id;
    int patientId;
    int doctorId; // add this for revenue per doctor
    double amount;
    string method; // Cash, Card, Insurance
    string date;   // "YYYY-MM-DD"
    bool isPaid;

    Payment(){
        pay_id = NULL;
        patientId = NULL;
        doctorId = NULL;
        amount = NULL;
        method = "Unknown";
        date = "Unknown";
        isPaid = false;
    }
};

#endif