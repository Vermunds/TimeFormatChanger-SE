#include "Settings.h"
#include "SimpleIni.h"

namespace TimeFormatChanger
{
	Settings* Settings::GetSingleton()
	{
		static Settings singleton;
		return &singleton;
	}

	void LoadSettings()
	{
		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini");

		ini.SetValue("FORMAT", nullptr, nullptr,
			"# Use these tags to create a custom format : \n"
			"#\n"
			"# %D - Day of week\n"
			"# %h - Hours (in 12h format)\n"
			"# %H - Hours (in 24h format)\n"
			"# %m - Minutes\n"
			"# %a - AM / PM\n"
			"# %d - Day of month\n"
			"# %t - Suffix after the day (In the english version of the game : \"st\", \"nd\", \"rd\" or \"th\")\n"
			"# %o  - \" of \" (used between the date and the month: 17th of ..., contains spaces)\n"
			"# %M  - Month of year (as a number)\n"
			"# %L  - Month of year (full name)\n"
			"# %E  - Era (always 4E)\n"
			"# %y  - Year\n"
			"# %i  - Immersive time of day\n"
			"\n"
			"# Example (vanilla format):\n"
			"# Turdas, 1:13 PM, 21st of Last Seed, 4E 201\n"
			"# %D, %h:%m %a, %d%t%o%M, %E %y\n"
			"#\n"
			"# Warning: too long strings will get trimmed. The actual character limit depends on the content. It is recommended to try the longest possible date before use.\n"
			"# You can force display any date if you change the format code to that specific date, like this:\n"
			"\n"
			"# sTimeFormat=\"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\"\n"
			"\n"
			"# This will always display \"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\" regardless of the actual in-game time. ");

		settings->format = ini.GetValue("FORMAT", "sTimeFormat", "%D, %h:%m %a, %d%t%o%L, %E %y");
		ini.SetValue("FORMAT", "sTimeFormat", settings->format.c_str(), nullptr, true);

		settings->formatNoYear = ini.GetValue("FORMAT", "sTimeFormatNoYear", "%D, %h:%m %a, %d%t%o%L");
		ini.SetValue("FORMAT", "sTimeFormatNoYear", settings->formatNoYear.c_str(), "# Format without year(used in Sleep / Wait menu)", true);

		settings->leadingZeroHour = ini.GetBoolValue("FORMAT", "bLeadingZeroHour", false);
		ini.SetBoolValue("FORMAT", "bLeadingZeroHour", settings->leadingZeroHour, "# If enabled a leading zero will be added to the hour if it's less than 10 (8:30 -> 08:30)\n# Only for 24 hour time!", true);

		settings->leadingZeroDay = ini.GetBoolValue("FORMAT", "bLeadingZeroDay", false);
		ini.SetBoolValue("FORMAT", "bLeadingZeroDay", settings->leadingZeroDay, "# If enabled a leading zero will be added to the day of month if it's less than 10 (7 -> 07)", true);

		settings->leadingZeroMonth = ini.GetBoolValue("FORMAT", "bLeadingZeroMonth", false);
		ini.SetBoolValue("FORMAT", "bLeadingZeroMonth", settings->leadingZeroMonth, "# If enabled a leading zero will be added to the month if it's less than 10 (7 -> 07)", true);

		for (int i = 0; i < 24; ++i)
		{
			std::string section = "s"s + (i < 10 ? "0" : "") + std::to_string(i) + "_"s + (i + 1 < 10 ? "0"s : ""s) + std::to_string(i + 1);
			std::string defaultValue = (i >= 4 && i < 6)   ? "Dawn"s :
			                           (i >= 6 && i < 10)  ? "Morning"s :
			                           (i >= 10 && i < 12) ? "Midday"s :
			                           (i >= 12 && i < 17) ? "Afternoon"s :
			                           (i >= 17 && i < 21) ? "Evening"s :
			                                                 "Night"s;

			settings->immersiveNames[i] = ini.GetValue("IMMERSIVE_NAMES", section.c_str(), defaultValue.c_str());
			ini.SetValue("IMMERSIVE_NAMES", section.c_str(), settings->immersiveNames[i].c_str(), nullptr, true);
		}

		ini.SaveFile(".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini");
	}
}
