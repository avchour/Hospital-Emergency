#include "./data/Patient.h"
#include "./include/PatientManager.h"

#include <string>
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
        1,
        "09:30",
        "Waiting"};

    Patient patient2 = {
        1002,
        "Sok Tara",
        22,
        "Female",
        "Severe chest pain and difficulty breathing",
        1,
        "09:30",
        "Waiting"};

    patientManager.addPatient(&patient1);
    patientManager.addPatient(&patient2);

    std::string sym = patientManager.findPatient(patient1.id)->symptoms;

    patientManager.displayPatient();

    std::cout << sym << std::endl;

    return 0;
}
