#include "Hooks.h"
#include "Formatter.h"
#include "Settings.h"

namespace TimeFormatChanger
{
	const char* GetOrdinalSuffix(std::uint32_t a_number)
	{
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		assert(a_number <= 31);

		switch (a_number)
		{
		case 1:
		case 21:
		case 31:
			return gmst->GetSetting("sFirstOrdSuffix")->GetString(); //st
		case 2:
		case 22:
			return gmst->GetSetting("sSecondOrdSuffix")->GetString(); //nd
		case 3:
		case 23:
			return gmst->GetSetting("sThirdOrdSuffix")->GetString(); //rd
		default:
			return gmst->GetSetting("sDefaultOrdSuffix")->GetString(); //th
		}
	}

	void FormatTime_Hook(RE::Calendar* a_calendar, char* a_str, std::uint64_t a_bufferSize, bool a_showYear)
	{
		std::int32_t year = a_calendar->GetYear();
		std::int32_t month = a_calendar->GetMonth() + 1; //indexed from 0
		std::int32_t day = static_cast<std::int32_t>(a_calendar->GetDay());
		std::int32_t hour = static_cast<std::int32_t>(a_calendar->GetHour());
		std::int32_t minutes = static_cast<std::int32_t>((a_calendar->GetHour() - hour) * 60);

		Settings* settings = TimeFormatChanger::Settings::GetSingleton();
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		Formatter formatter{};

		//Era
		formatter.era = "4E";

		//Year
		formatter.year = std::to_string(year);

		//Month
		if (month < 10 && settings->leadingZeroMonth)
		{
			std::string leadingZero = "0";
			formatter.month = leadingZero.append(std::to_string(month));
		}
		else
		{
			formatter.month = std::to_string(month);
		}

		//Day of month
		if (day < 10 && settings->leadingZeroDay)
		{
			std::string leadingZero = "0";
			formatter.day = leadingZero.append(std::to_string(day));
		}
		else
		{
			formatter.day = std::to_string(day);
		}

		//Hour (24h)
		if (hour < 10 && settings->leadingZeroHour)
		{
			std::string leadingZero = "0";
			formatter.hours24 = leadingZero.append(std::to_string(hour));
		}
		else
		{
			formatter.hours24 = std::to_string(hour);
		}

		//Hour (12h)
		if (hour < 1)
		{
			//After midnight
			//12:00 AM - 12:59 AM
			formatter.am_pm = gmst->GetSetting("sTimeAM")->GetString();
			formatter.hours12 = std::to_string(hour + 12);
		}
		else if (hour >= 1 && hour < 12)
		{
			//1:00 AM - 12:59 AM
			formatter.am_pm = gmst->GetSetting("sTimeAM")->GetString();
			formatter.hours12 = std::to_string(hour);
		}
		else if (hour >= 12 && hour < 13)
		{
			//After noon
			//12:00 PM - 12:59 PM
			formatter.am_pm = gmst->GetSetting("sTimePM")->GetString();
			formatter.hours12 = std::to_string(hour);
		}
		else
		{
			//1:00 PM - 11:59 PM
			formatter.am_pm = gmst->GetSetting("sTimePM")->GetString();
			formatter.hours12 = std::to_string(hour - 12);
		}

		//Minutes
		if (minutes < 10)
		{
			std::string leadingZero = "0";
			formatter.minutes = leadingZero.append(std::to_string(minutes));
		}
		else
		{
			formatter.minutes = std::to_string(minutes);
		}
		
		//Ordinal suffix
		formatter.daySuffix = GetOrdinalSuffix(day);

		//The string " of "
		formatter.of = gmst->GetSetting("sOf")->GetString();

		//Immersive Name
		if (hour < 24)
		{
			formatter.immersiveName = settings->immersiveNames[hour];
		}
		else
		{
			formatter.immersiveName = "Bad Hour";
		}
				

		//Full name of the month
		formatter.longMonth = a_calendar->GetMonthName();

		//Day of week
		formatter.weekday = a_calendar->GetDayName();

		std::string formattedString;

		if (a_showYear)
		{
			formattedString = formatter.GetFormattedTime(settings->format);
		} 
		else
		{
			formattedString = formatter.GetFormattedTime(settings->formatNoYear);
		}

		strcpy_s(a_str, a_bufferSize, formattedString.c_str());
	}

	void InstallHook()
	{
		SKSE::GetTrampoline().write_branch<5>(REL::ID(static_cast<std::uint64_t>(35413)).address(), (std::uintptr_t)FormatTime_Hook);
	}
}
