#include "ParkingSystem.h"

Ticket::Ticket() {
    id = 0;
    slotNumber = 0;
    fee = 0;
    active = false;
}

Ticket::Ticket(int ticketId, const Vehicle& v, int slotNo, const Time& entry) {
    id = ticketId;
    vehicle = v;
    slotNumber = slotNo;
    entryTime = entry;
    fee = 0;
    active = true;
}

int Ticket::getId() const {
    return id;
}

Vehicle Ticket::getVehicle() const {
    return vehicle;
}

int Ticket::getSlotNumber() const {
    return slotNumber;
}

Time Ticket::getEntryTime() const {
    return entryTime;
}

Time Ticket::getExitTime() const {
    return exitTime;
}

int Ticket::getFee() const {
    return fee;
}

bool Ticket::isActive() const {
    return active;
}

void Ticket::close(const Time& exit) {
    exitTime = exit;

    int minutes = exit - entryTime;      // uses overloaded operator-
    int hours = (minutes + 59) / 60;     // round up to full hours
    if (hours < 1) {
        hours = 1;                       // minimum 1 hour charge
    }

    fee = hours * vehicle.getRatePerHour();
    active = false;
}

std::ostream& operator<<(std::ostream& out, const Ticket& t) {
    out << "\n------- PARKING TICKET -------\n";
    out << "Ticket ID : " << t.getId() << "\n";
    out << "Vehicle   : " << t.getVehicle().getPlate()
        << " (" << t.getVehicle().getTypeName() << ")\n";
    out << "Slot      : " << t.getSlotNumber() << "\n";
    out << "Entry     : " << t.getEntryTime() << "\n";

    if (t.isActive()) {
        out << "Status    : PARKED\n";
    } else {
        out << "Exit      : " << t.getExitTime() << "\n";
        out << "Fee       : Rs. " << t.getFee() << "\n";
        out << "Status    : CLOSED\n";
    }
    out << "------------------------------\n";
    return out;
}