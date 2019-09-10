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
		const char* format;
		const char* formatNoYear;

		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;

		const char* immersiveName_00_01;
		const char* immersiveName_01_02;
		const char* immersiveName_02_03;
		const char* immersiveName_03_04;
		const char* immersiveName_04_05;
		const char* immersiveName_05_06;
		const char* immersiveName_06_07;
		const char* immersiveName_07_08;
		const char* immersiveName_08_09;
		const char* immersiveName_09_10;
		const char* immersiveName_10_11;
		const char* immersiveName_11_12;
		const char* immersiveName_12_13;
		const char* immersiveName_13_14;
		const char* immersiveName_14_15;
		const char* immersiveName_15_16;
		const char* immersiveName_16_17;
		const char* immersiveName_17_18;
		const char* immersiveName_18_19;
		const char* immersiveName_19_20;
		const char* immersiveName_20_21;
		const char* immersiveName_21_22;
		const char* immersiveName_22_23;
		const char* immersiveName_23_24;

		static SettingStore* GetSingleton();
	};

	extern void LoadSettings();
}
