#include <iostream>
#include <string>
#include "ParkingSystem.h"

using namespace std;

Time readTime() {
    int h, m;
    do {
        cout << "Enter time (HH MM, 24-hour): ";
        cin >> h >> m;
    } while (h < 0 || h > 23 || m < 0 || m > 59);
    return Time(h, m);
}

void showStatus(const ParkingLot& lot) {
    cout << "\n--- Lot Status: " << lot.getFreeSlots() << "/"
         << lot.getTotalSlots() << " slots free ---\n";

    for (int i = 1; i <= lot.getTotalSlots(); i++) {
        cout << "Slot " << i << ": ";
        if (lot.isSlotFree(i)) {
            cout << "FREE\n";
        } else {
            cout << "OCCUPIED\n";
        }
    }
}

void doPark(ParkingLot& lot) {
    string plate;
    int type;

    cout << "Enter plate number: ";
    cin >> plate;

    do {
        cout << "Vehicle type (1 = Bike, 2 = Car, 3 = Truck): ";
        cin >> type;
    } while (type < 1 || type > 3);

    Time entry = readTime();
    int id = lot.park(Vehicle(plate, type), entry);

    if (id == 0) {
        cout << "\nSorry, the parking lot is full.\n";
    } else if (id == -1) {
        cout << "\nThis vehicle is already parked.\n";
    } else {
        cout << lot.getTicket(id);   // uses overloaded <<
    }
}

void doUnpark(ParkingLot& lot) {
    int id;
    cout << "Enter ticket ID: ";
    cin >> id;

    Time exit = readTime();
    int fee = lot.unpark(id, exit);

    if (fee == -1) {
        cout << "\nInvalid or already closed ticket.\n";
    } else if (fee == -2) {
        cout << "\nExit time cannot be before entry time.\n";
    } else {
        cout << lot.getTicket(id);   // shows exit time and fee
    }
}

void doLookup(const ParkingLot& lot) {
    int id;
    cout << "Enter ticket ID: ";
    cin >> id;

    if (lot.isValidTicket(id)) {
        cout << lot.getTicket(id);
    } else {
        cout << "\nNo such ticket.\n";
    }
}

int main() {
    ParkingLot lot;
    int choice;

    do {
        cout << "\n===== SMART PARKING SYSTEM =====\n";
        cout << "1. Park a vehicle\n";
        cout << "2. Unpark a vehicle\n";
        cout << "3. Show lot status\n";
        cout << "4. Look up a ticket\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1: doPark(lot);     break;
            case 2: doUnpark(lot);   break;
            case 3: showStatus(lot); break;
            case 4: doLookup(lot);   break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}