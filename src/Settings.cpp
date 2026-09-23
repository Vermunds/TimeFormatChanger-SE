#include "Settings.h"
#include "SimpleIni.h"

namespace
{
	constexpr const char* INI_PATH = R"(.\Data\SKSE\Plugins\TimeFormatChanger.ini)";

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

	// "s00_01", "s01_02", ...
	std::string ImmersiveNameKey(std::int32_t a_hour)
	{
		return std::format("s{:02d}_{:02d}", a_hour, a_hour + 1);
	}

	void LoadSettings()
	{
		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(INI_PATH);

		SKSE::log::info("Loading settings from: {}", std::filesystem::absolute(INI_PATH).string());

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
			"# %D, %h:%m %a, %d%t%o%L, %E %y\n"
			"#\n"
			"# Warning: too long strings may get trimmed. The actual character limit depends on the content. It is recommended to try the longest possible date before use.\n"
			"# You can force display any date if you change the format code to that specific date, like this:\n"
			"\n"
			"# sTimeFormat=\"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\"\n"
			"\n"
			"# This will always display \"Loredas, 11:13 PM, 21st of Morning Star, 4E 201\" regardless of the actual in-game time. ");

		settings->format = IniGetString(ini, "FORMAT", "sTimeFormat", FORMAT_DEFAULT_VALUE);
		settings->formatNoYear = IniGetString(ini, "FORMAT", "sTimeFormatNoYear", FORMAT_NO_YEAR_DEFAULT_VALUE, "# Format without year(used in Sleep / Wait menu)");
		settings->leadingZeroHour = IniGetBool(ini, "FORMAT", "bLeadingZeroHour", LEADING_ZERO_HOUR_DEFAULT_VALUE, "# If enabled a leading zero will be added to the hour if it's less than 10 (8:30 -> 08:30)\n# Only for 24 hour time!");
		settings->leadingZeroDay = IniGetBool(ini, "FORMAT", "bLeadingZeroDay", LEADING_ZERO_DAY_DEFAULT_VALUE, "# If enabled a leading zero will be added to the day of month if it's less than 10 (7 -> 07)");
		settings->leadingZeroMonth = IniGetBool(ini, "FORMAT", "bLeadingZeroMonth", LEADING_ZERO_MONTH_DEFAULT_VALUE, "# If enabled a leading zero will be added to the month if it's less than 10 (7 -> 07)");

		IniSection(ini, "IMMERSIVE_NAME_OVERRIDES", "# These names are used by the %i tag. The name of the interval the current in-game hour falls into is inserted into the format. You can override them below, or leave the fields empty to use the default values.");

		for (std::int32_t i = 0; i < 24; ++i)
		{
			settings->immersiveNameOverrides[i] = IniGetString(ini, "IMMERSIVE_NAME_OVERRIDES", ImmersiveNameKey(i).c_str(), IMMERSIVE_NAME_OVERRIDES_DEFAULT_VALUE);
		}

		// Legacy settings cleanup
		ini.Delete("IMMERSIVE_NAMES", nullptr, true);

		SKSE::log::info("Settings loaded.");

		ini.SaveFile(INI_PATH);
	}

	void SaveSettings()
	{
		Settings* settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(INI_PATH);

		ini.SetValue("FORMAT", "sTimeFormat", settings->format.c_str(), nullptr, true);
		ini.SetValue("FORMAT", "sTimeFormatNoYear", settings->formatNoYear.c_str(), nullptr, true);
		ini.SetBoolValue("FORMAT", "bLeadingZeroHour", settings->leadingZeroHour, nullptr, true);
		ini.SetBoolValue("FORMAT", "bLeadingZeroDay", settings->leadingZeroDay, nullptr, true);
		ini.SetBoolValue("FORMAT", "bLeadingZeroMonth", settings->leadingZeroMonth, nullptr, true);

		for (std::int32_t i = 0; i < 24; ++i)
		{
			ini.SetValue("IMMERSIVE_NAME_OVERRIDES", ImmersiveNameKey(i).c_str(), settings->immersiveNameOverrides[i].c_str(), nullptr, true);
		}

		ini.SaveFile(INI_PATH);

		SKSE::log::info("Settings saved.");
	}

	void RestoreDefaults()
	{
		Settings* settings = Settings::GetSingleton();

		settings->format = FORMAT_DEFAULT_VALUE;
		settings->formatNoYear = FORMAT_NO_YEAR_DEFAULT_VALUE;
		settings->leadingZeroHour = LEADING_ZERO_HOUR_DEFAULT_VALUE;
		settings->leadingZeroDay = LEADING_ZERO_DAY_DEFAULT_VALUE;
		settings->leadingZeroMonth = LEADING_ZERO_MONTH_DEFAULT_VALUE;

		for (std::int32_t i = 0; i < 24; ++i)
		{
			settings->immersiveNameOverrides[i] = IMMERSIVE_NAME_OVERRIDES_DEFAULT_VALUE;
		}

		SaveSettings();
	}
}
