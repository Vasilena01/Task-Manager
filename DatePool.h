#pragma once
#include <iostream>
#include <ctime>
#include "Vector.hpp"
#include "MyString.h"

// Using the singleton pattern here
class DatePool
{
public:
    static DatePool& getInstance();

	std::tm getOrAddDate(const MyString& dateStr);

    DatePool(const DatePool& other) = delete;
    DatePool& operator=(const DatePool& other) = delete;
private:
    DatePool() = default;

    struct DateEntry {
        MyString dateStr;
        std::tm date;
    };

    Vector<DateEntry> dates_;

    std::tm parseDate(const MyString& dateStr);
};

