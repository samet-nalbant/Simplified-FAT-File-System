#ifndef DATE_TIME_H
#define DATE_TIME_H


#include <iostream>
#include <chrono>
#include <ctime>
#include <string>

using namespace std;

typedef struct dt{
    unsigned short int year;
    unsigned short int month;
    unsigned short int day;
    unsigned short int hour;
    unsigned short int minute;
    unsigned short int second;
    string toString() {
        tm lastModificationTm = { 0 };
        lastModificationTm.tm_year = year - 1900;
        lastModificationTm.tm_mon = month - 1;
        lastModificationTm.tm_mday = day;
        lastModificationTm.tm_hour = hour;
        lastModificationTm.tm_min = minute;
        lastModificationTm.tm_sec = second;
        time_t lastModificationTimestamp = mktime(&lastModificationTm);
        char buffer[100];
        strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", localtime(&lastModificationTimestamp));
        return string(buffer);
    }


} DateTime;


DateTime getCurrentDateTime();
#endif