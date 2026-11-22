#include "../include/PaymentManager.h"

PaymentManager::PaymentManager()
{
    head = nullptr;
    tail = nullptr;
    paymentCount = 0;
}

PaymentManager::~PaymentManager()
{
    PaymentNode *t = head;
    while (t != nullptr){
        PaymentNode *nextNode = t->next; // save before deleting
        delete t;
        t = nextNode;
    }
}

bool PaymentManager::addPayment(const Payment &p)
{
    if (p.amount <= 0){
        return false; // invalid amount
    }
    if (findById(p.pay_id) != nullptr){
        return false; // duplicate ID
    }
    PaymentNode *newNode = new PaymentNode(p);

    if (head == nullptr){
        head = newNode;
        tail = newNode;
    }else{
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    paymentCount++;
    return true;
}

Payment *PaymentManager::findById(int id)
{
    for (PaymentNode *t = head; t != nullptr; t = t->next){
        if (t->data.pay_id == id){
            return &t->data;
        }
    }
    return nullptr;
}

Payment *PaymentManager::getAt(int index)
{
    PaymentNode *t;
    if (index < 0 || index >= paymentCount){
        return nullptr;
    }

    if (index < paymentCount / 2){
        t = head; // closer to the front
        for (int i = 0; i < index; i++){
            t = t->next;
        }
        return &t->data;
    }else{
        t = tail; // closer to the back
        for (int i = paymentCount - 1; i > index; i--){
            t = t->prev;
        }
        return &t->data;
    }
}

bool PaymentManager::markPaid(int id)
{
    Payment *p = findById(id);
    if (p == nullptr){
        return false;
    }
    p->isPaid = true;
    return true;
}

bool PaymentManager::updatePayment(int id, const Payment &newData)
{
    Payment *p = findById(id);
    if (p == nullptr){
        return false; // no payment with this ID
    }
    if (newData.amount <= 0){
        return false; // reject invalid amount
    }
    p->patientId = newData.patientId;
    p->doctorId = newData.doctorId;
    p->amount = newData.amount;
    p->method = newData.method;
    p->service = newData.service;
    p->date = newData.date;
    p->isPaid = newData.isPaid;
    return true;
}

bool PaymentManager::removePayment(int id)
{
    PaymentNode *t = head;
    while (t != nullptr){
        if (t->data.pay_id == id){
            if (t->prev != nullptr){
                t->prev->next = t->next;
            }else{
                head = t->next; // removing the first node
            }
            if (t->next != nullptr){
                t->next->prev = t->prev;
            }else{
                tail = t->prev; // removing the last node
            }

            delete t;
            paymentCount--;
            return true;
        }
        t = t->next;
    }
    return false;
}

// ---------- REVENUE (paid payments only) ----------
double PaymentManager::getTotalRevenue() const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getPendingAmount() const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (!t->data.isPaid)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getRevenueByDate(const std::string &date) const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid && t->data.date == date)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getRevenueByMethod(const std::string &method) const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid && t->data.method == method)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getRevenueByService(const std::string &service) const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid && t->data.service == service)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getRevenueByDoctor(int doctorId) const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid && t->data.doctorId == doctorId)
            total += t->data.amount;
    }
    return total;
}

double PaymentManager::getRevenueByPatient(int patientId) const
{
    double total = 0;
    for (PaymentNode *t = head; t != nullptr; t = t->next)
    {
        if (t->data.isPaid && t->data.patientId == patientId)
            total += t->data.amount;
    }
    return total;
}