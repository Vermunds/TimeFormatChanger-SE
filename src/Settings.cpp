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

		settingStore->immersiveNames[0] = GetIniValue("IMMERSIVE_NAMES", "s00_01", "Night");
		settingStore->immersiveNames[1] = GetIniValue("IMMERSIVE_NAMES", "s01_02", "Night");
		settingStore->immersiveNames[2] = GetIniValue("IMMERSIVE_NAMES", "s02_03", "Night");
		settingStore->immersiveNames[3] = GetIniValue("IMMERSIVE_NAMES", "s03_04", "Night");
		settingStore->immersiveNames[4] = GetIniValue("IMMERSIVE_NAMES", "s04_05", "Dawn");
		settingStore->immersiveNames[5] = GetIniValue("IMMERSIVE_NAMES", "s05_06", "Dawn");
		settingStore->immersiveNames[6] = GetIniValue("IMMERSIVE_NAMES", "s06_07", "Morning");
		settingStore->immersiveNames[7] = GetIniValue("IMMERSIVE_NAMES", "s07_08", "Morning");
		settingStore->immersiveNames[8] = GetIniValue("IMMERSIVE_NAMES", "s08_09", "Morning");
		settingStore->immersiveNames[9] = GetIniValue("IMMERSIVE_NAMES", "s09_10", "Morning");
		settingStore->immersiveNames[10] = GetIniValue("IMMERSIVE_NAMES", "s10_11", "Midday");
		settingStore->immersiveNames[11] = GetIniValue("IMMERSIVE_NAMES", "s11_12", "Midday");
		settingStore->immersiveNames[12] = GetIniValue("IMMERSIVE_NAMES", "s12_13", "Afternoon");
		settingStore->immersiveNames[13] = GetIniValue("IMMERSIVE_NAMES", "s13_14", "Afternoon");
		settingStore->immersiveNames[14] = GetIniValue("IMMERSIVE_NAMES", "s14_15", "Afternoon");
		settingStore->immersiveNames[15] = GetIniValue("IMMERSIVE_NAMES", "s15_16", "Afternoon");
		settingStore->immersiveNames[16] = GetIniValue("IMMERSIVE_NAMES", "s16_17", "Afternoon");
		settingStore->immersiveNames[17] = GetIniValue("IMMERSIVE_NAMES", "s17_18", "Evening");
		settingStore->immersiveNames[18] = GetIniValue("IMMERSIVE_NAMES", "s18_19", "Evening");
		settingStore->immersiveNames[19] = GetIniValue("IMMERSIVE_NAMES", "s19_20", "Evening");
		settingStore->immersiveNames[20] = GetIniValue("IMMERSIVE_NAMES", "s20_21", "Evening");
		settingStore->immersiveNames[21] = GetIniValue("IMMERSIVE_NAMES", "s21_22", "Night");
		settingStore->immersiveNames[22] = GetIniValue("IMMERSIVE_NAMES", "s22_23", "Night");
		settingStore->immersiveNames[23] = GetIniValue("IMMERSIVE_NAMES", "s23_24", "Night");
	}
}
