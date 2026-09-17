#include "./data/Patient.h"
#include "./include/PatientManager.h"
#include "./include/PriorityQueue.h"
#include <iostream>

int main()
{
    PatientManager patientManager;

    Patient patient1 = {
        1001,
        "Sok Dara",
        22,
        "Male",
        "Severe chest pain and difficulty breathing",
        CRITICAL,
        "09:30",
        "Waiting"};

    Patient patient2 = {
        1002,
        "Sok Tara",
        22,
        "Female",
        "Severe chest pain and difficulty breathing",
        URGENT,
        "09:30",
        "Waiting"};

    Patient patient3 = {
        1003,
        "Sok Dara",
        22,
        "Male",
        "Severe chest pain and difficulty breathing",
        NORMAL,
        "09:30",
        "Waiting"};

    patientManager.addPatient(&patient1);
    patientManager.addPatient(&patient2);
    patientManager.addPatient(&patient3);

    std::string sym = patientManager.findPatientInfo(patient1.id)->symptoms;

    patientManager.removePatient(1003);

    std::cout << patientManager.getPatientCount() << std::endl;
    patientManager.displayPatient();

    //
    Patient p1{1, "Alice", 25, "Female", "Headache", NORMAL, "10:00", "Waiting"};
    Patient p2{2, "Bob", 40, "Male", "Chest pain", CRITICAL, "10:05", "Waiting"};
    Patient p3{3, "Charlie", 30, "Male", "Fever", URGENT, "10:10", "Waiting"};
    Patient p4{4, "David", 55, "Male", "Severe bleeding", URGENT, "10:15", "Waiting"};
    PriorityQueue emergencyQueue;

    emergencyQueue.enqueuePatient(&p1);
    emergencyQueue.enqueuePatient(&p2);
    emergencyQueue.enqueuePatient(&p3);
    emergencyQueue.enqueuePatient(&p4);

    
    std::cout << "\n=== Emergency Queue ===\n";
    emergencyQueue.displayQueue();

    std::cout << "\n=== Treatment Order ===\n";

    Patient *patient;

    while (!emergencyQueue.checkEmpty())
    {
        patient = emergencyQueue.treatNextPatient();

        std::cout << "Treating: "
                  << patient->name
                  << " (Priority: "
                  << patient->priority
                  << ")\n";
    }

    return 0;
}