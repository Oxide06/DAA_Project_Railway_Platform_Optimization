#pragma once
#ifndef TRAINSCHEDULER_H
#define TRAINSCHEDULER_H

#include "Train.h"
#include <vector>
#include <string>

class TrainScheduler {
private:
    std::vector<Train> trains; // List of all train objects
    std::vector<std::pair<std::string, int>> assignments; // Stores train ID and assigned platform
    int platformCount = 0; // Total platforms used

public:
    TrainScheduler() = default;
    TrainScheduler(const std::vector<Train>& trains); // Constructor for bulk load

    void addTrain(const Train& train); // Add single train
    void scheduleTrains();             // Schedule using greedy algorithm
    void printSchedule() const;        // Output the result
    int getPlatformCount() const;      // Getter for number of platforms used
    const std::vector<std::pair<std::string, int>>& getAssignments() const; // Access results
};

#endif
