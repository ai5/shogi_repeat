﻿#pragma once
#ifndef SHOGI_DATE_TIME_H_
#define SHOGI_DATE_TIME_H_

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

class DateTime : public std::chrono::time_point<std::chrono::system_clock>
{
public:
	DateTime() {}
	
	DateTime(const std::chrono::time_point<std::chrono::system_clock>& time) : std::chrono::time_point<std::chrono::system_clock>(time)
	{
	}

	DateTime(int year, int month, int day, int hour, int minute, int second)
	{
		tm date;
		date.tm_year = year - 1900;
		date.tm_mon = month - 1;
		date.tm_mday = day;
		date.tm_hour = hour;
		date.tm_min = minute;
		date.tm_sec = second;
		time_t time = mktime(&date);
		
		*this = std::chrono::system_clock::from_time_t(time);
	}

	~DateTime() {}


	static DateTime Now() { return std::chrono::system_clock::now(); }

	std::string ToString() const {
		time_t time = std::chrono::system_clock::to_time_t(*this);
		tm local_tm;
#ifdef _WIN32
		localtime_s(&local_tm, &time);
#else
		local_tm = *localtime(&time);
#endif
		return std::to_string(1900 + local_tm.tm_year) + "/"
			+ std::to_string(1 + local_tm.tm_mon) + "/"
			+ std::to_string(local_tm.tm_mday) + " "
			+ std::to_string(local_tm.tm_hour) + ":"
			+ std::to_string(local_tm.tm_min) + ":"
			+ std::to_string(local_tm.tm_sec);
	}

	std::wstring ToWString() const {
		time_t time = std::chrono::system_clock::to_time_t(*this);
		tm local_tm;
#ifdef _WIN32
		localtime_s(&local_tm, &time);
#else
		local_tm = *localtime(&time);
#endif
		
		return std::to_wstring(1900 + local_tm.tm_year) + L"/"
			+ std::to_wstring(1 + local_tm.tm_mon) + L"/"
			+ std::to_wstring(local_tm.tm_mday) + L" "
			+ std::to_wstring(local_tm.tm_hour) + L":"
			+ std::to_wstring(local_tm.tm_min) + L":"
			+ std::to_wstring(local_tm.tm_sec);
	}


	tm LocalTime() {
		time_t time = std::chrono::system_clock::to_time_t(*this);
		tm local_tm;
#ifdef _WIN32
		localtime_s(&local_tm, &time);
#else
		local_tm = *localtime(&time);
#endif
		local_tm.tm_year += 1900;
		local_tm.tm_mon += 1;

		return local_tm;
	}

	/*-----------------------------------------------------------------------------*/
	/**
	* @brief 時間を文字列に変換する
	* @param time 時間(ms)
	* @note
	*/
	/*-----------------------------------------------------------------------------*/
	static std::string ToTimeString(int time)
	{
		int min = time / (1000 * 60);
		int sec = (time - (min * 1000 * 60)) / 1000;
		int msec = time % 1000;

		std::ostringstream str;

		if (min != 0)
		{
			str << min << + ":" << std::setfill('0') << std::setw(2) << sec;
		}
		else
		{
			str << sec;
		}

		if (msec != 0)
		{
			str << ".";
			for (int i = 100; i >= 1 && msec != 0; i = i / 10)
			{
				int n = msec / i;
				str << n;

				msec -= n * i;

			}
		}

		return str.str();
	}

};

#endif
