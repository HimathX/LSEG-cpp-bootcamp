#include "CSVWriter.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

CSVWriter::CSVWriter(const std::string &filename)
{
    outFile_.open(filename);
    if (!outFile_.is_open())
    {
        throw std::runtime_error("Could not open output file: " + filename);
    }
    buffer_ = "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";
}

CSVWriter::~CSVWriter()
{
    outFile_ << buffer_; // single buffered write — no per-row flushes
}

std::string CSVWriter::generateTimestamp() const
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &t);
#else
    localtime_r(&t, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%d-%H%M%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void CSVWriter::write(const ExecutionReport &report)
{
    std::ostringstream row;
    row << report.orderID << ','
        << report.clientOrderID << ','
        << report.instrument << ','
        << report.side << ','
        << report.status << ','
        << report.quantity << ','
        << std::fixed << std::setprecision(2) << report.price << ','
        << report.reason << ','
        << generateTimestamp() << '\n';
    buffer_ += row.str();
}
