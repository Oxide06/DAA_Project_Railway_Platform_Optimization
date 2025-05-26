#include "../include/PlatformAllocator.h"
#include "../include/MinHeap.h"
#include <iostream>
#include <algorithm>
const int MAX_PLATFORMS = 5;

TrainScheduler::TrainScheduler(const std::vector<Train>& trains) 
    : trains(trains), platformCount(0) {}

void TrainScheduler::addTrain(const Train& train) {
    trains.push_back(train);
}

void TrainScheduler::scheduleTrains() {
    std::sort(trains.begin(), trains.end(), [](const Train& a, const Train& b) {
        return a.getArrival() < b.getArrival();
    });

    MinHeap heap;
    int nextPlatform = 1;

    for (const auto& train : trains) {
        int arrival = train.getArrival();
        int departure = train.getDeparture();

        if (!heap.isEmpty() && heap.peek().first <= arrival) {
            auto reused = heap.extractMin();
            heap.insert({departure, reused.second});
            assignments.push_back({train.getID(), reused.second, arrival, departure});
        } else if (nextPlatform <= MAX_PLATFORMS) {
            heap.insert({departure, nextPlatform});
            assignments.push_back({train.getID(), nextPlatform, arrival, departure});
            nextPlatform++;
        } else {
            std::cerr << "No platform available for train " << train.getID() << "\n";
        }
    }

    platformCount = nextPlatform - 1;
}

void TrainScheduler::printSchedule() const {
    std::cout << "Train Schedule:\n";
    for (const auto& entry : assignments) {
        std::cout << "Train " << entry.trainID 
                  << " at Platform " << entry.platform
                  << " (" << entry.arrival/60 << ":" << entry.arrival%60
                  << " - " << entry.departure/60 << ":" << entry.departure%60 << ")\n";
    }
}

int TrainScheduler::getPlatformCount() const {
    return platformCount;
}

const std::vector<Assignment>& TrainScheduler::getAssignments() const {
    return assignments;
}
