#pragma once

#include <string>
#include "PlatformAllocator.h"

class CSVReader {
public:
    void readCSV(const std::string& filename, TrainScheduler& scheduler);
};

