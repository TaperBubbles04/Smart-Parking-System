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
    out << std::setw(2) << std::setfill('0') << t.getHour() << ":"
        << std::setw(2) << std::setfill('0') << t.getMinute();
    out << std::setfill(' ');   // reset fill so other output is unaffected
    return out;
}