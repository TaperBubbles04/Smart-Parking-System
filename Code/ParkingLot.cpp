#include "ParkingSystem.h"

ParkingLot::ParkingLot() {
    for (int i = 0; i < MAX_SLOTS; i++) {
        slots[i] = Slot(i + 1);     // slot numbers start from 1
    }
    ticketCount = 0;
}

int ParkingLot::findFreeSlot() const {
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (slots[i].isFree()) {
            return i;
        }
    }
    return -1;
}

bool ParkingLot::isAlreadyParked(const std::string& plate) const {
    for (int i = 0; i < ticketCount; i++) {
        if (tickets[i].isActive() && tickets[i].getVehicle().getPlate() == plate) {
            return true;
        }
    }
    return false;
}

int ParkingLot::park(const Vehicle& v, const Time& entry) {
    if (isAlreadyParked(v.getPlate())) {
        return -1;
    }

    int index = findFreeSlot();
    if (index == -1 || ticketCount == MAX_TICKETS) {
        return 0;
    }

    int id = ticketCount + 1;
    tickets[ticketCount] = Ticket(id, v, slots[index].getNumber(), entry);
    slots[index].occupy(id);
    ticketCount++;

    return id;
}

int ParkingLot::unpark(int ticketId, const Time& exit) {
    if (!isValidTicket(ticketId) || !tickets[ticketId - 1].isActive()) {
        return -1;
    }

    Ticket& t = tickets[ticketId - 1];

    if (exit - t.getEntryTime() < 0) {   // uses overloaded operator-
        return -2;
    }

    t.close(exit);
    slots[t.getSlotNumber() - 1].release();

    return t.getFee();
}

int ParkingLot::getTotalSlots() const {
    return MAX_SLOTS;
}

int ParkingLot::getFreeSlots() const {
    int count = 0;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (slots[i].isFree()) {
            count++;
        }
    }
    return count;
}

bool ParkingLot::isSlotFree(int slotNo) const {
    return slots[slotNo - 1].isFree();
}

bool ParkingLot::isValidTicket(int ticketId) const {
    return ticketId >= 1 && ticketId <= ticketCount;
}

Ticket ParkingLot::getTicket(int ticketId) const {
    return tickets[ticketId - 1];
}