#include "CSVWriter.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// 4MB buffer limit to prevent RAM spikes during large file evaluations
constexpr size_t MAX_BUFFER_SIZE = 4 * 1024 * 1024;

CSVWriter::CSVWriter(const std::string &filename)
{
    outFile_.open(filename);
    if (!outFile_.is_open())
    {
        throw std::runtime_error("Could not open output file: " + filename);
    }

    // Write the headers immediately to the buffer
    buffer_ = "Order ID,Client Order ID,Instrument,Side,Exec Status,Quantity,Price,Reason,Transaction Time\n";

    // Pre-allocate memory to prevent the string from resizing constantly
    buffer_.reserve(MAX_BUFFER_SIZE + 1024);
}

CSVWriter::~CSVWriter()
{
    // Flush whatever is left in the buffer when the program finishes
    if (!buffer_.empty())
    {
        outFile_ << buffer_;
    }
}

std::string CSVWriter::generateTimestamp() const
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::time_t t = system_clock::to_time_t(now);
    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &t); // Safe version for Windows
#else
    localtime_r(&t, &tm_buf); // Safe version for Linux/Mac
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y%m%d-%H%M%S")
        << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void CSVWriter::write(const ExecutionReport &report)
{
    // Fallbacks for malformed rows (handling Example 7 rejections)
    const std::string sideText = report.sideText.has_value() ? *report.sideText : std::to_string(report.side);
    const std::string quantityText = report.quantityText.has_value() ? *report.quantityText : std::to_string(report.quantity);

    std::ostringstream priceStream;
    if (report.priceText.has_value())
    {
        priceStream << *report.priceText;
    }
    else
    {
        priceStream << std::fixed << std::setprecision(2) << report.price;
    }

    // Build the row string
    std::ostringstream row;
    row << report.orderID << ','
        << report.clientOrderID << ','
        << report.instrument << ','
        << sideText << ','
        << report.status << ','
        << quantityText << ','
        << priceStream.str() << ','
        << report.reason << ','
        << generateTimestamp() << '\n';

    buffer_ += row.str();

    // The optimization: If the buffer gets too big, dump it to the disk and clear it
    if (buffer_.size() >= MAX_BUFFER_SIZE)
    {
        outFile_ << buffer_;
        buffer_.clear();
    }
}