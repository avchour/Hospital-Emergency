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

    // Constructors
    Payment() {
        pay_id = 0;
        patientId = 0;
        doctorId = 0;
        amount = 0.0;
        method = "Unknown";
        service = "General";
        date = "Unknown";
        isPaid = false;
    }

    Payment(int pay_id, int patientId, int doctorId, double amount,
            const std::string &method, const std::string &service,
            const std::string &date, bool isPaid) {
        this->pay_id = pay_id;
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->amount = amount;
        this->method = method;
        this->service = service;
        this->date = date;
        this->isPaid = isPaid;
    }

    // Overload for 7 arguments (backward compatible with original constructor)
    Payment(int pay_id, int patientId, int doctorId, double amount,
            const std::string &method, const std::string &date, bool isPaid) {
        this->pay_id = pay_id;
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->amount = amount;
        this->method = method;
        this->service = "General";
        this->date = date;
        this->isPaid = isPaid;
    }

    // Getters
    int getPayId() const { return pay_id; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    double getAmount() const { return amount; }
    std::string getMethod() const { return method; }
    std::string getService() const { return service; }
    std::string getDate() const { return date; }
    bool getIsPaid() const { return isPaid; }

    // Setters
    void setPayId(int id) { this->pay_id = id; }
    void setPatientId(int id) { this->patientId = id; }
    void setDoctorId(int id) { this->doctorId = id; }
    void setAmount(double amt) { this->amount = amt; }
    void setMethod(const std::string &m) { this->method = m; }
    void setService(const std::string &s) { this->service = s; }
    void setDate(const std::string &d) { this->date = d; }
    void setIsPaid(bool paid) { this->isPaid = paid; }
};

#endif
