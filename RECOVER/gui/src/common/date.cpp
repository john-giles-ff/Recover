#include <sstream>
#include <iomanip>
#include <climits>
#include <gui\common\date.hpp>

using namespace std;


// Year for which tm::tm_year is zero:
const int TM_START_YEAR = 1900;

// Seconds to milliseconds quotient
const int MONTH_COUNT = 12;
const int SECS_IN_MINUTE = 60;
const int SECS_IN_HOUR = SECS_IN_MINUTE * 60;
const int SECS_IN_DAY = SECS_IN_HOUR * 24;

// Displacement of time_t ticks related to DateTime::m_time
const long long TIME_T_ZERO = 62167132800000;

const int MONTH_LENGTHS[12]      =     {31, 28, 31, 30,   31,  30,  31,  31,  30,  31,  30,  31};
const int MONTH_LENGTHS_LEAP[12] =     {31, 29, 31, 30,   31,  30,  31,  31,  30,  31,  30,  31};

const int MONTH_STARTS[13]       = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

// Spare day of the leap year counted in the leap-year-days
// So it's compensated after February but lacks in Jan & Feb
const int MONTH_STARTS_LEAP[13]  = {-1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

/** Check if the year is leap
 */
inline bool isLeap (int year) {
  // Gregorian
  return (year % 4 == 0)
    && ((year % 100 != 0) || (year % 400 == 0));
}

/** Count leap days in the years
 */
inline int getLeapDays(int years) {
  // Gregorian
  return years / 4 - years / 100 + years / 400;
}



STime::STime():
  year(0),
  month(0),
  day(0),
  hour(0),
  minute(0),
  second(0),
  valid(true)
{}

void STime::Reset() {
	year = 0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;	

}


STime::STime (long long time): valid(true) {  
  // Seconds since the day start
  second = time % SECS_IN_DAY;

  hour = second / SECS_IN_HOUR;
  second %= SECS_IN_HOUR;

  minute = second / SECS_IN_MINUTE;
  second %= SECS_IN_MINUTE;
  // ... now everything within the day is correct

  // Amount of days since 1.01.01
  time /= SECS_IN_DAY;

  // Maximum possible years count for this amount of days:
  year = static_cast<int>(time / 365);
  year -= getLeapDays(year) / 365;

  // The year changed, leap days count may be changed also:
  int leap = getLeapDays(year);

  // Day of the year for these year and leap days count:
  long long days = time - (long long)year * 365 - leap;

  // Adjusting the year to achieve days being in [0; 365]
  // Down:
  while (days < 0) {
    year--;
    leap = getLeapDays(year);
    days = time - (long long)year * 365 - leap;
  }

  // Up:
  while (days > 365) {
    year++;
    leap = getLeapDays(year);
    days = time - (long long)year * 365 - leap;
  }

  // Split day-of-the year onto month and day
  if (isLeap(year))
    setYearsDay(MONTH_STARTS_LEAP, MONTH_LENGTHS_LEAP, static_cast<int>(days));
  else
    setYearsDay(MONTH_STARTS, MONTH_LENGTHS, static_cast<int>(days));
  
}

void STime::setYearsDay(const int *monthTable, const int *lengths, int yearDay) {
  for (int i = 1; i < 13; i++) {
    if (yearDay <= monthTable[i]) {
      month = i-1;
      day = yearDay - monthTable[i-1] + 1;	  

      break;
    }
  }

  // Lame date correction for the cases like Jan, 32
  if (day > lengths[month]) {
    day = 1;
    month++;
    if (month > 11) {
      year++;
      month = 0;
    }	
  }
}

long long STime::get(void) {
  if (!valid) return LLONG_MIN;

  // By chance, month may be more than 11...
  year += month / MONTH_COUNT;
  month %= MONTH_COUNT;  

  int mdays;
  if (isLeap(year))
    mdays = MONTH_STARTS_LEAP[month];
  else
    mdays = MONTH_STARTS[month];

  // Days amount from Jan, 1 of the 1'st year
  long long result = year * 365 // in non-leap years
    + getLeapDays(year)         // days in leap years
    + mdays                     // the day on which the month starts
    + day                       // month's day
    - 1;                        // since month starts with day 1 (not 0)

  result *= SECS_IN_DAY;
  result += hour * SECS_IN_HOUR;
  result += minute * SECS_IN_MINUTE;
  result += second;    

  return result;
}



int STime::dayOfYear(void) const {
  if (isLeap(year))
    return MONTH_STARTS_LEAP[month] + day;
  else
    return MONTH_STARTS[month] + day;
}

//******************************
DateTime::DateTime () {
  // Invalid date-time by default	
  m_time = 0;
}

DateTime::DateTime(const DateTime &value) :	
  m_time(value.m_time)
{}

DateTime::DateTime(long long time)
{
	m_time = time;
}

bool DateTime::isValid(void) const {
  return m_time != LLONG_MIN;
}

long long DateTime::getRaw(void) const {
  return m_time;
}

void DateTime::setRaw(long long value)
{
	m_time = value;
}

DateTime& DateTime::incSecond(int seconds) {	

  if (m_time != LLONG_MIN)
    m_time += (long long) seconds;
  return *this;
}

DateTime& DateTime::incMinute(int minutes) {
  if (m_time != LLONG_MIN)
    m_time += (long long) minutes * SECS_IN_MINUTE;
  return *this;
}

DateTime& DateTime::incHour(int hours) {
  if (m_time != LLONG_MIN)
    m_time += (long long) hours * SECS_IN_HOUR;
  return *this;
}

DateTime& DateTime::incDay(int days) {
  if (m_time != LLONG_MIN)
    m_time += (long long) days * SECS_IN_DAY;
  return *this;
}

DateTime& DateTime::incMonth(int months) {
  if (m_time != LLONG_MIN) {
    STime time(m_time);
    time.incMonth(months);
    m_time = time.get();
  }
  return *this;
}

DateTime& DateTime::incYear(int years) {
  if (m_time != LLONG_MIN) {
    STime time(m_time);
    time.incYear(years);
    m_time = time.get();
  }
  return *this;
}

const DateTime& DateTime::operator = (const DateTime &date) {
  m_time = date.m_time;
  return *this;
}

