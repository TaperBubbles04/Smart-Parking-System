#include "ParkingSystem.h"
#include <iomanip>

Time::Time() {
    hour = 0;
    minute = 0;
}

Time::Time(int h, int m) {
    hour = h;
    minute = m;
}

int Time::getHour() const {
    return hour;
}

int Time::getMinute() const {
    return minute;
}

int Time::operator-(const Time& other) const {
    int thisMinutes  = hour * 60 + minute;
    int otherMinutes = other.hour * 60 + other.minute;
    return thisMinutes - otherMinutes;
}

std::ostream& operator<<(std::ostream& out, const Time& t) {
    int h = t.getHour() % 12;
    if (h == 0) {
        h = 12; // 0 -> 12 AM, 12 -> 12 PM
    }
    const char* period = (t.getHour() < 12) ? "AM" : "PM";
    out << std::setw(2) << std::setfill('0') << h << ":"
        << std::setw(2) << std::setfill('0') << t.getMinute() << " " << period;
    out << std::setfill(' ');   // reset fill so other output is unaffected
    return out;
}