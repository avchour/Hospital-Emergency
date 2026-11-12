#ifndef TRIAGERECORD_H
#define TRIAGERECORD_H

#include <string>

enum TriagePriority
{
    RESUSCITATION = 1,
    EMERGENT = 2,
    URGENT = 3,
    LESS_URGENT = 4,
    NON_URGENT = 5
};
struct TriageRecord
{
    int patientID;
    std::string symptoms;

    double temperature;
    int heartRate;
    int bloodPressureSystolic;
    int bloodPressureDiastolic;
    int respiratoryRate;
    int oxygenSaturation;

    TriagePriority priority;
};

#endif