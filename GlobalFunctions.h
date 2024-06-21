#pragma once
#include <ctime>
#include "Optional.hpp"

class GlobalFunctions
{
public:
    static time_t toTimeT(const std::tm& tm);
    static bool compareDates(const Optional<std::tm>& optionalDate, const std::tm& date);
};