#pragma once

namespace TimeFormatChanger
{
	// Format
	constexpr const char* FORMAT_DEFAULT_VALUE = "%D, %H:%m, %d%t%o%L, %E %y";
	constexpr const char* FORMAT_NO_YEAR_DEFAULT_VALUE = "%D, %H:%m, %d%t%o%L";
	constexpr bool LEADING_ZERO_HOUR_DEFAULT_VALUE = false;
	constexpr bool LEADING_ZERO_DAY_DEFAULT_VALUE = false;
	constexpr bool LEADING_ZERO_MONTH_DEFAULT_VALUE = false;

	// Immersive name overrides, shared by every hour. Empty means the built-in name is used.
	constexpr const char* IMMERSIVE_NAME_OVERRIDES_DEFAULT_VALUE = "";

	class Settings
	{
	public:
		std::string format;
		std::string formatNoYear;

		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;

		std::string immersiveNameOverrides[24];

		static Settings* GetSingleton();

	private:
		Settings() {};
		~Settings() {};
		Settings(const Settings&) = delete;
		Settings& operator=(const Settings&) = delete;
	};

	void LoadSettings();
	void SaveSettings();
	void RestoreDefaults();
}
