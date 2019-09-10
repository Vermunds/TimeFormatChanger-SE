#include "Formatter.h"

namespace TimeFormatChanger
{
	std::string Formatter::GetFormattedTime(std::string a_format)
	{
		size_t pos;
		pos = a_format.find("%D");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->weekday);
		}
		pos = a_format.find("%h");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->hours12);
		}
		pos = a_format.find("%H");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->hours24);
		}
		pos = a_format.find("%m");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->minutes);
		}
		pos = a_format.find("%a");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->am_pm);
		}
		pos = a_format.find("%d");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->day);
		}
		pos = a_format.find("%t");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->daySuffix);
		}
		pos = a_format.find("%o");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->of);
		}
		pos = a_format.find("%M");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->month);
		}
		pos = a_format.find("%L");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->longMonth);
		}
		pos = a_format.find("%E");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->era);
		}
		pos = a_format.find("%y");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->year);
		}
		pos = a_format.find("%i");
		if (!(pos == SIZE_MAX))
		{
			a_format.replace(pos, 2, this->immersiveName);
		}
		return a_format;
	}
}
