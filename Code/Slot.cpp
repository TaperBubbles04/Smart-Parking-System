#include "ParkingSystem.h"

Slot::Slot() {
    number = 0;
    occupied = false;
    ticketId = 0;
}

Slot::Slot(int slotNumber) {
    number = slotNumber;
    occupied = false;
    ticketId = 0;
}

int Slot::getNumber() const {
    return number;
}

bool Slot::isFree() const {
    return !occupied;
}

int Slot::getTicketId() const {
    return ticketId;
}

void Slot::occupy(int id) {
    occupied = true;
    ticketId = id;
}

void Slot::release() {
    occupied = false;
    ticketId = 0;
}