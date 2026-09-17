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

    return 0;
}