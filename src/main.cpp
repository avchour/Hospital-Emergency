#include "./data/Patient.h"
#include "./include/PatientManager.h"

#include <string>
#include <iostream>

int main()
{
    PatientManager patientManager;

    std::cout << "Initial checkEmpty(): " << (patientManager.checkEmpty() ? "True" : "False") << "\n\n";

    // Create patients with different priorities (1 = High, 2 = Medium, 3 = Low)
    Patient *patient1 = new Patient{
        1001, "Sok Dara", 22, "Male", "Minor headache", 3, "09:30", "Waiting"};

    Patient *patient2 = new Patient{
        1002, "Sok Tara", 25, "Female", "Severe chest pain", 1, "09:32", "Waiting"};

    Patient *patient3 = new Patient{
        1003, "Chan Vanna", 30, "Male", "Fever and cough", 2, "09:35", "Waiting"};

    std::cout << "Adding Patient 1 (Priority 3 - Low)...\n";
    patientManager.addPatient(patient1);

    std::cout << "Adding Patient 2 (Priority 1 - Emergency/High)...\n";
    patientManager.addPatient(patient2);

    std::cout << "Adding Patient 3 (Priority 2 - Medium)...\n";
    patientManager.addPatient(patient3);

    std::cout << "\nDisplaying Priority Queue:\n";
    patientManager.displayQueue();

    std::cout << "\nTreating highest priority patient:\n";
    patientManager.treatNextPatient();

    std::cout << "\nDisplaying Queue after treating top patient:\n";
    patientManager.displayQueue();

    std::cout << "\nTreating remaining patients...\n";
    patientManager.treatNextPatient();
    patientManager.treatNextPatient();

    std::cout << "\nCheck empty after treating all: " << (patientManager.checkEmpty() ? "True" : "False") << "\n";

    return 0;
}
