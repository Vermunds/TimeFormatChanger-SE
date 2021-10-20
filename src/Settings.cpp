#include "Settings.h"
#include "SimpleIni.h"

namespace TimeFormatChanger
{

	Settings* Settings::singleton = nullptr;

	Settings::Settings()
	{

	}

	Settings* Settings::GetSingleton()
	{
		if (singleton)
		{
			return singleton;
		}
		singleton = new Settings();
		return singleton;
	}

	void LoadSettings()
	{
		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini");

		ini.SetValue("FORMAT", nullptr, nullptr, "# Use these tags to create a custom format : \n#\n# % D - Day of week\n# % h - Hours(in 12h format)\n# % H - Hours(in 24h format)\n# % m - Minutes\n# % a - AM / PM\n# % d - Day of month\n# % t - Suffix after the day(In the english version of the game : \"st\", \"nd\", \"rd\" or \"th\")\n# %o  - \" of \" (used between the date and the month: 17th of ..., contains spaces)\n# %M  - Month of year (as a number)\n# %L  - Month of year (full name)\n# %E  - Era (always 4E)\n# %y  - Year\n# %i  - Immersive time of day\n\n# Example (vanilla format):\n# Turdas, 1:13 PM, 21st of Last Seed, 4E 201\n# %D, %h:%m %a, %d%t%o%M, %E %y\n#\n# Warning: too long strings will get trimmed. The actual character limit depends on the content. It is recommended to try the longest possible date before use.\n# You can force display any date if you change the format code to that specific date, like this:\n\n# sTimeFormat=\"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\"\n\n# This will always display \"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\" regardless of the actual in-game time. ");

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

		ini.SetValue("IMMERSIVE_NAMES", nullptr, nullptr, "# Here you can change the names of the immersive times of the day on a hourly basis (from-to)");

		settings->immersiveNames[0] = ini.GetValue("IMMERSIVE_NAMES", "s00_01", "Night");
		settings->immersiveNames[1] = ini.GetValue("IMMERSIVE_NAMES", "s01_02", "Night");
		settings->immersiveNames[2] = ini.GetValue("IMMERSIVE_NAMES", "s02_03", "Night");
		settings->immersiveNames[3] = ini.GetValue("IMMERSIVE_NAMES", "s03_04", "Night");
		settings->immersiveNames[4] = ini.GetValue("IMMERSIVE_NAMES", "s04_05", "Dawn");
		settings->immersiveNames[5] = ini.GetValue("IMMERSIVE_NAMES", "s05_06", "Dawn");
		settings->immersiveNames[6] = ini.GetValue("IMMERSIVE_NAMES", "s06_07", "Morning");
		settings->immersiveNames[7] = ini.GetValue("IMMERSIVE_NAMES", "s07_08", "Morning");
		settings->immersiveNames[8] = ini.GetValue("IMMERSIVE_NAMES", "s08_09", "Morning");
		settings->immersiveNames[9] = ini.GetValue("IMMERSIVE_NAMES", "s09_10", "Morning");
		settings->immersiveNames[10] = ini.GetValue("IMMERSIVE_NAMES", "s10_11", "Midday");
		settings->immersiveNames[11] = ini.GetValue("IMMERSIVE_NAMES", "s11_12", "Midday");
		settings->immersiveNames[12] = ini.GetValue("IMMERSIVE_NAMES", "s12_13", "Afternoon");
		settings->immersiveNames[13] = ini.GetValue("IMMERSIVE_NAMES", "s13_14", "Afternoon");
		settings->immersiveNames[14] = ini.GetValue("IMMERSIVE_NAMES", "s14_15", "Afternoon");
		settings->immersiveNames[15] = ini.GetValue("IMMERSIVE_NAMES", "s15_16", "Afternoon");
		settings->immersiveNames[16] = ini.GetValue("IMMERSIVE_NAMES", "s16_17", "Afternoon");
		settings->immersiveNames[17] = ini.GetValue("IMMERSIVE_NAMES", "s17_18", "Evening");
		settings->immersiveNames[18] = ini.GetValue("IMMERSIVE_NAMES", "s18_19", "Evening");
		settings->immersiveNames[19] = ini.GetValue("IMMERSIVE_NAMES", "s19_20", "Evening");
		settings->immersiveNames[20] = ini.GetValue("IMMERSIVE_NAMES", "s20_21", "Evening");
		settings->immersiveNames[21] = ini.GetValue("IMMERSIVE_NAMES", "s21_22", "Night");
		settings->immersiveNames[22] = ini.GetValue("IMMERSIVE_NAMES", "s22_23", "Night");
		settings->immersiveNames[23] = ini.GetValue("IMMERSIVE_NAMES", "s23_24", "Night");

		ini.SetValue("IMMERSIVE_NAMES", "s00_01", settings->immersiveNames[0].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s01_02", settings->immersiveNames[1].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s02_03", settings->immersiveNames[2].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s03_04", settings->immersiveNames[3].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s04_05", settings->immersiveNames[4].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s05_06", settings->immersiveNames[5].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s06_07", settings->immersiveNames[6].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s07_08", settings->immersiveNames[7].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s08_09", settings->immersiveNames[8].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s09_10", settings->immersiveNames[9].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s10_11", settings->immersiveNames[10].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s11_12", settings->immersiveNames[11].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s12_13", settings->immersiveNames[12].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s13_14", settings->immersiveNames[13].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s14_15", settings->immersiveNames[14].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s15_16", settings->immersiveNames[15].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s16_17", settings->immersiveNames[16].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s17_18", settings->immersiveNames[17].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s18_19", settings->immersiveNames[18].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s19_20", settings->immersiveNames[19].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s20_21", settings->immersiveNames[20].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s21_22", settings->immersiveNames[21].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s22_23", settings->immersiveNames[22].c_str(), nullptr, true);
		ini.SetValue("IMMERSIVE_NAMES", "s23_24", settings->immersiveNames[23].c_str(), nullptr, true);

		ini.SaveFile(".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini");

	}
}
