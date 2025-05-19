#include "Train.h"
#include <string>
#include <stdexcept>

Train::Train(const std::string& id_, const std::string& arrTime, const std::string& depTime)
    : id(id_), arrival(convertToMinutes(arrTime)), departure(convertToMinutes(depTime)) {}

int Train::getArrival() const { return arrival; }
int Train::getDeparture() const { return departure; }
std::string Train::getID() const { return id; }

int Train::convertToMinutes(const std::string& timeStr) {
    // Validate format: should be at least 5 characters and contain a colon
    if (timeStr.length() < 5 || timeStr[2] != ':') {
        throw std::invalid_argument("Invalid time format: " + timeStr);
    }

    try {
        int hour = std::stoi(timeStr.substr(0, 2));
        int minute = std::stoi(timeStr.substr(3, 2));

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            throw std::out_of_range("Hour or minute out of valid range in: " + timeStr);
        }

        return hour * 60 + minute;
    } catch (const std::exception& e) {
        throw std::invalid_argument("Failed to convert time: " + timeStr + " (" + e.what() + ")");
    }
}
