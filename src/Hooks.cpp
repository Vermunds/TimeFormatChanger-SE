#include "skse64_common/BranchTrampoline.h"

#include "RE/BSFixedString.h"

#include "Hooks.h"
#include "Formatter.h"
#include "Settings.h"
#include "Offsets.h"

namespace Hooks
{
	const char*(*GetDaySuffix)(GameTimeHolder* a_holder, UInt8 a_day);

	const char* GetMonthName(UInt8 a_month)
	{
		if (a_month <= 11)
		{
			uintptr_t* ptr = reinterpret_cast<uintptr_t*>(Offsets::MonthNameHolder.GetUIntPtr() + (a_month * 8));
			const char** str = reinterpret_cast<const char**>(*ptr + 8);
			return *str;
		}
		return "Bad Month";
	}

	const char* GetWeekDayName(UInt8 a_weekday)
	{
		UInt8 weekday = a_weekday % 7;
		uintptr_t* ptr = reinterpret_cast<uintptr_t*>(Offsets::WeekDayNameHolder.GetUIntPtr() + (weekday * 8));
		const char** str = reinterpret_cast<const char**>(*ptr + 8);
		return *str;
	}

	std::string GetImmersiveName(UInt8 a_hour)
	{
		TimeFormatChanger::SettingStore* settings = TimeFormatChanger::SettingStore::GetSingleton();

		switch (a_hour)
		{
		case 0:
			return settings->immersiveName_00_01;
		case 1:
			return settings->immersiveName_01_02;
		case 2:
			return settings->immersiveName_02_03;
		case 3:
			return settings->immersiveName_03_04;
		case 4:
			return settings->immersiveName_04_05;
		case 5:
			return settings->immersiveName_05_06;
		case 6:
			return settings->immersiveName_06_07;
		case 7:
			return settings->immersiveName_07_08;
		case 8:
			return settings->immersiveName_08_09;
		case 9:
			return settings->immersiveName_09_10;
		case 10:
			return settings->immersiveName_10_11;
		case 11:
			return settings->immersiveName_11_12;
		case 12:
			return settings->immersiveName_12_13;
		case 13:
			return settings->immersiveName_13_14;
		case 14:
			return settings->immersiveName_14_15;
		case 15:
			return settings->immersiveName_15_16;
		case 16:
			return settings->immersiveName_16_17;
		case 17:
			return settings->immersiveName_17_18;
		case 18:
			return settings->immersiveName_18_19;
		case 19:
			return settings->immersiveName_19_20;
		case 20:
			return settings->immersiveName_20_21;
		case 21:
			return settings->immersiveName_21_22;
		case 22:
			return settings->immersiveName_22_23;
		case 23:
			return settings->immersiveName_23_24;
		default:
			return "Bad Hour";
		}
	}

	void FormatTime_Hook(GameTimeHolder* a_holder, char* a_str, UInt64 a_bufferSize, bool a_showYear)
	{
		float year = a_holder->year->value;
		float month = a_holder->month->value;
		float day = a_holder->day->value;
		float hour = a_holder->hour->value;
		float weekday = a_holder->weekday->value;

		TimeFormatChanger::SettingStore* settings = TimeFormatChanger::SettingStore::GetSingleton();
		TimeFormatChanger::Formatter* formatter = new TimeFormatChanger::Formatter();

		formatter->era = "4E";
		formatter->year = std::to_string(static_cast<UInt8>(year));

		UInt8 monthInt = static_cast<UInt8>(month + 1);
		if (monthInt < 10 && settings->leadingZeroMonth)
		{
			std::string leadingZero = "0";
			formatter->month = leadingZero.append(std::to_string(monthInt));
		}
		else
		{
			formatter->month = std::to_string(monthInt);
		}
		UInt8 dayInt = static_cast<UInt8>(day);
		if (dayInt < 10 && settings->leadingZeroDay)
		{
			std::string leadingZero = "0";
			formatter->day = leadingZero.append(std::to_string(dayInt));
		}
		else
		{
			formatter->day = std::to_string(dayInt);
		}

		UInt8 hourInt = static_cast<UInt8>(hour);
		if (hour < 10 && settings->leadingZeroHour)
		{
			std::string leadingZero = "0";
			formatter->hours24 = leadingZero.append(std::to_string(hourInt));
		}
		else
		{
			formatter->hours24 = std::to_string(hourInt);
		}

		if (hour < 1)
		{
			//After midnight
			//12:00 AM - 12:59 AM
			formatter->am_pm = reinterpret_cast<RE::BSFixedString*>(Offsets::AM_str.GetUIntPtr())->c_str();
			formatter->hours12 = std::to_string(static_cast<UInt8>(hour + 12));
		}
		else if (hour >= 1 && hour < 12)
		{
			//1:00 AM - 12:59 AM
			formatter->am_pm = reinterpret_cast<RE::BSFixedString*>(Offsets::AM_str.GetUIntPtr())->c_str();
			formatter->hours12 = std::to_string(static_cast<UInt8>(hour));
		}
		else if (hour >= 12 && hour < 13)
		{
			//After noon
			//12:00 PM - 12:59 PM
			formatter->am_pm = reinterpret_cast<RE::BSFixedString*>(Offsets::PM_str.GetUIntPtr())->c_str();
			formatter->hours12 = std::to_string(static_cast<UInt8>(hour));
		}
		else
		{
			//1:00 PM - 11:59 PM
			formatter->am_pm = reinterpret_cast<RE::BSFixedString*>(Offsets::PM_str.GetUIntPtr())->c_str();
			formatter->hours12 = std::to_string(static_cast<UInt8>(hour - 12));
		}


		UInt8 minutes = static_cast<UInt8>((hour - static_cast<UInt8>(hour)) * 60);
		if (minutes < 10)
		{
			std::string leadingZero = "0";
			formatter->minutes = leadingZero.append(std::to_string(minutes));
		}
		else
		{
			formatter->minutes = std::to_string(minutes);
		}
		
		formatter->daySuffix = GetDaySuffix(a_holder, static_cast<UInt8>(day));
		formatter->of = reinterpret_cast<RE::BSFixedString*>(Offsets::of_str.GetUIntPtr())->c_str();

		formatter->immersiveName = GetImmersiveName(static_cast<UInt8>(hour));
		formatter->longMonth = GetMonthName(static_cast<UInt8>(month));
		formatter->weekday = GetWeekDayName(static_cast<UInt8>(weekday));

		std::string formattedString;

		if (a_showYear)
		{
			formattedString = formatter->GetFormattedTime(settings->format);
		} 
		else
		{
			formattedString = formatter->GetFormattedTime(settings->formatNoYear);
		}
		delete formatter;

		strcpy_s(a_str, a_bufferSize, formattedString.c_str());
	}

	void InstallHook()
	{
		GetDaySuffix = reinterpret_cast<const char*(*)(GameTimeHolder*, UInt8)>(Offsets::GetDaySuffix.GetUIntPtr());
		g_branchTrampoline.Write5Branch(Offsets::FormatTime_Hook.GetUIntPtr(), (uintptr_t)FormatTime_Hook);
	}
}