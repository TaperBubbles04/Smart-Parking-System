#include <windows.h>
#include <string>
#include <sstream>

#if __has_include("ParkingSystem.h")
#include "ParkingSystem.h"
#else
#include "../Code/ParkingSystem.h"
#endif

// Global parking lot object
ParkingLot lot;

// Control IDs
#define ID_BTN_PARK    101
#define ID_BTN_UNPARK  102
#define ID_BTN_LOOKUP  103
#define ID_BTN_CLEAR   104

// Input controls
HWND hPlate, hType, hEntryH, hEntryM, hEntryAP;
HWND hTicketId, hExitH, hExitM, hExitAP;
HWND hLookupId, hOutput;
HWND hSlots[MAX_SLOTS];

// Brushes and Fonts for visual appearance
HBRUSH hBgBrush = NULL;
HBRUSH hBrushFree = NULL;
HBRUSH hBrushBusy = NULL;
HFONT hFont = NULL;
HFONT hFontBold = NULL;
HFONT hFontTitle = NULL;
HFONT hFontMono = NULL;

// Helper to convert GUI time inputs into a Time object
Time getTime(HWND hH, HWND hM, HWND hAP) {
    char bH[10], bM[10], bAP[10];
    GetWindowTextA(hH, bH, 10);
    GetWindowTextA(hM, bM, 10);
    GetWindowTextA(hAP, bAP, 10);

    int h = atoi(bH);
    int m = atoi(bM);
    std::string ap = bAP;

    if (ap == "PM" && h != 12) h += 12;
    if (ap == "AM" && h == 12) h = 0;

    return Time(h, m);
}

