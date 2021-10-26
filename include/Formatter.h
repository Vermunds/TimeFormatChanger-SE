#pragma once
#include <string>

namespace TimeFormatChanger
{
	class Formatter
	{
	public:
		std::string weekday;        //%D
		std::string hours12;        //%h
		std::string hours24;        //%H
		std::string minutes;        //%m
		std::string am_pm;          //%a
		std::string day;            //%d
		std::string daySuffix;      //%t
		std::string of;             //%o
		std::string month;          //%M
		std::string longMonth;      //%L
		std::string era;            //%E
		std::string year;           //%y
		std::string immersiveName;  //%i

		std::string GetFormattedTime(std::string a_format);
	};
}
