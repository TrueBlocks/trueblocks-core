#pragma once

#include "utillib.h"
#include <unordered_map>
using std::setw;
namespace qblocks {
    struct loc_date_t {
        int year = 0;
        int month = 0;
        int day = 0;
    };
    inline ostream& operator<<(ostream& os, const loc_date_t& dt) {
        os.fill('0');
        os << setw(4) << dt.year << "-" << setw(2) << dt.month << "-" << setw(2) << dt.day;
        return os;
    }

    struct loc_time_t {
        int hour = 0;
        int minute = 0;
        int second = 0;
        int microsecond = 0;
    };
    inline ostream& operator<<(ostream& os, const loc_time_t& ltime) {
        os.fill('0');
        os << setw(2) << ltime.hour << ":" << setw(2) << ltime.minute << ":" << setw(2) << ltime.second;
        if (ltime.microsecond > 0) {
            os << ".";
            int power = 100000;
            for (int curr_us = ltime.microsecond; curr_us; power /= 10) {
                auto num = curr_us / power;
                os << num;
                curr_us -= num * power;
            }
        }
        return os;
    }

    struct zone_off_t {
        int hour_offset = 0;
        int minute_offset = 0;
    };
    inline ostream& operator<<(ostream& os, const zone_off_t& zo)
    {
        os.fill('0');
        if (zo.hour_offset != 0 || zo.minute_offset != 0) {
            if (zo.hour_offset > 0) {
                os << "+";
            } else {
                os << "-";
            }
            os << setw(2) << abs(zo.hour_offset) << ":" << setw(2)
            << abs(zo.minute_offset);
        } else {
            os << "Z";
        }
        return os;
    }

    struct loc_datetime_t : loc_date_t, loc_time_t {
    };
    inline ostream& operator<<(ostream& os, const loc_datetime_t& dt) {
        return os << static_cast<const loc_date_t&>(dt) << "T" << static_cast<const loc_time_t&>(dt);
    }

    struct off_datetime_t : loc_datetime_t, zone_off_t {
    };
    inline ostream& operator<<(ostream& os, const off_datetime_t& dt) {
        return os << static_cast<const loc_datetime_t&>(dt)
        << static_cast<const zone_off_t&>(dt);
    }

    template <class T>
    class option {
    public:
        option() : empty_{true} { }
        option(T value) : empty_{false}, value_(move(value)) { }
        explicit operator bool() const { return !empty_; }
        const T& operator*() const { return value_; }
        const T* operator->() const { return &value_; }
    private:
        bool empty_;
        T value_;
    };
};
