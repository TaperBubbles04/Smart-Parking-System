# Visual C++ GUI — Smart Parking Lot Management System

This folder contains the graphical desktop interface for the Smart Parking Lot Management System (`ParkingLot_Win32.cpp`).

---

## 1. Overview

The GUI connects directly to the core C++ backend (`ParkingSystem.h`). Because of object-oriented encapsulation and abstraction, the domain classes (`ParkingLot`, `Ticket`, `Slot`, `Vehicle`, `Time`) stay completely unchanged. The GUI simply replaces the console driver (`main.cpp`) with a graphical window containing input boxes and buttons.

---

## 2. What the GUI Contains

- **Live Slot Status Matrix:** Displays all 10 parking slots in a grid at the top. Free slots show in green (`Slot: FREE`) and occupied slots show in red (`Slot: OCCUPIED`), updating automatically on park or unpark.
- **Park Vehicle Section:** Input fields for vehicle license plate, vehicle type (1=Bike, 2=Car, 3=Truck), entry time (Hour, Minute, AM/PM), and a "PARK VEHICLE" button.
- **Unpark Vehicle Section:** Input fields for Ticket ID, exit time, and an "UNPARK & CALCULATE FEE" button.
- **Ticket Lookup:** Input a Ticket ID to look up its details anytime.
- **Output Screen:** A multi-line text box that displays printed tickets and fee receipts using the overloaded `<<` operator.
- **Message Popups:** Dialog alerts if the lot is full, vehicle is already parked, or an invalid ticket is entered.

---

## 3. How to Build and Run from Command Line (No Visual Studio IDE Needed)

### Method A: Using MSVC Command Prompt (`cl.exe`)
If you have Visual C++ build tools installed, open the **Developer Command Prompt for VS** (or **x64 Native Tools Command Prompt**):

```cmd
cd Visual_CPP
cl /EHsc ParkingLot_Win32.cpp ..\Code\Time.cpp ..\Code\Vehicle.cpp ..\Code\Slot.cpp ..\Code\Ticket.cpp ..\Code\ParkingLot.cpp user32.lib gdi32.lib /Fe:parking_gui.exe
parking_gui.exe
```

### Method B: Using MinGW GCC (`g++` on Windows)
If you are on Windows and compile with MinGW `g++` (in Command Prompt or PowerShell):

```cmd
cd Visual_CPP
g++ ParkingLot_Win32.cpp ../Code/Time.cpp ../Code/Vehicle.cpp ../Code/Slot.cpp ../Code/Ticket.cpp ../Code/ParkingLot.cpp -mwindows -static -o parking_gui.exe
parking_gui.exe
```

### Method C: Compiling on Linux (Using MinGW + Wine)
Because `<windows.h>` is a Windows API header, standard Linux `g++` does not have it. To compile and run this Windows GUI on Linux (Ubuntu):

1. Install the Windows cross-compiler and Wine:
   ```bash
   sudo apt install -y g++-mingw-w64-x86-64 wine
   ```
2. Compile with `-static` (so it bundles the required C++ DLLs) and run:
   ```bash
   cd Visual_CPP
   x86_64-w64-mingw32-g++ ParkingLot_Win32.cpp ../Code/Time.cpp ../Code/Vehicle.cpp ../Code/Slot.cpp ../Code/Ticket.cpp ../Code/ParkingLot.cpp -mwindows -static -o parking_gui.exe
   wine parking_gui.exe
   ```

---

## 4. How to Build and Run Using Visual Studio (IDE)

If you prefer using the Visual Studio IDE:

1. Open **Visual Studio** $\rightarrow$ **Create a new project** $\rightarrow$ select **Windows Desktop Application** (or **Desktop Application (C++)** / **Empty Project**). Name it `SmartParkingGUI`.
2. Add files to the project:
   - Under **Header Files**, add `Code/ParkingSystem.h`.
   - Under **Source Files**, add:
     - `Code/Time.cpp`
     - `Code/Vehicle.cpp`
     - `Code/Slot.cpp`
     - `Code/Ticket.cpp`
     - `Code/ParkingLot.cpp`
     - `Visual_CPP/ParkingLot_Win32.cpp`
   *(Do not add `Code/main.cpp` because `ParkingLot_Win32.cpp` has its own `WinMain` entry point)*.
3. In **Project** $\rightarrow$ **Properties** $\rightarrow$ **C/C++** $\rightarrow$ **Precompiled Headers**, set **Precompiled Header** to **Not Using Precompiled Headers**.
4. Press **Ctrl + F5** to build and run.

---

## 5. How the Code Works

The GUI is straightforward:
1. When the window opens (`WM_CREATE`), standard Windows labels, edit boxes, and buttons are created with `CreateWindowA`.
2. When a button is clicked (`WM_COMMAND`):
   - **Park:** Reads plate, type, and entry time from the text boxes, calls `lot.park()`, updates the slot grid, and prints the ticket to the output box.
   - **Unpark:** Reads ticket ID and exit time, calls `lot.unpark()`, updates the slot grid, and prints the receipt with the fee.
   - **Status Update:** The slot matrix updates automatically by querying `lot.isSlotFree()`, visually showing free and occupied bays.
3. All inputs and fee logic are handled by the same C++ classes used in the console version.
