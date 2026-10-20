#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include <iostream>
#include <string>

// ===================== Time =====================
class Time {
private:
    int hour;
    int minute;

public:
    Time();
    Time(int h, int m);

    int getHour() const;
    int getMinute() const;

    // Returns difference in minutes: (this - other)
    int operator-(const Time& other) const;
};

std::ostream& operator<<(std::ostream& out, const Time& t);


// ===================== Vehicle =====================
class Vehicle {
private:
    std::string plate;
    int type;   // 1 = Bike, 2 = Car, 3 = Truck

public:
    Vehicle();
    Vehicle(std::string plateNumber, int vehicleType);

    std::string getPlate() const;
    int getType() const;
    std::string getTypeName() const;
    int getRatePerHour() const;
};


// ===================== Slot =====================
class Slot {
private:
    int number;
    bool occupied;
    int ticketId;   // 0 when the slot is free

public:
    Slot();
    Slot(int slotNumber);

    int getNumber() const;
    bool isFree() const;
    int getTicketId() const;

    void occupy(int id);
    void release();
};


// ===================== Ticket =====================
class Ticket {
private:
    int id;
    Vehicle vehicle;    // composition
    int slotNumber;
    Time entryTime;     // composition
    Time exitTime;      // composition
    int fee;
    bool active;

public:
    Ticket();
    Ticket(int ticketId, const Vehicle& v, int slotNo, const Time& entry);

    int getId() const;
    Vehicle getVehicle() const;
    int getSlotNumber() const;
    Time getEntryTime() const;
    Time getExitTime() const;
    int getFee() const;
    bool isActive() const;

    // Closes the ticket and calculates the fee
    void close(const Time& exit);
};

std::ostream& operator<<(std::ostream& out, const Ticket& t);


// ===================== ParkingLot =====================
const int MAX_SLOTS = 10;
const int MAX_TICKETS = 100;

class ParkingLot {
private:
    Slot slots[MAX_SLOTS];          // composition
    Ticket tickets[MAX_TICKETS];    // composition
    int ticketCount;

    int findFreeSlot() const;
    bool isAlreadyParked(const std::string& plate) const;

public:
    ParkingLot();

    // Returns ticket id (>0) on success, 0 if lot is full, -1 if plate already parked
    int park(const Vehicle& v, const Time& entry);

    // Returns fee (>0) on success, -1 if ticket invalid/closed, -2 if exit time is before entry
    int unpark(int ticketId, const Time& exit);

    int getTotalSlots() const;
    int getFreeSlots() const;
    bool isSlotFree(int slotNo) const;
    bool isValidTicket(int ticketId) const;
    Ticket getTicket(int ticketId) const;
};

#endif