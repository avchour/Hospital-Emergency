Achitecture

                  ┌──────────────┐
                  │   main.cpp   │
                  └──────┬───────┘
                         ↓
                ┌──────────────────┐
                │  HospitalSystem  │
                │    CLI / Menu    │
                └────────┬─────────┘
                         │
        ┌────────────────┼────────────────┐
        ↓                ↓                ↓
 PatientManager    PriorityQueue        Queue
   Linked List      Emergency DSA       FIFO DSA
        │                │                │
        └────────────────┼────────────────┘
                         ↓
                  Search / Sort

"We use a linked list to implement a priority queue. The linked list allows dynamic insertion and removal of patients, while the priority queue logic ensures critical patients are treated before urgent and normal patients. Patients with equal priority follow first-come-first-served order."                  