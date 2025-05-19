#include "PlatformAllocator.h"
#include "MinHeap.h"
#include <iostream>
#include <algorithm>
const int MAX_PLATFORMS = 7;


TrainScheduler::TrainScheduler(const std::vector<Train>& trains) : trains(trains), platformCount(0) {}

void TrainScheduler::addTrain(const Train& train) {
    trains.push_back(train);
}

void TrainScheduler::scheduleTrains() {
    // Sort by arrival time
    std::sort(trains.begin(), trains.end(), [](const Train& a, const Train& b) {
        return a.getArrival() < b.getArrival();
    });

    MinHeap heap;
    int nextPlatform = 1;

    for (const auto& train : trains) {
        int arrival = train.getArrival();
        int departure = train.getDeparture();

        if (!heap.isEmpty() && heap.peek().first <= arrival) {
            // Reuse platform
            auto reused = heap.extractMin();
            heap.insert({departure, reused.second});
            assignments.push_back({train.getID(), reused.second});
        }  else if (nextPlatform <= MAX_PLATFORMS) {
            heap.insert({departure, nextPlatform});
            assignments.push_back({train.getID(), nextPlatform});
            nextPlatform++;
        }
       
        else {
            std::cerr << "No platform available for train " << train.getID() << "\n";
        }
    }

    platformCount = nextPlatform - 1;
}

void TrainScheduler::printSchedule() const {
    std::cout << "Train Schedule:\n";
    for (const auto& entry : assignments) {
        std::cout << "Train " << entry.first << " assigned to Platform " << entry.second << "\n";
    }
}

int TrainScheduler::getPlatformCount() const {
    return platformCount;
}

const std::vector<std::pair<std::string, int>>& TrainScheduler::getAssignments() const {
    return assignments;
}
