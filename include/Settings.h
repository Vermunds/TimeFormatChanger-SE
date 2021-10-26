#pragma once

#include <string>
#include <vector>

namespace TimeFormatChanger
{
	class Settings
	{
	private:
		static Settings* singleton;
		Settings();

	public:
		std::string format;
		std::string formatNoYear;

		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;

		std::string immersiveNames[24];

		static Settings* GetSingleton();
	};

	extern void LoadSettings();
}
