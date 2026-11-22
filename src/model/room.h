#ifndef ROOM_H
#define ROOM_H

#include <string>

enum RoomStatus
{
    AVAILABLE,
    OCCUPIED
};

class Room
{
public:
    int roomID;
    int patientID;
    std::string roomNumber;
    RoomStatus status;

    // Constructors
    Room() {
        roomID = 0;
        patientID = -1;
        roomNumber = "";
        status = AVAILABLE;
    }

    Room(int roomID, const std::string &roomNumber, RoomStatus status = AVAILABLE, int patientID = -1) {
        this->roomID = roomID;
        this->patientID = patientID;
        this->roomNumber = roomNumber;
        this->status = status;
    }

    // Getters
    int getRoomID() const { return roomID; }
    int getPatientID() const { return patientID; }
    std::string getRoomNumber() const { return roomNumber; }
    RoomStatus getStatus() const { return status; }

    // Setters
    void setRoomID(int id) { this->roomID = id; }
    void setPatientID(int pid) { this->patientID = pid; }
    void setRoomNumber(const std::string &num) { this->roomNumber = num; }
    void setStatus(RoomStatus s) { this->status = s; }
};

#endif
