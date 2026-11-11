#include "./model/Patient.h"
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
        "09:30",
        };

    Patient patient2 = {
        1002,
        "Sok Tara",
        22,
        "Female",
        "Severe chest pain and difficulty breathing",
        "Waiting"};

    Patient patient3 = {
        1003,
        "Sok Dara",
        22,
        "Male",
        "Severe chest pain and difficulty breathing",
        "Waiting"};

    patientManager.addPatient(&patient1);
    patientManager.addPatient(&patient2);
    patientManager.addPatient(&patient3);

    patientManager.displayPatient(); 
    std::cout << "Patient Total Numbers : " << patientManager.getPatientCount() << std::endl;


    patientManager.removePatient(1003);

    std::cout << "Patient Total Numbers : " << patientManager.getPatientCount() << std::endl;
    patientManager.displayPatient();

    
    Patient p1{1, "Alice", 25, "Female", "Headache", "Waiting"};
    Patient p2{2, "Bob", 40, "Male", "Chest pain", "Waiting"};
    Patient p3{3, "Charlie", 30, "Male", "Fever", "Waiting"};
    Patient p4{4, "David", 55, "Male", "Severe bleeding", "Waiting"};
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
                  << "\n";
    }

    return 0;
}