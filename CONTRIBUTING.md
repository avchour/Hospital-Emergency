# Contributing to Project Hospital

A C++ program that manages patients in a hospital triage system using a linked list.

## Project Structure

```
project-hospital/
├── src/
│   ├── data/
│   │   ├── Patient.h            # Patient data definition
│   │   └── PatientManager.cpp   # Patient list logic
│   ├── include/
│   │   └── PatientManager.h     # Patient list interface
│   └── main.cpp                 # Entry point
├── readme.md
└── .gitignore
```

## How It Works
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
### Patient (src/data/Patient.h)
Stores a single patient record:
- `id` - unique identifier
- `name` - patient name
- `age` - age in years
- `gender` - "Male" or "Female"
- `symptoms` - description of condition
- `priority` - 1 = high, 2 = medium, 3 = low
- `arrivalTime` - time patient arrived (e.g. "09:30")
- `status` - current state (e.g. "Waiting", "In Treatment")

### PatientManager (src/include/PatientManager.h + src/data/PatientManager.cpp)
Manages a list of patients using a linked list. Available functions:
- `addPatient(Patient*)` - append a patient to the list
- `displayPatient()` - print all patient names
- `findPatient(int id)` - find a patient by ID
- `getPatientCount()` - get total number of patients
- `checkEmpty()` - check if list has no patients
- ` removePatient()`
## Building

Using g++ (Linux/Mac):
```bash
g++ -o pe.exe src/main.cpp src/data/PatientManager.cpp -I src
./pe.exe
```

Using g++ (Windows with MinGW):
```bash
g++ -o pe.exe src/main.cpp src/data/PatientManager.cpp -I src
pe.exe
```

## Adding a New Feature

1. Define any new data in `src/data/` (keep it simple - plain structs)
2. Declare functions in `src/include/`
3. Implement in `src/data/`
4. Wire it up in `src/main.cpp`
5. Build and test manually

## Commit Messages

Use lowercase, describe what changed:
```
add patient priority sorting
fix display list showing wrong names
```

## Things to Know

- This is a C++ learning project using raw linked lists (no STL containers yet)
- PatientManager manually allocates memory (`new`/`delete`) for list nodes
- No build system (Makefile) yet - build manually with g++
- `pe.exe` and object files are gitignored
