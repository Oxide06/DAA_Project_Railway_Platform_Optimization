#include <iostream>
#include "PlatformAllocator.h"
#include "CSVReader.h"

int main() {
    TrainScheduler scheduler;
    CSVReader reader;

    std::string filename = "trains.csv";
    reader.readCSV(filename, scheduler);

    scheduler.scheduleTrains();
    scheduler.printSchedule();

    return 0;
}