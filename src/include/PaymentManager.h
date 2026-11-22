#ifndef PAYMENTMANAGER_H
#define PAYMENTMANAGER_H

#include <string>
#include "../model/Payment.h"

class PaymentNode{
    public:
    Payment data;
    PaymentNode *next;
    PaymentNode *prev;

    PaymentNode(){
        next = nullptr;
        prev = nullptr;
    }

    PaymentNode(const Payment &p)
    {
        data = p;
        next = nullptr;
        prev = nullptr;
    }
};

class PaymentManager{
    private:
        PaymentNode *head, *tail;
        int paymentCount;

        PaymentManager(const PaymentManager &) = delete;
        PaymentManager &operator=(const PaymentManager &) = delete;
    public:
        PaymentManager();
        ~PaymentManager();

        //Manage Payment
        bool addPayment(const Payment &p);
        bool removePayment(int id);
        bool updatePayment(int id, const Payment &newData);
        bool markPaid(int id);
        Payment *findById(int id);
        Payment *getAt(int index); //get payment at which position
        int count() const { return paymentCount; } //to read payment count (time complexity O(1))

        // Revenue (counts only payments with isPaid == true)
        double getTotalRevenue() const;
        double getPendingAmount() const;                            // unpaid total
        double getRevenueByDate(const std::string &date) const;     // "2026-11-20"
        double getRevenueByMethod(const std::string &method) const; // "Cash"
        double getRevenueByService(const std::string &service) const;
        double getRevenueByDoctor(int doctorId) const;
        double getRevenueByPatient(int patientId) const;
};

#endif