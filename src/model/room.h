#ifndef ROOM_H
#define ROOM_H

#include <string>
using namespace std;

enum RoomStatus
{
    AVAILABLE,
    OCCUPIED
};

struct Room
{
    int roomID;
    int patientID;
    string roomNumber;
    RoomStatus status;
};

#endif
