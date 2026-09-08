#include "Settings.h"
#include "SimpleIni.h"

namespace
{
	void IniSection(CSimpleIniA& a_ini, const char* a_section, const char* a_comment = nullptr)
	{
		a_ini.SetValue(a_section, nullptr, nullptr, a_comment);
		SKSE::log::info("[{}]", a_section);
	}

	bool IniGetBool(CSimpleIniA& a_ini, const char* a_section, const char* a_key, bool a_default, const char* a_comment = nullptr)
	{
		bool val = a_ini.GetBoolValue(a_section, a_key, a_default);
		a_ini.SetBoolValue(a_section, a_key, val, a_comment, true);
		SKSE::log::info("  {}: {}", a_key, val);
		return val;
	}

	std::string IniGetString(CSimpleIniA& a_ini, const char* a_section, const char* a_key, const char* a_default, const char* a_comment = nullptr)
	{
		std::string val = a_ini.GetValue(a_section, a_key, a_default);
		a_ini.SetValue(a_section, a_key, val.c_str(), a_comment, true);
		SKSE::log::info("  {}: {}", a_key, val);
		return val;
	}
}

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

		constexpr const char* iniPath = R"(.\Data\SKSE\Plugins\TimeFormatChanger.ini)";

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(iniPath);

		SKSE::log::info("Loading settings from: {}", std::filesystem::absolute(iniPath).string());

		IniSection(ini, "FORMAT",
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

		settings->format = IniGetString(ini, "FORMAT", "sTimeFormat", "%D, %h:%m %a, %d%t%o%L, %E %y");
		settings->formatNoYear = IniGetString(ini, "FORMAT", "sTimeFormatNoYear", "%D, %h:%m %a, %d%t%o%L", "# Format without year(used in Sleep / Wait menu)");
		settings->leadingZeroHour = IniGetBool(ini, "FORMAT", "bLeadingZeroHour", false, "# If enabled a leading zero will be added to the hour if it's less than 10 (8:30 -> 08:30)\n# Only for 24 hour time!");
		settings->leadingZeroDay = IniGetBool(ini, "FORMAT", "bLeadingZeroDay", false, "# If enabled a leading zero will be added to the day of month if it's less than 10 (7 -> 07)");
		settings->leadingZeroMonth = IniGetBool(ini, "FORMAT", "bLeadingZeroMonth", false, "# If enabled a leading zero will be added to the month if it's less than 10 (7 -> 07)");

		IniSection(ini, "IMMERSIVE_NAMES");
		for (int32_t i = 0; i < 24; ++i)
		{
			std::string section = "s"s + (i < 10 ? "0" : "") + std::to_string(i) + "_"s + (i + 1 < 10 ? "0"s : ""s) + std::to_string(i + 1);
			std::string defaultValue = (i >= 4 && i < 6)   ? "Dawn"s :
			                           (i >= 6 && i < 10)  ? "Morning"s :
			                           (i >= 10 && i < 12) ? "Midday"s :
			                           (i >= 12 && i < 17) ? "Afternoon"s :
			                           (i >= 17 && i < 21) ? "Evening"s :
			                                                 "Night"s;

			settings->immersiveNames[i] = IniGetString(ini, "IMMERSIVE_NAMES", section.c_str(), defaultValue.c_str());
		}

		SKSE::log::info("Settings loaded.");

		ini.SaveFile(iniPath);
	}
}
