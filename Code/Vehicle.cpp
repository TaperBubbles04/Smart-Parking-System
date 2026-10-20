#include "ParkingSystem.h"

Vehicle::Vehicle() {
    plate = "";
    type = 2;
}

Vehicle::Vehicle(std::string plateNumber, int vehicleType) {
    plate = plateNumber;
    type = vehicleType;
}

std::string Vehicle::getPlate() const {
    return plate;
}

int Vehicle::getType() const {
    return type;
}

std::string Vehicle::getTypeName() const {
    if (type == 1) return "Bike";
    if (type == 3) return "Truck";
    return "Car";
}

int Vehicle::getRatePerHour() const {
    if (type == 1) return 10;
    if (type == 3) return 40;
    return 20;
}