#include "../include/RoomManager.h"
#include "../include/PatientManager.h"
#include <iostream>
using namespace std;

RoomManager::RoomManager(int limit)
{
    head = nullptr;
    tail = nullptr;

    count = 0;
    nextRoomID = 1;

    if (limit <= 0)
        maxRooms = 10;
    else
        maxRooms = limit;
}

RoomManager::~RoomManager()
{
    RoomNode *current = head;

    while (current != nullptr)
    {
        RoomNode *temp = current;
        current = current->next;

        delete temp;
    }

    head = nullptr;
    tail = nullptr;
    count = 0;
}

bool RoomManager::addRoom()
{

    if (count >= maxRooms)
    {
        cout << "Room limit reached! Cannot add more rooms."
             << endl;
        return false;
    }

    Room newRoom;

    newRoom.roomID = nextRoomID;
    newRoom.roomNumber = "R-" + to_string(nextRoomID);
    newRoom.patientID = -1;
    newRoom.status = AVAILABLE;

    RoomNode *newNode = new RoomNode(newRoom);

    if (head == nullptr)
    {

        head = newNode;
        tail = newNode;
    }
    else
    {

        tail->next = newNode;
        tail = newNode;
    }

    count++;
    nextRoomID++;

    cout << "Room " << newRoom.roomNumber
         << " added successfully!" << endl;

    return true;
}

bool RoomManager::assignRoom(
    int roomID,
    int patientID,
    const PatientManager &patientManager)
{
    // 1. Validate Patient ID
    if (patientID <= 0)
    {
        cout << "Invalid Patient ID!" << endl;
        return false;
    }

    Patient *patient = patientManager.findPatientInfo(patientID);

    if (patient == nullptr)
    {
        cout << "Patient ID " << patientID
             << " does not exist!" << endl;
        return false;
    }

    if (head == nullptr)
    {
        cout << "No emergency rooms available!" << endl;
        return false;
    }

    // 4. Check duplicate patient assignment
    RoomNode *current = head;

    while (current != nullptr)
    {
        if (current->data.patientID == patientID)
        {
            cout << "Patient is already assigned to a room!"
                 << endl;
            return false;
        }

        current = current->next;
    }

    // 5. Find selected room
    current = head;

    while (current != nullptr)
    {
        if (current->data.roomID == roomID)
        {
            // Check room availability
            if (current->data.status == OCCUPIED)
            {
                cout << "Room is already occupied!" << endl;
                return false;
            }

            // Assign patient
            current->data.patientID = patientID;
            current->data.status = OCCUPIED;

            cout << "Patient " << patient->name
                 << " (ID: " << patientID << ") assigned to "
                 << current->data.roomNumber << " successfully!"
                 << endl;

            return true;
        }

        current = current->next;
    }

    // 6. Room not found
    cout << "Room ID " << roomID << " does not exist!"
         << endl;

    return false;
}

bool RoomManager::removeRoom(int roomID)
{
    if (head == nullptr)
    {
        cout << "No rooms available to remove!" << endl;
        return false;
    }

    RoomNode *current = head;
    RoomNode *prev = nullptr;

    while (current != nullptr)
    {
        if (current->data.roomID == roomID)
        {
            if (current->data.status == OCCUPIED)
            {
                cout << "Cannot remove room " << current->data.roomNumber
                     << "! Room is currently occupied." << endl;
                return false;
            }

            if (current == head)
            {
                head = head->next;
                if (head == nullptr)
                    tail = nullptr;
            }
            else
            {
                prev->next = current->next;
                if (current == tail)
                    tail = prev;
            }

            cout << "Room " << current->data.roomNumber
                 << " removed successfully!" << endl;

            delete current;
            count--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    cout << "Room ID " << roomID << " does not exist!"
         << endl;

    return false;
}

void RoomManager::displayRooms() const
{
    if (head == nullptr)
    {
        cout << "No rooms to display!" << endl;
        return;
    }

    cout << "\n===== EMERGENCY ROOMS (" << count << "/" << maxRooms << ") =====" << endl;

    RoomNode *current = head;

    while (current != nullptr)
    {
        cout << "Room ID: " << current->data.roomID
             << " | Room Number: " << current->data.roomNumber
             << " | Status: " << (current->data.status == AVAILABLE ? "AVAILABLE" : "OCCUPIED");

        if (current->data.status == OCCUPIED)
        {
            cout << " | Patient ID: " << current->data.patientID;
        }

        cout << endl;

        current = current->next;
    }
}

bool RoomManager::assignRoom(int roomID, int patientID)
{

    if (patientID <= 0)
    {
        cout << "Invalid Patient ID!" << endl;
        return false;
    }

    if (head == nullptr)
    {
        cout << "No emergency rooms available!" << endl;
        return false;
    }

    // dubplicate
    RoomNode *current = head;

    while (current != nullptr)
    {
        if (current->data.patientID == patientID)
        {
            cout << "Patient is already assigned to a room!"
                 << endl;
            return false;
        }

        current = current->next;
    }

    current = head;

    while (current != nullptr)
    {
        if (current->data.roomID == roomID)
        {

            if (current->data.status == OCCUPIED)
            {
                cout << "Room is already occupied!" << endl;
                return false;
            }

            // Assign patient
            current->data.patientID = patientID;
            current->data.status = OCCUPIED;

            cout << "Patient ID " << patientID << " assigned to "
                 << current->data.roomNumber << " successfully!"
                 << endl;

            return true;
        }

        current = current->next;
    }

    // 5. Room not found
    cout << "Room ID " << roomID << " does not exist!"
         << endl;

    return false;
}
