#pragma once
#ifndef TRAINSCHEDULER_H
#define TRAINSCHEDULER_H

#include "Train.h"
#include <vector>
#include <string>

struct Assignment {
    std::string trainID;
    int platform;
    int arrival;
    int departure;
};

class TrainScheduler {
private:
    std::vector<Train> trains; 
    std::vector<Assignment> assignments; 
    int platformCount = 0; 

public:
    TrainScheduler() = default;
    TrainScheduler(const std::vector<Train>& trains); 

    void addTrain(const Train& train); 
    void scheduleTrains();             
    void printSchedule() const;        
    int getPlatformCount() const;      
    const std::vector<Assignment>& getAssignments() const;
};

#endif
