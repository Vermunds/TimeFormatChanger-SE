#pragma once

#include <string>
#include <vector>

namespace TimeFormatChanger
{
	class Settings
	{
	public:
		std::string format;
		std::string formatNoYear;

		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;

		std::string immersiveNames[24];

		static Settings* GetSingleton();

	private:
		Settings(){};
		~Settings(){};
		Settings(const Settings&) = delete;
		Settings& operator=(const Settings&) = delete;
	};

	extern void LoadSettings();
}
