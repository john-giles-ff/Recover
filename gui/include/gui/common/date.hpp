#ifndef C_DATETIME_H
#define C_DATETIME_H

#include <ctime>
//#include <gui\common\String.hpp>
#include <touchgfx\Utils.hpp>

class DateTime {
  /** Milliseconds since the start of January, 1 of the 1'st year
   */
  long long m_time;

public:
  /** Default constructor. Sets the instance to invalid date and time
   */
  DateTime ();

  /** Copy constructor
   */
  DateTime (const DateTime &value);    

  /** Construct DateTime instance from a time_t value
   * /param time        Seconds from UNIX-epoch start in UTC
   */
  DateTime (const time_t &time);

  /** Construct DateTime value from struct *tm
   * /param time        struct std::tm date-time object
   */
  DateTime (const tm *time);

#ifdef WIN32
  /** Construct DateTime value from Windows FILETIME struct
   */
  DateTime (const FILETIME &time);

  void set (const FILETIME &time);
#endif

  /** Check validity of the date-time value of this instance
   * /result      False if the instance does not contain valid date and time
   */
  bool isValid(void) const;

  /** Return raw date and time value of this instance
   * /result      Milliseconds from Jan, 1 of the 1'st year
   */
  long long getRaw(void) const;

  /** Get date-time
   * /returns       Date and time in UNIX time_t format
   */
  time_t asUnixTime(void) const;

  /** Get date and time
   * /param time    Time to accept value of the DateTime
   * /returns       False if the DateTime is invalid
   */
  bool asTime(tm *time);

  /** Set value to current date and time (UTC)
   */
  void setNow(void);

	/** Check does this date-time value contain time portion
	 * /returns				True if this value has time portion
	 */
	bool hasTime(void) const;  

  /** Set date and time
   * /param time      A time_t (milliseconds from UNIX epoch) value
   */
  void set (const time_t &time);

  /** Set date and time
   * /param time      Date and time in a tm format
   */
  void set (const tm *time);  

  /** Increase date and time by certain amount of seconds
   *  /param seconds     Amount of seconds by which to increment/decrement current date-time
   *   Negative parameter means subtruction
   */
  DateTime& incSecond(int seconds);

  /** Increase date and time by certain amount of minutes
   *  /param minutes     Amount of minutes by which to increment/decrement current date-time
   *   Negative parameter means subtruction
   */
  DateTime& incMinute(int minutes);

  /** Increase date and time by certain amount of hours
   *  /param hours     Amount of hours by which to increment/decrement current date-time
   *   Negative parameter means subtruction
   */
  DateTime& incHour(int hours);

  /** Increase date and time by certain amount of days
   *  /param days     Amount of days by which to increment/decrement current date-time
   *   Negative parameter means days' subtruction
   */
  DateTime& incDay(int days);

  /** Increase date and time by given amount of months
   * /param months      Amount of months by which current daate should be incremented/decremented
   *   May be negative (for months' subtraction) and can exceed a single year diapasone.
   */
  DateTime& incMonth(int months);

  /** Increase date and time by given amount of years
   * /param years      Amount of years by which current daate should be incremented/decremented
   *   May be negative (for subtraction).
   */
  DateTime& incYear(int years);

  /** Get weekday of the date
   * /result      Weekday of a valid date (0 for Mon, 6 for Sun) or -1
   */
  int getWeekDay(void) const;

  /** Get day of the year
   * /result      Day of the year of a valid date or -1 for the invalid
   */
  int getDayOfYear(void) const;

  /** Get amount of days between two DateTime values
   * /param date1       First date
   * /param date2       Second date
   *  Returns amount of whole days between two valid dates or -1 otherwise
   */
  static int daysBetween(const DateTime &date1, const DateTime &date2);

  /** Get amount of months between two DateTime values
   * /param date1       First date
   * /param date2       Second date
   *  Returns amount of whole months between two valid dates or -1 otherwise
   */
  static int monthsBetween(const DateTime &date1, const DateTime &date2);

  /** Get amount of years between two DateTime values
   * /param date1       First date
   * /param date2       Second date
   *  Returns amount of whole years between two valid dates or -1 otherwise
   */
  static int yearsBetween(const DateTime &date1, const DateTime &date2);

  /** This value differs from the provided one not more then by a minute?
   */
  bool identic(const DateTime &other) const;

  const DateTime& operator= (const DateTime &date);

  friend bool operator == (const DateTime &date1, const DateTime &date2);
  friend bool operator < (const DateTime &date1, const DateTime &date2);
  friend bool operator != (const DateTime &date1, const DateTime &date2);
  friend bool operator <= (const DateTime &date1, const DateTime &date2);
  friend bool operator > (const DateTime &date1, const DateTime &date2);
  friend bool operator >= (const DateTime &date1, const DateTime &date2);
};

/** Quick and dirty replacement for struct tm with
 *  additional functionality
 */
class STime {
	int year;
	int month;  // 0 - 11
	int day;    // 1 - xx
	int hour;
	int minute;
	int second;
	int millisecond;
	bool valid;

	// Extract month and day from given monthTable (MONTH_STARTS or MONTH_STARTS_LEAP)
	void setYearsDay(const int *monthTable, const int *lengths, int yearDay);
	void Reset();

public:
	STime();
	STime(long long time);
	STime(const tm *time);	

	// Get amount of milliseconds from J1n, 1 of the 1'th year
	long long get(void);

	int GetYear() { return year; }
	int GetMonth() { return month; }
	int GetDay() { return day; }
	int GetHour() { return hour; }
	int GetMinute() { return minute; }
	int GetSecond() { return second; }
	int GetMillisecond() { return millisecond; }	

	// Returns day-of-year for the date
	int dayOfYear(void) const;

	// Add months to the date-time
	void incMonth(int months) { month += months; }

	// Add years to the date-time
	void incYear(int years) { year += years; }	

	// Get amount of months since a previous date
	// (this date-time MUST be before or equal to the "from" one)
	int monthsAfter(const STime &from);

	// Get STime in representation of struct tm
	void asTime(tm *time);
};

inline bool operator == (const DateTime &date1, const DateTime &date2) {
  return date1.m_time == date2.m_time;
}

inline bool operator < (const DateTime &date1, const DateTime &date2) {
  return date1.m_time < date2.m_time;
}

inline bool operator != (const DateTime &date1, const DateTime &date2) {
  return !(date1 == date2);
}

inline bool operator <= (const DateTime &date1, const DateTime &date2) {
  return date1 < date2 || date1 == date2;
}

inline bool operator > (const DateTime &date1, const DateTime &date2) {
  return !(date1 <= date2);
}

inline bool operator >= (const DateTime &date1, const DateTime &date2) {
  return !(date1 < date2);
}

#endif