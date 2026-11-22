#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include "../model/room.h"

class PatientManager;

class RoomManager
{
private:
    struct RoomNode
    {
        Room data;
        RoomNode *next;

        RoomNode(Room room)
        {
            data = room;
            next = nullptr;
        }
    };

    RoomNode *head;
    RoomNode *tail;

    int count;
    int nextRoomID;
    int maxRooms;

public:
    RoomManager(int limit = 10);
    ~RoomManager();

    bool addRoom();
    bool assignRoom(int roomID, int patientID);
    bool assignRoom(int roomID, int patientID, const PatientManager &patientManager);
    bool releaseRoom(int roomID);
    bool removeRoom(int roomID);
    void displayRooms() const;

    // Inspection & Query methods
    int getRoomCount() const { return count; }
    int getMaxRooms() const { return maxRooms; }
    Room *getRoomAt(int index) const;
    Room *findRoom(int roomID) const;
    int getAvailableCount() const;
    int getOccupiedCount() const;
};

#endif
