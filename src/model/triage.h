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

class TriageRecord
{
private:
    int patientID;
    std::string symptoms;

    double temperature;
    int heartRate;
    int bloodPressureSystolic;
    int bloodPressureDiastolic;
    int respiratoryRate;
    int oxygenSaturation;

    TriagePriority priority;

public:
    // ✏️ MANUAL CHANGE: Getter methods

    int getPatientID() const
    {
        return patientID;
    }

    std::string getSymptoms() const
    {
        return symptoms;
    }

    double getTemperature() const
    {
        return temperature;
    }

    int getHeartRate() const
    {
        return heartRate;
    }

    int getBloodPressureSystolic() const
    {
        return bloodPressureSystolic;
    }

    int getBloodPressureDiastolic() const
    {
        return bloodPressureDiastolic;
    }

    int getOxygenSaturation() const
    {
        return oxygenSaturation;
    }

    TriagePriority getPriority() const
    {
        return priority;
    }

    void setPriority(TriagePriority newPriority)
    {
        priority = newPriority;
    }
};

#endif
