#include "GlobalFunctions.h"

time_t GlobalFunctions::toTimeT(const std::tm& tm) {
    std::tm temp = tm;
    return std::mktime(&temp);
}

bool GlobalFunctions::compareDates(const Optional<std::tm>& optionalDate, const std::tm& date) {
    if (!optionalDate.isFilled()) {
        return false;
    }
    time_t time1 = toTimeT(optionalDate.getValue());
    time_t time2 = toTimeT(date);
    return time1 == time2;
}