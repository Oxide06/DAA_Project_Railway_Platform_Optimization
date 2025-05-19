#ifndef CSVREADER_H
#define CSVREADER_H

#include <string>
#include "PlatformAllocator.h"

class CSVReader {
public:
    void readCSV(const std::string& filename, TrainScheduler& scheduler);
};

#endif
