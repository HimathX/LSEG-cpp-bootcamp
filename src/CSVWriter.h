#pragma once
#include <fstream>
#include <string>
#include "ExecutionReport.h"

class CSVWriter
{
public:
    explicit CSVWriter(const std::string &filename);
    ~CSVWriter(); // flushes accumulated buffer to disk

    void write(const ExecutionReport &report);

private:
    std::ofstream outFile_;
    std::string buffer_;

    std::string generateTimestamp() const;
};
