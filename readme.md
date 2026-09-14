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