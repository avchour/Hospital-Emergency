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
public:
    int patientID;
    std::string symptoms;

    double temperature;
    int heartRate;
    int bloodPressureSystolic;
    int bloodPressureDiastolic;
    int respiratoryRate;
    int oxygenSaturation;

    TriagePriority priority;

    // Constructors
    TriageRecord() {
        patientID = 0;
        symptoms = "";
        temperature = 36.5;
        heartRate = 75;
        bloodPressureSystolic = 120;
        bloodPressureDiastolic = 80;
        respiratoryRate = 16;
        oxygenSaturation = 98;
        priority = NON_URGENT;
    }

    TriageRecord(int patientID, const std::string &symptoms, double temperature,
                 int heartRate, int bloodPressureSystolic, int bloodPressureDiastolic,
                 int respiratoryRate, int oxygenSaturation, TriagePriority priority) {
        this->patientID = patientID;
        this->symptoms = symptoms;
        this->temperature = temperature;
        this->heartRate = heartRate;
        this->bloodPressureSystolic = bloodPressureSystolic;
        this->bloodPressureDiastolic = bloodPressureDiastolic;
        this->respiratoryRate = respiratoryRate;
        this->oxygenSaturation = oxygenSaturation;
        this->priority = priority;
    }

    // Getters
    int getPatientID() const { return patientID; }
    std::string getSymptoms() const { return symptoms; }
    double getTemperature() const { return temperature; }
    int getHeartRate() const { return heartRate; }
    int getBloodPressureSystolic() const { return bloodPressureSystolic; }
    int getBloodPressureDiastolic() const { return bloodPressureDiastolic; }
    int getRespiratoryRate() const { return respiratoryRate; }
    int getOxygenSaturation() const { return oxygenSaturation; }
    TriagePriority getPriority() const { return priority; }

    // Setters
    void setPatientID(int id) { this->patientID = id; }
    void setSymptoms(const std::string &s) { this->symptoms = s; }
    void setTemperature(double t) { this->temperature = t; }
    void setHeartRate(int hr) { this->heartRate = hr; }
    void setBloodPressureSystolic(int sys) { this->bloodPressureSystolic = sys; }
    void setBloodPressureDiastolic(int dia) { this->bloodPressureDiastolic = dia; }
    void setRespiratoryRate(int rr) { this->respiratoryRate = rr; }
    void setOxygenSaturation(int o2) { this->oxygenSaturation = o2; }
    void setPriority(TriagePriority p) { this->priority = p; }
};

#endif
