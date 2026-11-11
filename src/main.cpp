#include "./model/Patient.h"
#include "./include/PatientManager.h"
#include "./include/PriorityQueue.h"
#include <iostream>
using namespace std;


int main()
{
    PatientManager manager;

    Patient p1;
    p1.name = "John Smith";
    p1.age = 25;
    p1.gender = "Male";
    p1.phone = "012345678";
    p1.address = "Phnom Penh";

    Patient p2;
    p2.name = "Sarah Johnson";
    p2.age = 32;
    p2.gender = "Female";
    p2.phone = "098765432";
    p2.address = "Siem Reap";

    Patient p3;
    p3.name = "David Brown";
    p3.age = 45;
    p3.gender = "Male";
    p3.phone = "011223344";
    p3.address = "Battambang";

    Patient p4;
    p4.name = "Emily Davis";
    p4.age = 19;
    p4.gender = "Female";
    p4.phone = "096112233";
    p4.address = "Kampot";

    // =================================
    // 2. Test addPatient()
    // =================================

    cout << "\n===== ADD PATIENTS =====\n";

    cout << "Generated ID: "
         << manager.addPatient(&p1) << endl;

    cout << "Generated ID: "
         << manager.addPatient(&p2) << endl;

    cout << "Generated ID: "
         << manager.addPatient(&p3) << endl;

    cout << "Generated ID: "
         << manager.addPatient(&p4) << endl;

    // =================================
    // 3. Test displayPatient()
    // =================================

    cout << "\n===== DISPLAY PATIENTS =====\n";

    manager.displayPatient();

    // =================================
    // 4. Test getPatientCount()
    // =================================

    cout << "\n===== PATIENT COUNT =====\n";

    cout << "Total: "
         << manager.getPatientCount() << endl;

    // =================================
    // 5. Test findPatientInfo()
    // =================================

    cout << "\n===== SEARCH PATIENT =====\n";

    Patient *found = manager.findPatientInfo(2);

    if (found != nullptr)
    {
        cout << "Patient found: "
             << found->name << endl;
    }
    else
    {
        cout << "Patient not found.\n";
    }

    // Search nonexistent ID
    found = manager.findPatientInfo(99);

    if (found == nullptr)
    {
        cout << "ID 99 not found.\n";
    }

    // =================================
    // 6. Test removePatient()
    // =================================

    cout << "\n===== REMOVE PATIENT =====\n";

    if (manager.removePatient(2))
        cout << "Patient ID 2 removed.\n";
    else
        cout << "Removal failed.\n";

    manager.displayPatient();

    cout << "Total: "
         << manager.getPatientCount() << endl;

    // =================================
    // 7. Test ID generation after deletion
    // =================================

    cout << "\n===== ADD AFTER DELETE =====\n";

    Patient p5;
    p5.name = "Michael Wilson";
    p5.age = 28;
    p5.gender = "Male";
    p5.phone = "015667788";
    p5.address = "Kampong Cham";

    cout << "Generated ID: "
         << manager.addPatient(&p5) << endl;

    manager.displayPatient();

    cout << "\n===== END TEST =====\n";

    return 0;
}