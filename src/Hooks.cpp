#include "skse64_common/BranchTrampoline.h"

#include "RE/BSFixedString.h"
#include "RE/BSTimeManager.h"
#include "RE/GameSettingCollection.h"

#include "Hooks.h"
#include "Formatter.h"
#include "Settings.h"
#include "Offsets.h"

namespace Hooks
{
	const char* GetOrdinalSuffix(UInt32 a_number)
	{
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		if (a_number >= 11 && a_number <= 13) {
			return gmst->GetSetting("sDefaultOrdSuffix")->GetString(); //th
		}
		switch (a_number % 10)
		{
		case 1:
			return gmst->GetSetting("sFirstOrdSuffix")->GetString(); //st
		case 2:
			return gmst->GetSetting("sSecondOrdSuffix")->GetString(); //nd
		case 3:
			return gmst->GetSetting("sThirdOrdSuffix")->GetString(); //nd
		default:
			return gmst->GetSetting("sDefaultOrdSuffix")->GetString(); //th
		}
	}

	void FormatTime_Hook(RE::BSTimeManager* a_timeManager, char* a_str, UInt64 a_bufferSize, bool a_showYear)
	{
		UInt32 year = a_timeManager->GetYear();
		UInt32 month = a_timeManager->GetMonth() + 1; //indexed from 0
		UInt32 day = static_cast<UInt32>(a_timeManager->GetDay());
		UInt32 hour = static_cast<UInt32>(a_timeManager->GetHour());
		UInt32 minutes = (a_timeManager->GetHour() - hour) * 60;

		TimeFormatChanger::SettingStore* settings = TimeFormatChanger::SettingStore::GetSingleton();
		TimeFormatChanger::Formatter* formatter = new TimeFormatChanger::Formatter();
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		//Era
		formatter->era = "4E";

		//Year
		formatter->year = std::to_string(year);

		//Month
		if (month < 10 && settings->leadingZeroMonth)
		{
			std::string leadingZero = "0";
			formatter->month = leadingZero.append(std::to_string(month));
		}
		else
		{
			formatter->month = std::to_string(month);
		}

		//Day of month
		if (day < 10 && settings->leadingZeroDay)
		{
			std::string leadingZero = "0";
			formatter->day = leadingZero.append(std::to_string(day));
		}
		else
		{
			formatter->day = std::to_string(day);
		}

		//Hour (24h)
		if (hour < 10 && settings->leadingZeroHour)
		{
			std::string leadingZero = "0";
			formatter->hours24 = leadingZero.append(std::to_string(hour));
		}
		else
		{
			formatter->hours24 = std::to_string(hour);
		}

		//Hour (12h)
		if (hour < 1)
		{
			//After midnight
			//12:00 AM - 12:59 AM
			formatter->am_pm = gmst->GetSetting("sTimeAM")->GetString();
			formatter->hours12 = std::to_string(hour + 12);
		}
		else if (hour >= 1 && hour < 12)
		{
			//1:00 AM - 12:59 AM
			formatter->am_pm = gmst->GetSetting("sTimeAM")->GetString();
			formatter->hours12 = std::to_string(hour);
		}
		else if (hour >= 12 && hour < 13)
		{
			//After noon
			//12:00 PM - 12:59 PM
			formatter->am_pm = gmst->GetSetting("sTimePM")->GetString();
			formatter->hours12 = std::to_string(hour);
		}
		else
		{
			//1:00 PM - 11:59 PM
			formatter->am_pm = gmst->GetSetting("sTimePM")->GetString();
			formatter->hours12 = std::to_string(hour - 12);
		}

		//Minutes
		if (minutes < 10)
		{
			std::string leadingZero = "0";
			formatter->minutes = leadingZero.append(std::to_string(minutes));
		}
		else
		{
			formatter->minutes = std::to_string(minutes);
		}
		
		//Ordinal suffix
		formatter->daySuffix = GetOrdinalSuffix(day);

		//The string " of "
		formatter->of = gmst->GetSetting("sOf")->GetString();

		//Immersive Name
		if (hour < 24)
		{
			formatter->immersiveName = settings->immersiveNames[hour];
		}
		else
		{
			formatter->immersiveName = "Bad Hour";
		}
				

		//Full name of the month
		formatter->longMonth = a_timeManager->GetMonthName();

		//Day of week
		formatter->weekday = a_timeManager->GetDayName();

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
		g_branchTrampoline.Write5Branch(Offsets::FormatTime_Hook.GetUIntPtr(), (uintptr_t)FormatTime_Hook);
	}
}