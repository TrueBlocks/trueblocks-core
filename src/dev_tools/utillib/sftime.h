#pragma once
#include "sfos.h"

namespace qblocks {

class time_q;
struct CDateStruct {
    uint32_t m_Year;
    uint32_t m_Month;
    uint32_t m_Day;
    uint32_t m_Hour;
    uint32_t m_Minute;
    uint32_t m_Second;
};

class time_q {
  private:
    class CDate {
        uint64_t m_nDays;

        CDate(void);
        CDate(const CDate& dt);
        CDate(uint32_t year, uint32_t month, uint32_t day);
        CDate(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek);
        CDate(const string_q& dateStr, const string_q& fmtStr);

        explicit CDate(int64_t days);
        explicit CDate(const tm& sysTime);

        CDate& operator=(const CDate& date);

        uint32_t GetYear(void) const;
        uint32_t GetMonth(void) const;
        uint32_t GetDay(void) const;

        int64_t GetTotalDays(void) const;

        CDate operator+(int32_t days) const;
        CDate operator-(int32_t days) const;

        CDate& operator+=(int32_t days);
        CDate& operator-=(int32_t days);

        CDate& setValues(uint32_t y, uint32_t m, uint32_t d);
        CDateStruct getDateStruct(void) const;

        friend class time_q;
        friend uint32_t getWeekOfMonth(const CDate& date);
        friend uint32_t getDayOfWeek(const CDate& date);
    };

    class CTimeOfDay {
      public:
        CTimeOfDay(void);
        CTimeOfDay(const CTimeOfDay& tod);
        CTimeOfDay(uint32_t h, uint32_t m, uint32_t s);
        CTimeOfDay(const string_q& dateStr, const string_q& fmtStr);

        explicit CTimeOfDay(const tm& sysTime);
        explicit CTimeOfDay(uint32_t secs);

        uint32_t GetHour(void) const;
        uint32_t GetMinute(void) const;
        uint32_t GetSecond(void) const;
        uint32_t GetTotalSeconds(void) const;

        uint32_t m_nSeconds;
    };

  public:
    time_q(void);
    time_q(const time_q& date);

    time_q(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t sec);
    time_q(uint32_t year, uint32_t month, uint32_t weekInMonth, uint32_t dayOfWeek, uint32_t hour, uint32_t minute,
           uint32_t sec);
    time_q(uint32_t days, uint32_t hour, uint32_t minute, uint32_t sec);
    time_q(const CDate& date, const CTimeOfDay& tod);
    time_q(const string_q& dateStr, const string_q& fmtStr);

    explicit time_q(const tm& sysTime, bool useDayOfWeek = false);

    time_q& operator=(const time_q& date);

    bool IsValid(void) const;

    time_q operator+(const uint32_t& ts) const;
    time_q operator-(const uint32_t& ts) const;
    int64_t operator-(time_q& date);

    bool operator==(const time_q& date) const;
    bool operator!=(const time_q& date) const;

    bool operator>(const time_q& date) const;
    bool operator<(const time_q& date) const;

    bool operator>=(const time_q& date) const;
    bool operator<=(const time_q& date) const;

    uint32_t GetDay(void) const;
    uint32_t GetMonth(void) const;
    uint32_t GetYear(void) const;
    uint32_t GetHour(void) const;
    uint32_t GetMinute(void) const;
    uint32_t GetSecond(void) const;
    uint32_t GetDayOfWeek(void) const;
    int64_t GetTotalSeconds(void) const;
    CDate getDatePart(void) const;
    CTimeOfDay getTimePart(void) const;
    int64_t m_nSeconds;
    friend uint32_t getDayOfWeek(const CDate& date);
};

extern time_q Now(void);
extern const time_q latestDate;
extern const time_q earliestDate;

extern time_q fileLastModifyDate(const string_q& filename);
typedef struct {
    time_q fileTime;
    string_q fileName;
} fileInfo;
extern bool getNewestFile(const string_q& path, void* data);
extern fileInfo getNewestFileInFolder(const string_q& path);
}  // namespace qblocks
