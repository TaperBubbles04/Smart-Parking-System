#include <iostream>
#include <string>
#include <cctype>
#include "ParkingSystem.h"

using namespace std;

Time readTime()
{
    int h, m;
    string period;
    while (true)
    {
        cout << "Enter time (HH MM AM/PM, e.g. 9 30 PM): ";
        cin >> h >> m >> period;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Use spaces, like: 9 30 PM\n";
            continue;
        }
        for (size_t i = 0; i < period.length(); i++)
        {
            period[i] = toupper(period[i]);
        }
        if (h >= 1 && h <= 12 && m >= 0 && m <= 59 && (period == "AM" || period == "PM"))
        {
            break;
        }
        cout << "Invalid time. Hour is 1-12, minute is 0-59, then AM or PM.\n";
    }

    // Convert 12-hour to 24-hour for internal storage
    if (period == "AM")
    {
        if (h == 12) h = 0; // 12 AM = midnight
    }
    else
    {
        if (h != 12) h += 12; // 12 PM stays 12
    }
    return Time(h, m);
}

void showHelp()
{
    cout << "\n=========== HOW THIS SYSTEM WORKS ===========\n";
    cout << "This is a small parking lot with 10 slots.\n\n";

    cout << "PARKING (option 1)\n";
    cout << "  - Enter the plate number, vehicle type and entry time.\n";
    cout << "  - The system gives you the first free slot and prints a ticket.\n";
    cout << "  - Keep your Ticket ID, you need it to leave.\n";
    cout << "  - A vehicle that is already parked cannot be parked again.\n\n";

    cout << "LEAVING (option 2)\n";
    cout << "  - Enter your Ticket ID and the exit time.\n";
    cout << "  - The system finds the time spent, calculates the fee,\n";
    cout << "    frees the slot and prints the closed ticket.\n";
    cout << "  - Exit time cannot be earlier than entry time.\n\n";

    cout << "FEE RULES\n";
    cout << "  - Rates per hour:  Bike Rs. 10 | Car Rs. 20 | Truck Rs. 40\n";
    cout << "  - Time is rounded UP to full hours (95 min = 2 hours).\n";
    cout << "  - Minimum charge is 1 hour.\n\n";

    cout << "OTHER OPTIONS\n";
    cout << "  - Option 3 shows which slots are free or occupied.\n";
    cout << "  - Option 4 looks up any ticket by its ID.\n\n";

    cout << "TIME FORMAT\n";
    cout << "  - Enter hour, minute and AM/PM separated by spaces.\n";
    cout << "  - Examples: 9 30 AM | 12 05 PM | 11 45 pm\n";
    cout << "  - All times must be on the same day (no overnight stays).\n";
    cout << "=============================================\n";
}

void showStatus(const ParkingLot &lot)
{
    cout << "\n--- Lot Status: " << lot.getFreeSlots() << "/"
         << lot.getTotalSlots() << " slots free ---\n";

    for (int i = 1; i <= lot.getTotalSlots(); i++)
    {
        cout << "Slot " << i << ": ";
        if (lot.isSlotFree(i))
        {
            cout << "FREE\n";
        }
        else
        {
            cout << "OCCUPIED\n";
        }
    }
}

void doPark(ParkingLot &lot)
{
    string plate;
    int type;

    cout << "Enter plate number: ";
    cin >> plate;

    do
    {
        cout << "Vehicle type (1 = Bike, 2 = Car, 3 = Truck): ";
        cin >> type;
    } while (type < 1 || type > 3);

    Time entry = readTime();
    int id = lot.park(Vehicle(plate, type), entry);

    if (id == 0)
    {
        cout << "\nSorry, the parking lot is full.\n";
    }
    else if (id == -1)
    {
        cout << "\nThis vehicle is already parked.\n";
    }
    else
    {
        cout << lot.getTicket(id); // uses overloaded <<
    }
}

void doUnpark(ParkingLot &lot)
{
    int id;
    cout << "Enter ticket ID: ";
    cin >> id;

    Time exit = readTime();
    int fee = lot.unpark(id, exit);

    if (fee == -1)
    {
        cout << "\nInvalid or already closed ticket.\n";
    }
    else if (fee == -2)
    {
        cout << "\nExit time cannot be before entry time.\n";
    }
    else
    {
        cout << lot.getTicket(id); // shows exit time and fee
    }
}

void doLookup(const ParkingLot &lot)
{
    int id;
    cout << "Enter ticket ID: ";
    cin >> id;

    if (lot.isValidTicket(id))
    {
        cout << lot.getTicket(id);
    }
    else
    {
        cout << "\nNo such ticket.\n";
    }
}

int main()
{
    ParkingLot lot;
    int choice;

    do
    {
        cout << "\n===== SMART PARKING SYSTEM =====\n";
        cout << "1. Park a vehicle\n";
        cout << "2. Unpark a vehicle\n";
        cout << "3. Show lot status\n";
        cout << "4. Look up a ticket\n";
        cout << "5. How this system works\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            doPark(lot);
            break;
        case 2:
            doUnpark(lot);
            break;
        case 3:
            showStatus(lot);
            break;
        case 4:
            doLookup(lot);
            break;
        case 5:
            showHelp();
            break;
        case 0:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}