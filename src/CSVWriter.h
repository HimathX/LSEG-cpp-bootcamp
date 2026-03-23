#pragma once
#include <fstream>
#include <string>
#include "ExecutionReport.h"

class BufferedCSVWriter
{
public:
    BufferedCSVWriter(const std::string &filename, const std::string &header);
    ~BufferedCSVWriter(); // flushes accumulated buffer to disk

private:
    std::ofstream outFile_;
    std::string buffer_;

protected:
    void appendRow(const std::string &row);
    std::string generateTimestamp() const;
};

class ExecutionReportCSVWriter : private BufferedCSVWriter
{
public:
    explicit ExecutionReportCSVWriter(const std::string &filename);

    void write(const ExecutionReport &report);
};

class RejectedExecutionReportCSVWriter:private BufferedCSVWriter
{
public:
    explicit RejectedExecutionReportCSVWriter(const std::string &filename);

    void write(const RejectedExecutionReport &report);
};
