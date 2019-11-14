#ifndef C_DATETIME_H
#define C_DATETIME_H

#include <ctime>
//#include <gui\common\String.hpp>
#include <touchgfx\Utils.hpp>
#include <touchgfx/TypedText.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <math.h>

class DateTime {
  /** Seconds since the start of January, 1 of the 1'st year
   */	

  long long m_time;  
public:
  /** Default constructor. Sets the instance to invalid date and time
   */
  DateTime ();

  /** Copy constructor
   */
  DateTime (const DateTime &value);      

  DateTime(long long time);

  /** Check validity of the date-time value of this instance
   * /result      False if the instance does not contain valid date and time
   */
  bool isValid(void) const;

  /** Return raw date and time value of this instance
   * /result      Milliseconds from Jan, 1 of the 1'st year
   */
  long long getRaw(void) const;  

  void setRaw(long long value);
   
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
	bool valid;

	// Extract month and day from given monthTable (MONTH_STARTS or MONTH_STARTS_LEAP)
	void setYearsDay(const int *monthTable, const int *lengths, int yearDay);
	void Reset();	
public:
	STime();
	STime(long long time);	

	// Get amount of milliseconds from J1n, 1 of the 1'th year
	long long get(void);

	int GetYear() { return year; }
	int GetPresentableMonth() { return month + 1; }
	int GetMonth() { return month; }	
	int GetDay() { return day; }
	int GetHour() { return hour; }
	int GetMinute() { return minute; }
	int GetSecond() { return second; }	
	
	int GetTotalMinutes() {

		return (int)floor((double)(get() / 60));
	}


	// Returns day-of-year for the date
	int dayOfYear(void) const;

	// Add months to the date-time
	void incMonth(int months) { month += months; }

	// Add years to the date-time
	void incYear(int years) { year += years; }	
	
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