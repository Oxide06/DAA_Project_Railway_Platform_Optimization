#include "CSVReader.h"
#include "Train.h"
#include <fstream>
#include <sstream>
#include <iostream>

void CSVReader::readCSV(const std::string& filename, TrainScheduler& scheduler) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, arrival, departure;

        // Read each comma-separated field
        std::getline(ss, id, ',');
        std::getline(ss, arrival, ',');
        std::getline(ss, departure, ',');

        // Create Train object using string constructor
        Train t(id, arrival, departure);

        // Add to scheduler
        scheduler.addTrain(t);
    }

    file.close();
}
