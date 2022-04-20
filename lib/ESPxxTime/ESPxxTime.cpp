/*
   MIT License

  Copyright (c) 2022 3tawi

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "ESPxxTime.h"
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval

time_t now;

/*!
    @brief  Constructor for ESP8266Time
*/
ESPxxTime::ESPxxTime(){}

/*!
    @brief  set the internal RTC time
    @param  sc
            second (0-59)
    @param  mn
            minute (0-59)
    @param  hr
            hour of day (0-23)
    @param  dy
            day of month (1-31)
    @param  mt
            month (1-12)
    @param  yr
            year ie 2021
    @param  ms
            microseconds (optional)
*/
void ESPxxTime::setTime(int sc, int mn, int hr, int dy, int mt, int yr, int ms) {
  // seconds, minute, hour, day, month, year $ microseconds(optional)
  // ie setTime(20, 54, 13, 1, 1, 2022) = 13:54:20 1/1/2022
  struct tm t = {0};        // Initalize to all 0's
  t.tm_year = yr - 1900;    // This is year-1900, so 122 = 2022
  t.tm_mon = mt - 1;
  t.tm_mday = dy;
  t.tm_hour = hr;
  t.tm_min = mn;
  t.tm_sec = sc;
  time_t timeSinceEpoch = mktime(&t);
  setTime(timeSinceEpoch, ms);
}

/*!
    @brief  set the internal RTC time
    @param  epoch
            epoch time in seconds
    @param  ms
            microseconds (optional)
*/
void ESPxxTime::setTime(long epoch, int ms) {
  struct timeval tv;
  tv.tv_sec = epoch;  // epoch time (seconds)
  tv.tv_usec = ms;    // microseconds
  settimeofday(&tv, NULL);
}

/*!
    @brief  get the internal RTC time as a tm struct
*/
void ESPxxTime::getTimeStruct(){
 struct timeval tv;
  gettimeofday(&tv, nullptr);
  now = time(nullptr);
}

/*!
    @brief  get the time and date as an Arduino String object
    @param  mode
            true = Long date format
			false = Short date format
*/
String ESPxxTime::getDateTime(bool mode){
	getTimeStruct();
	struct tm* timeinfo = localtime(&now);
	char s[51];
	if (mode)
	{
		strftime(s, 50, "%A, %B %d %Y %H:%M:%S", timeinfo);
	}
	else
	{
		strftime(s, 50, "%a, %b %d %Y %H:%M:%S", timeinfo);
	}
	return String(s);
}

/*!
    @brief  get the time and date as an Arduino String object
    @param  mode
            true = Long date format
			false = Short date format
*/
String ESPxxTime::getTimeDate(bool mode){
	getTimeStruct();
	struct tm* timeinfo = localtime(&now);
	char s[51];
	if (mode)
	{
		strftime(s, 50, "%H:%M:%S %A, %B %d %Y", timeinfo);
	}
	else
	{
		strftime(s, 50, "%H:%M:%S %a, %b %d %Y", timeinfo);
	}
	return String(s);
}

/*!
    @brief  get the time as an Arduino String object
*/
String ESPxxTime::getTime(){
	getTimeStruct();
	struct tm* timeinfo = localtime(&now);
	char s[51];
	strftime(s, 50, "%H:%M:%S", timeinfo);
	return String(s);
}

/*!
    @brief  get the time as an Arduino String object with the specified format
	@param	format
			time format 
			http://www.cplusplus.com/reference/ctime/strftime/
*/
String ESPxxTime::getTime(String format){
	getTimeStruct();
	struct tm* timeinfo = localtime(&now);
	char s[128];
	char c[128];
	format.toCharArray(c, 127);
	strftime(s, 127, c, timeinfo);
	return String(s);
}

/*!
    @brief  get the date as an Arduino String object
    @param  mode
            true = Long date format
			false = Short date format
*/
String ESPxxTime::getDate(bool mode){
	getTimeStruct();
	struct tm* timeinfo = localtime(&now);
	char s[51];
	if (mode)
	{
		strftime(s, 50, "%A, %B %d %Y", timeinfo);
	}
	else
	{
		strftime(s, 50, "%a, %b %d %Y", timeinfo);
	}
	return String(s);
}

/*!
    @brief  get the current milliseconds as long
*/
long ESPxxTime::getMillis(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec/1000;
}

/*!
    @brief  get the current microseconds as long
*/
long ESPxxTime::getMicros(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec;
}

/*!
    @brief  get the current epoch seconds as long
*/
long ESPxxTime::getEpoch(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec;
}

/*!
    @brief  get the current seconds as int
*/
int ESPxxTime::getSecond(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_sec;
}

/*!
    @brief  get the current minutes as int
*/
int ESPxxTime::getMinute(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_min;
}

/*!
    @brief  get the current hour as int
	@param	mode
			true = 24 hour mode (0-23)
			false = 12 hour mode (0-12)
*/
int ESPxxTime::getHour(bool mode){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	if (mode)
	{
		return p_tm->tm_hour;
	}
	else
	{
		int hour = p_tm->tm_hour;
		if (hour > 12)
		{
			return p_tm->tm_hour-12;
		}
		else
		{
			return p_tm->tm_hour;
		}
		
	}
}

/*!
    @brief  return current hour am or pm
	@param	lowercase
			true = lowercase
			false = uppercase
*/
String ESPxxTime::getAmPm(bool lowercase){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	if (p_tm->tm_hour >= 12)
	{
		if (lowercase)
		{
			return "pm";
		}
		else
		{
			return "PM";
		}
	}
	else
	{
		if (lowercase)
		{
			return "am";
		}
		else
		{
			return "AM";
		}
	}
}

/*!
    @brief  get the current day as int (1-31)
*/
int ESPxxTime::getDay(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_mday;
}

/*!
    @brief  get the current day of week as int (0-6)
*/
int ESPxxTime::getDayofWeek(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_wday;
}

/*!
    @brief  get the current day of year as int (0-365)
*/
int ESPxxTime::getDayofYear(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_yday;
}

/*!
    @brief  get the current month as int (0-11)
*/
int ESPxxTime::getMonth(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_mon;
}

/*!
    @brief  get the current year as int
*/
int ESPxxTime::getYear(){
	getTimeStruct();
	struct tm* p_tm = localtime(&now);
	return p_tm->tm_year+1900;
}
