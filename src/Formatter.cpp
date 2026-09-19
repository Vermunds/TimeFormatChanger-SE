#include "Formatter.h"
#include "ImmersiveNames.h"
#include "Settings.h"

namespace
{
	// A number the way it appears in the formatted time, padded to two digits when asked for.
	std::string FormatNumber(std::int32_t a_number, bool a_leadingZero)
	{
		std::string result = std::to_string(a_number);
		if (a_leadingZero && a_number < 10)
		{
			result.insert(0, 1, '0');
		}
		return result;
	}

	const char* GetOrdinalSuffix(std::uint32_t a_number)
	{
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		assert(a_number <= 31);

		switch (a_number)
		{
		case 1:
		case 21:
		case 31:
			return gmst->GetSetting("sFirstOrdSuffix")->GetString();  //st
		case 2:
		case 22:
			return gmst->GetSetting("sSecondOrdSuffix")->GetString();  //nd
		case 3:
		case 23:
			return gmst->GetSetting("sThirdOrdSuffix")->GetString();  //rd
		default:
			return gmst->GetSetting("sDefaultOrdSuffix")->GetString();  //th
		}
	}
}

namespace TimeFormatChanger
{
	void Formatter::Populate(RE::Calendar* a_calendar)
	{
		std::int32_t year = a_calendar->GetYear();
		std::int32_t month = a_calendar->GetMonth() + 1;  //indexed from 0
		std::int32_t day = static_cast<std::int32_t>(a_calendar->GetDay());
		std::int32_t hour = static_cast<std::int32_t>(a_calendar->GetHour());
		std::int32_t minutes = static_cast<std::int32_t>((a_calendar->GetHour() - hour) * 60);

		Settings* settings = Settings::GetSingleton();
		RE::GameSettingCollection* gmst = RE::GameSettingCollection::GetSingleton();

		//Era
		this->era = "4E";

		//Year
		this->year = std::to_string(year);

		//Month
		this->month = FormatNumber(month, settings->leadingZeroMonth);

		//Day of month
		this->day = FormatNumber(day, settings->leadingZeroDay);

		//Hour (24h)
		this->hours24 = FormatNumber(hour, settings->leadingZeroHour);

		//Hour (12h)
		std::int32_t hour12;
		if (hour < 1)
		{
			//After midnight
			//12:00 AM - 12:59 AM
			this->am_pm = gmst->GetSetting("sTimeAM")->GetString();
			hour12 = hour + 12;
		}
		else if (hour >= 1 && hour < 12)
		{
			//1:00 AM - 12:59 AM
			this->am_pm = gmst->GetSetting("sTimeAM")->GetString();
			hour12 = hour;
		}
		else if (hour >= 12 && hour < 13)
		{
			//After noon
			//12:00 PM - 12:59 PM
			this->am_pm = gmst->GetSetting("sTimePM")->GetString();
			hour12 = hour;
		}
		else
		{
			//1:00 PM - 11:59 PM
			this->am_pm = gmst->GetSetting("sTimePM")->GetString();
			hour12 = hour - 12;
		}
		this->hours12 = FormatNumber(hour12, settings->leadingZeroHour);

		//Minutes
		this->minutes = FormatNumber(minutes, true);

		//Ordinal suffix
		this->daySuffix = GetOrdinalSuffix(day);

		//The string " of "
		this->of = gmst->GetSetting("sOf")->GetString();

		//Immersive Name
		this->immersiveName = GetImmersiveName(hour);

		//Full name of the month
		this->longMonth = a_calendar->GetMonthName();

		//Day of week
		this->weekday = a_calendar->GetDayName();
	}

	std::string Formatter::GetFormattedTime(std::string a_format)
	{
		size_t pos = 0;
		while ((pos = a_format.find('%', pos)) != std::string::npos)
		{
			if (pos + 1 < a_format.length())
			{
				switch (a_format[pos + 1])
				{
				case 'D':
					a_format.replace(pos, 2, this->weekday);
					break;
				case 'h':
					a_format.replace(pos, 2, this->hours12);
					break;
				case 'H':
					a_format.replace(pos, 2, this->hours24);
					break;
				case 'm':
					a_format.replace(pos, 2, this->minutes);
					break;
				case 'a':
					a_format.replace(pos, 2, this->am_pm);
					break;
				case 'd':
					a_format.replace(pos, 2, this->day);
					break;
				case 't':
					a_format.replace(pos, 2, this->daySuffix);
					break;
				case 'o':
					a_format.replace(pos, 2, this->of);
					break;
				case 'M':
					a_format.replace(pos, 2, this->month);
					break;
				case 'L':
					a_format.replace(pos, 2, this->longMonth);
					break;
				case 'E':
					a_format.replace(pos, 2, this->era);
					break;
				case 'y':
					a_format.replace(pos, 2, this->year);
					break;
				case 'i':
					a_format.replace(pos, 2, this->immersiveName);
					break;
				default:
					// Unknown format specifier
					break;
				}
			}
			pos++;
		}
		return a_format;
	}
}
