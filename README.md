# Smart Parking Lot Management System

A console-based parking lot manager written in C++, built as an Object-Oriented Programming course project. It parks vehicles, issues tickets, calculates fees and frees slots on exit, using core OOP concepts throughout.

> **Status:** Mid-sem version. Covers classes and objects, constructors, encapsulation, abstraction, composition and operator overloading. Inheritance, polymorphism, STL, exception handling and stream handling are planned for the final version (see the Roadmap).

---

## Features

- Park a vehicle (Bike, Car or Truck) and get a printed ticket
- Automatic allocation of the first free slot
- Unpark using the Ticket ID, with automatic fee calculation
- Live lot status showing which slots are free or occupied
- Ticket lookup by ID
- Built-in help screen explaining how the system works
- Input checks: duplicate plates, full lot, invalid tickets and exit-before-entry are all rejected

---

## OOP Concepts Used

| Concept                  | Where it is used                                                                                                          |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------- |
| **Classes and Objects**  | `Time`, `Vehicle`, `Slot`, `Ticket`, `ParkingLot`                                                                         |
| **Constructors**         | Default and parameterized constructors in every class                                                                     |
| **Encapsulation**        | All data members are `private`; access only through public methods                                                        |
| **Abstraction**          | `main.cpp` only calls `park()` and `unpark()`. Slot search, ticket creation and fee logic stay hidden inside `ParkingLot` |
| **Composition**          | `ParkingLot` has `Slot` and `Ticket` arrays. `Ticket` has a `Vehicle` and two `Time` objects                              |
| **Operator Overloading** | `Time::operator-` gives the minutes between two times. `operator<<` prints `Time` and `Ticket`                            |

---

## Project Structure

```
Smart-Parking-System/
├── Code/
│   ├── ParkingSystem.h    # Interface: declarations of all five classes
│   ├── Time.cpp           # Time class implementation (operator- and operator<<)
│   ├── Vehicle.cpp        # Vehicle class implementation
│   ├── Slot.cpp           # Slot class implementation
│   ├── Ticket.cpp         # Ticket class implementation (fee calculation, operator<<)
│   ├── ParkingLot.cpp     # ParkingLot class implementation (park / unpark logic)
│   └── main.cpp           # Console CLI driver
├── Visual_CPP/
│   ├── ParkingLot_Win32.cpp # Visual C++ desktop GUI implementation
│   └── README_VISUAL_CPP.md # Visual C++ GUI setup and execution guide
├── REPORT.md              # Academic OOP concepts report
├── .gitignore             # Git ignore rules for build artifacts
└── README.md
```

The interface and implementation are separated: one header holds every declaration, and each class has its own `.cpp` file. All console `cin`/`cout` code lives only in `main.cpp`. The Visual C++ GUI replaces `main.cpp` without modifying any domain classes.

---

## Class Overview

| Class        | Responsibility                                                                                                         |
| ------------ | ---------------------------------------------------------------------------------------------------------------------- |
| `Time`       | Stores hour and minute. Overloads `-` to return the difference in minutes and `<<` to print in 12-hour AM/PM format    |
| `Vehicle`    | Stores plate number and type. Provides the type name and hourly rate                                                   |
| `Slot`       | Represents one parking slot. Tracks whether it is free and which ticket occupies it                                    |
| `Ticket`     | Stores vehicle, slot, entry and exit times, and fee. Closes itself and calculates the fee. Overloads `<<` for printing |
| `ParkingLot` | Owns all slots and tickets. Provides `park()`, `unpark()` and status queries                                           |

---

## Fee Rules

| Vehicle | Rate per hour |
| ------- | ------------- |
| Bike    | Rs. 10        |
| Car     | Rs. 20        |
| Truck   | Rs. 40        |

- Time is **rounded up** to full hours (95 minutes is charged as 2 hours)
- **Minimum charge** is 1 hour
- All times use the 12-hour format with AM/PM (e.g., 9 30 AM) and must fall on the same day

---

## Build and Run

### Using g++ (command line)

```bash
g++ main.cpp Time.cpp Vehicle.cpp Slot.cpp Ticket.cpp ParkingLot.cpp -o parking
./parking
```

### Using Visual Studio

1. Create a new **Console App** (C++) project
2. Delete the auto-generated sample `.cpp` file
3. Add `ParkingSystem.h` under Header Files, and all six `.cpp` files under Source Files
4. Press **Ctrl + F5** to build and run

If you see a precompiled header error, go to _Project → Properties → C/C++ → Precompiled Headers_ and choose **Not Using Precompiled Headers**.

---

## Menu

```
===== SMART PARKING SYSTEM =====
1. Park a vehicle
2. Unpark a vehicle
3. Show lot status
4. Look up a ticket
5. How this system works
0. Exit
```

---

## Sample Run

1. Choose **1**, enter plate `TN01AB1234`, type `2` (Car), time `9 30 AM`:

```
------- PARKING TICKET -------
Ticket ID : 1
Vehicle   : TN01AB1234 (Car)
Slot      : 1
Entry     : 09:30 AM
Status    : PARKED
------------------------------
```

2. Choose **2**, enter Ticket ID `1`, exit time `11 05 AM`:

```
------- PARKING TICKET -------
Ticket ID : 1
Vehicle   : TN01AB1234 (Car)
Slot      : 1
Entry     : 09:30 AM
Exit      : 11:05 AM
Fee       : Rs. 40
Status    : CLOSED
------------------------------
```

The stay was 95 minutes, which rounds up to 2 hours, so the fee is 2 x Rs. 20 = Rs. 40.

---

## Limitations (Mid-Sem Version)

- The lot has 10 slots and holds up to 100 tickets in total (fixed-size arrays)
- Data is not saved; everything is lost when the program closes
- Stays cannot cross midnight
- Errors are reported through return codes rather than exceptions

---

## Roadmap (Final Version)

- [ ] **Inheritance:** `Vehicle` becomes a base class with `Bike`, `Car` and `Truck` subclasses
- [ ] **Polymorphism:** virtual `getRatePerHour()` overridden per vehicle type
- [ ] **STL:** `vector` and `map` replace the fixed arrays
- [ ] **Exception handling:** `throw` and `try/catch` replace return codes
- [ ] **Stream handling:** save and load the lot using `ofstream` and `ifstream`
- [ ] **GUI:** Visual C++ front end replacing `main.cpp`

---

## Team

_Add team member names and roll numbers here._

## Course

Object-Oriented Programming (C++), mid-sem project submission.
