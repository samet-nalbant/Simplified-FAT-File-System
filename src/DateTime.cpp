#include "../include/DateTime.h"

DateTime getCurrentDateTime() {
    auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    tm* localTime = localtime(&currentTime);
    DateTime dateTime;
    dateTime.year = localTime->tm_year + 1900;
    dateTime.month = localTime->tm_mon + 1;
    dateTime.day = localTime->tm_mday;
    dateTime.hour = localTime->tm_hour;
    dateTime.minute = localTime->tm_min;
    dateTime.second = localTime->tm_sec;
    return dateTime;
}