// Update the 10 visual slot bays
void updateSlots(HWND hWnd) {
    for (int i = 0; i < lot.getTotalSlots(); i++) {
        std::string txt = "Slot " + std::to_string(i + 1) + ": ";
        txt += lot.isSlotFree(i + 1) ? "FREE" : "OCCUPIED";
        SetWindowTextA(hSlots[i], txt.c_str());
    }
    InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // Create clean modern fonts
        hFont = CreateFontA(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, DEFAULT_QUALITY, 0, "Segoe UI");
        hFontBold = CreateFontA(15, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, DEFAULT_QUALITY, 0, "Segoe UI");
        hFontTitle = CreateFontA(19, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, DEFAULT_QUALITY, 0, "Segoe UI");
        hFontMono = CreateFontA(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, DEFAULT_QUALITY, 0, "Consolas");

        // Create soft background and slot status colors
        hBgBrush = CreateSolidBrush(RGB(245, 247, 250));
        hBrushFree = CreateSolidBrush(RGB(220, 252, 231));   // Soft Green
        hBrushBusy = CreateSolidBrush(RGB(254, 226, 226));   // Soft Red

        // Title Header
        HWND hTitle = CreateWindowA("STATIC", "SMART PARKING LOT MANAGEMENT SYSTEM",
            WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 12, 730, 25, hWnd, NULL, NULL, NULL);
        SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

        // Section 1: Live Slots Grid (2 rows of 5)
        HWND grpSlots = CreateWindowA("BUTTON", "Parking Bays Status (10 Slots)",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 42, 730, 98, hWnd, NULL, NULL, NULL);
        SendMessage(grpSlots, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        for (int i = 0; i < MAX_SLOTS; i++) {
            int x = (i < 5) ? (35 + i * 140) : (35 + (i - 5) * 140);
            int y = (i < 5) ? 68 : 100;
            hSlots[i] = CreateWindowA("STATIC", "",
                WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER,
                x, y, 130, 24, hWnd, NULL, NULL, NULL);
            SendMessage(hSlots[i], WM_SETFONT, (WPARAM)hFontBold, TRUE);
        }

        // Section 2: Park Vehicle Panel
        HWND grpPark = CreateWindowA("BUTTON", "1. Park Vehicle (Intake)",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 150, 355, 215, hWnd, NULL, NULL, NULL);
        SendMessage(grpPark, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        HWND lbl1 = CreateWindowA("STATIC", "Plate Number:", WS_VISIBLE | WS_CHILD, 35, 180, 95, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl1, WM_SETFONT, (WPARAM)hFont, TRUE);
        hPlate = CreateWindowA("EDIT", "TN01AB1234", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 135, 178, 120, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hPlate, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND lbl2 = CreateWindowA("STATIC", "Vehicle Type:", WS_VISIBLE | WS_CHILD, 35, 215, 95, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl2, WM_SETFONT, (WPARAM)hFont, TRUE);
        hType = CreateWindowA("EDIT", "2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 135, 213, 30, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hType, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND lbl2Hint = CreateWindowA("STATIC", "(1=Bike, 2=Car, 3=Truck)", WS_VISIBLE | WS_CHILD, 172, 215, 190, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl2Hint, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND lbl3 = CreateWindowA("STATIC", "Entry Time:", WS_VISIBLE | WS_CHILD, 35, 250, 95, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl3, WM_SETFONT, (WPARAM)hFont, TRUE);
        hEntryH = CreateWindowA("EDIT", "09", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 135, 248, 30, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hEntryH, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND c1 = CreateWindowA("STATIC", ":", WS_VISIBLE | WS_CHILD | SS_CENTER, 168, 250, 8, 20, hWnd, NULL, NULL, NULL);
        SendMessage(c1, WM_SETFONT, (WPARAM)hFont, TRUE);
        hEntryM = CreateWindowA("EDIT", "30", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 180, 248, 30, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hEntryM, WM_SETFONT, (WPARAM)hFont, TRUE);
        hEntryAP = CreateWindowA("EDIT", "AM", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, 218, 248, 36, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hEntryAP, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND btnPark = CreateWindowA("BUTTON", "PARK VEHICLE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            135, 295, 140, 36, hWnd, (HMENU)ID_BTN_PARK, NULL, NULL);
        SendMessage(btnPark, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        // Section 3: Unpark Vehicle Panel
        HWND grpUnpark = CreateWindowA("BUTTON", "2. Unpark Vehicle (Exit & Billing)",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 395, 150, 355, 215, hWnd, NULL, NULL, NULL);
        SendMessage(grpUnpark, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        HWND lbl4 = CreateWindowA("STATIC", "Ticket ID:", WS_VISIBLE | WS_CHILD, 410, 180, 90, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl4, WM_SETFONT, (WPARAM)hFont, TRUE);
        hTicketId = CreateWindowA("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 505, 178, 60, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hTicketId, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND lbl5 = CreateWindowA("STATIC", "Exit Time:", WS_VISIBLE | WS_CHILD, 410, 230, 90, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl5, WM_SETFONT, (WPARAM)hFont, TRUE);
        hExitH = CreateWindowA("EDIT", "11", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 505, 228, 30, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hExitH, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND c2 = CreateWindowA("STATIC", ":", WS_VISIBLE | WS_CHILD | SS_CENTER, 538, 230, 8, 20, hWnd, NULL, NULL, NULL);
        SendMessage(c2, WM_SETFONT, (WPARAM)hFont, TRUE);
        hExitM = CreateWindowA("EDIT", "05", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 550, 228, 30, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hExitM, WM_SETFONT, (WPARAM)hFont, TRUE);
        hExitAP = CreateWindowA("EDIT", "AM", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER, 588, 228, 36, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hExitAP, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND btnUnpark = CreateWindowA("BUTTON", "UNPARK & CALCULATE FEE", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            505, 295, 210, 36, hWnd, (HMENU)ID_BTN_UNPARK, NULL, NULL);
        SendMessage(btnUnpark, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        // Section 4: Output and Details
        HWND grpOutput = CreateWindowA("BUTTON", "3. Ticket Receipt & System Output",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 20, 380, 730, 230, hWnd, NULL, NULL, NULL);
        SendMessage(grpOutput, WM_SETFONT, (WPARAM)hFontBold, TRUE);

        hOutput = CreateWindowA("EDIT", "System Ready.\r\nEnter details above to Park or Unpark.",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
            35, 410, 520, 185, hWnd, NULL, NULL, NULL);
        SendMessage(hOutput, WM_SETFONT, (WPARAM)hFontMono, TRUE);

        HWND lbl6 = CreateWindowA("STATIC", "Ticket Lookup:", WS_VISIBLE | WS_CHILD, 575, 415, 120, 20, hWnd, NULL, NULL, NULL);
        SendMessage(lbl6, WM_SETFONT, (WPARAM)hFont, TRUE);
        hLookupId = CreateWindowA("EDIT", "1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 575, 440, 45, 24, hWnd, NULL, NULL, NULL);
        SendMessage(hLookupId, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND btnLookup = CreateWindowA("BUTTON", "Lookup", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 628, 438, 90, 28, hWnd, (HMENU)ID_BTN_LOOKUP, NULL, NULL);
        SendMessage(btnLookup, WM_SETFONT, (WPARAM)hFont, TRUE);

        HWND btnClear = CreateWindowA("BUTTON", "Clear Output", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 575, 495, 143, 32, hWnd, (HMENU)ID_BTN_CLEAR, NULL, NULL);
        SendMessage(btnClear, WM_SETFONT, (WPARAM)hFont, TRUE);

        updateSlots(hWnd);
        break;
    }

    case WM_CTLCOLORDLG:
        return (INT_PTR)hBgBrush;

    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;

        for (int i = 0; i < lot.getTotalSlots(); i++) {
            if (hCtrl == hSlots[i]) {
                if (lot.isSlotFree(i + 1)) {
                    SetTextColor(hdc, RGB(22, 101, 52));
                    SetBkColor(hdc, RGB(220, 252, 231));
                    return (INT_PTR)hBrushFree;
                } else {
                    SetTextColor(hdc, RGB(153, 27, 27));
                    SetBkColor(hdc, RGB(254, 226, 226));
                    return (INT_PTR)hBrushBusy;
                }
            }
        }
        SetBkColor(hdc, RGB(245, 247, 250));
        return (INT_PTR)hBgBrush;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);

        if (id == ID_BTN_PARK) {
            char plate[30], typeStr[10];
            GetWindowTextA(hPlate, plate, 30);
            GetWindowTextA(hType, typeStr, 10);
            int type = atoi(typeStr);

            if (strlen(plate) == 0) {
                MessageBoxA(hWnd, "Please enter a plate number!", "Input Error", MB_OK);
                break;
            }

            Time entry = getTime(hEntryH, hEntryM, hEntryAP);
            int ticketId = lot.park(Vehicle(plate, type), entry);

            if (ticketId == 0) {
                MessageBoxA(hWnd, "Sorry, the parking lot is full!", "Lot Full", MB_OK);
            } else if (ticketId == -1) {
                MessageBoxA(hWnd, "This vehicle is already parked!", "Duplicate Vehicle", MB_OK);
            } else {
                std::ostringstream ss;
                ss << lot.getTicket(ticketId); // Uses overloaded operator<<
                SetWindowTextA(hOutput, ss.str().c_str());
                updateSlots(hWnd);
                MessageBoxA(hWnd, "Vehicle Parked Successfully!", "Success", MB_OK);
            }
        }
        else if (id == ID_BTN_UNPARK) {
            char idStr[10];
            GetWindowTextA(hTicketId, idStr, 10);
            int ticketId = atoi(idStr);

            Time exit = getTime(hExitH, hExitM, hExitAP);
            int fee = lot.unpark(ticketId, exit);

            if (fee == -1) {
                MessageBoxA(hWnd, "Invalid or already closed ticket!", "Error", MB_OK);
            } else if (fee == -2) {
                MessageBoxA(hWnd, "Exit time cannot be before entry time!", "Chronology Error", MB_OK);
            } else {
                std::ostringstream ss;
                ss << lot.getTicket(ticketId); // Shows closed ticket
                SetWindowTextA(hOutput, ss.str().c_str());
                updateSlots(hWnd);
                std::string msg = "Vehicle unparked! Fee: Rs. " + std::to_string(fee);
                MessageBoxA(hWnd, msg.c_str(), "Receipt", MB_OK);
            }
        }
        else if (id == ID_BTN_LOOKUP) {
            char idStr[10];
            GetWindowTextA(hLookupId, idStr, 10);
            int ticketId = atoi(idStr);

            if (lot.isValidTicket(ticketId)) {
                std::ostringstream ss;
                ss << lot.getTicket(ticketId);
                SetWindowTextA(hOutput, ss.str().c_str());
            } else {
                MessageBoxA(hWnd, "Ticket not found!", "Lookup Error", MB_OK);
            }
        }
        else if (id == ID_BTN_CLEAR) {
            SetWindowTextA(hOutput, "Output cleared.\r\nReady for new actions...");
        }
        break;
    }

    case WM_DESTROY:
        DeleteObject(hBgBrush);
        DeleteObject(hBrushFree);
        DeleteObject(hBrushBusy);
        DeleteObject(hFont);
        DeleteObject(hFontBold);
        DeleteObject(hFontTitle);
        DeleteObject(hFontMono);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SmartParkingGUIClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(245, 247, 250));

    RegisterClassA(&wc);

    HWND hWnd = CreateWindowA(
        wc.lpszClassName,
        "Smart Parking Lot Management System - GUI",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 785, 660,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
