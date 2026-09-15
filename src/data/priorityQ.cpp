#include "../include/PriorityQ.h"
 
 
#include <iostream>
using namespace std;
 
PriorityQ::PriorityQ()
{
    head = nullptr;
    tail = nullptr;
    count = 0;
};

bool checkEmergency () {
    if(head == nullptr) {
        return 1;
    }

    else {
        return -1;
    }
}



