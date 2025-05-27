#pragma once

#include <string>

class Train {
private:
    std::string id;
    int arrival;
    int departure;

public:
    Train(const std::string& id_, const std::string& arrTime, const std::string& depTime);

    int getArrival() const;
    int getDeparture() const;
    std::string getID() const;

    static int convertToMinutes(const std::string& timeStr);
};

