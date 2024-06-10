#include "DatePool.h"

DatePool& DatePool::getInstance()
{
    static DatePool instance;
    return instance;
}

std::tm DatePool::getOrAddDate(const MyString& dateStr) {
    for (int i = 0; i < dates_.getSize(); i++) {
        if (dates_[i].dateStr == dateStr) {
            return dates_[i].date;
        }
    }

    std::tm newDate = parseDate(dateStr);
    dates_.pushBack({ dateStr, newDate });
    return newDate;
}

std::tm DatePool::parseDate(const MyString& dateStr) {
    const char* cstr = dateStr.c_str();
    std::tm tm = {};
    std::memset(&tm, 0, sizeof(tm)); // Ensuring tm is zeroed out

    // Parse the date manually
    int year = 0, month = 0, day = 0;
    size_t i = 0;
    while (cstr[i] != '-' && cstr[i] != '\0') {
        year = year * 10 + (cstr[i] - '0');
        i++;
    }
    if (cstr[i] == '\0' || cstr[++i] == '\0') 
        throw std::invalid_argument("Invalid date format");

    while (cstr[i] != '-' && cstr[i] != '\0') {
        month = month * 10 + (cstr[i] - '0');
        i++;
    }
    if (cstr[i] == '\0' || cstr[++i] == '\0') 
        throw std::invalid_argument("Invalid date format");

    while (cstr[i] != '\0') {
        day = day * 10 + (cstr[i] - '0');
        i++;
    }

    tm.tm_year = year - 1900; // tm_year is years since 1900
    tm.tm_mon = month - 1;    // tm_mon is months since January (0-11)
    tm.tm_mday = day;

    return tm;
}