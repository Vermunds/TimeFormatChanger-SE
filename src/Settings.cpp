#include "Settings.h"

namespace TimeFormatChanger
{

	SettingStore* SettingStore::singleton = nullptr;

	SettingStore::SettingStore()
	{

	}

	SettingStore* SettingStore::GetSingleton()
	{
		if (singleton)
		{
			return singleton;
		}
		singleton = new SettingStore();
		return singleton;
	}

	bool GetIniValue(const char* a_section, const char* a_setting, bool a_defaultValue)
	{
		const char* iniFile = ".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini";
		SInt32 value = GetPrivateProfileInt(a_section, a_setting, -1, iniFile);
		if (value < 0)
		{
			std::string str = std::to_string(a_defaultValue);
			const char* cstr = str.c_str();
			WritePrivateProfileString(a_section, a_setting, cstr, iniFile);
			return a_defaultValue;
		}
		return (bool)value;
	}

	const char* GetIniValue(const char * a_section, const char * a_key, const char* a_defaultValue)
	{
		const char* iniFile = ".\\Data\\SKSE\\Plugins\\TimeFormatChanger.ini";
		char buffer[200];

		UInt32 len = GetPrivateProfileString(a_section, a_key, a_defaultValue, buffer, 200, iniFile);
		if (GetLastError() == 0x2)
		{
			WritePrivateProfileString(a_section, a_key, a_defaultValue, iniFile);
			return a_defaultValue;
		}
		char* value = new char[len + 1];
		strncpy_s(value, len + 1, buffer, len);

		return value;
	}

	void LoadSettings()
	{
		SettingStore * settingStore = SettingStore::GetSingleton();
		
		settingStore->format = GetIniValue("FORMAT", "sTimeFormat", "%D, %h:%m %a, %d%t%o%L, %E %y");
		settingStore->formatNoYear = GetIniValue("FORMAT", "sTimeFormatNoYear", "%D, %h:%m %a, %d%t%o%L");

		settingStore->leadingZeroHour = GetIniValue("FORMAT", "bLeadingZeroHour", false);
		settingStore->leadingZeroDay = GetIniValue("FORMAT", "bLeadingZeroDay", false);
		settingStore->leadingZeroMonth = GetIniValue("FORMAT", "bLeadingZeroMonth", false);

		settingStore->immersiveName_00_01 = GetIniValue("IMMERSIVE_NAMES", "s00_01", "Night");
		settingStore->immersiveName_01_02 = GetIniValue("IMMERSIVE_NAMES", "s01_02", "Night");
		settingStore->immersiveName_02_03 = GetIniValue("IMMERSIVE_NAMES", "s02_03", "Night");
		settingStore->immersiveName_03_04 = GetIniValue("IMMERSIVE_NAMES", "s03_04", "Night");
		settingStore->immersiveName_04_05 = GetIniValue("IMMERSIVE_NAMES", "s04_05", "Dawn");
		settingStore->immersiveName_05_06 = GetIniValue("IMMERSIVE_NAMES", "s05_06", "Dawn");
		settingStore->immersiveName_06_07 = GetIniValue("IMMERSIVE_NAMES", "s06_07", "Morning");
		settingStore->immersiveName_07_08 = GetIniValue("IMMERSIVE_NAMES", "s07_08", "Morning");
		settingStore->immersiveName_08_09 = GetIniValue("IMMERSIVE_NAMES", "s08_09", "Morning");
		settingStore->immersiveName_09_10 = GetIniValue("IMMERSIVE_NAMES", "s09_10", "Morning");
		settingStore->immersiveName_10_11 = GetIniValue("IMMERSIVE_NAMES", "s10_11", "Midday");
		settingStore->immersiveName_11_12 = GetIniValue("IMMERSIVE_NAMES", "s11_12", "Midday");
		settingStore->immersiveName_12_13 = GetIniValue("IMMERSIVE_NAMES", "s12_13", "Afternoon");
		settingStore->immersiveName_13_14 = GetIniValue("IMMERSIVE_NAMES", "s13_14", "Afternoon");
		settingStore->immersiveName_14_15 = GetIniValue("IMMERSIVE_NAMES", "s14_15", "Afternoon");
		settingStore->immersiveName_15_16 = GetIniValue("IMMERSIVE_NAMES", "s15_16", "Afternoon");
		settingStore->immersiveName_16_17 = GetIniValue("IMMERSIVE_NAMES", "s16_17", "Afternoon");
		settingStore->immersiveName_17_18 = GetIniValue("IMMERSIVE_NAMES", "s17_18", "Evening");
		settingStore->immersiveName_18_19 = GetIniValue("IMMERSIVE_NAMES", "s18_19", "Evening");
		settingStore->immersiveName_19_20 = GetIniValue("IMMERSIVE_NAMES", "s19_20", "Evening");
		settingStore->immersiveName_20_21 = GetIniValue("IMMERSIVE_NAMES", "s20_21", "Evening");
		settingStore->immersiveName_21_22 = GetIniValue("IMMERSIVE_NAMES", "s21_22", "Night");
		settingStore->immersiveName_22_23 = GetIniValue("IMMERSIVE_NAMES", "s22_23", "Night");
		settingStore->immersiveName_23_24 = GetIniValue("IMMERSIVE_NAMES", "s23_24", "Night");
	}
}
