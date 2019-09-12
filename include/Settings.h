#pragma once

#include <string>
#include <vector>

namespace TimeFormatChanger
{
	class SettingStore
	{
	private:
		static SettingStore * singleton;
		SettingStore();
	public:
		std::string format;
		std::string formatNoYear;

		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;

		std::string immersiveNames[24];

		static SettingStore* GetSingleton();
	};

	extern void LoadSettings();
}
