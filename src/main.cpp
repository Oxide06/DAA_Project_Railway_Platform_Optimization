#include <iostream>
#include "../include/PlatformAllocator.h"
#include "../include/CSVReader.h"
#include "../include/SimulationUI.h"
int main() {
    TrainScheduler scheduler;
    CSVReader reader;

    std::string filename = "../trains.csv";
    reader.readCSV(filename, scheduler);

    scheduler.scheduleTrains();
    //scheduler.printSchedule();
    
    SimulationUI ui(scheduler);
    ui.run();
    return 0;
}