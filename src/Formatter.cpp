#include "Formatter.h"

namespace TimeFormatChanger
{
	std::string Formatter::GetFormattedTime(std::string a_format)
	{
		size_t pos = 0;
		while ((pos = a_format.find('%', pos)) != std::string::npos)
		{
			if (pos + 1 < a_format.length())
			{
				switch (a_format[pos + 1])
				{
				case 'D':
					a_format.replace(pos, 2, this->weekday);
					break;
				case 'h':
					a_format.replace(pos, 2, this->hours12);
					break;
				case 'H':
					a_format.replace(pos, 2, this->hours24);
					break;
				case 'm':
					a_format.replace(pos, 2, this->minutes);
					break;
				case 'a':
					a_format.replace(pos, 2, this->am_pm);
					break;
				case 'd':
					a_format.replace(pos, 2, this->day);
					break;
				case 't':
					a_format.replace(pos, 2, this->daySuffix);
					break;
				case 'o':
					a_format.replace(pos, 2, this->of);
					break;
				case 'M':
					a_format.replace(pos, 2, this->month);
					break;
				case 'L':
					a_format.replace(pos, 2, this->longMonth);
					break;
				case 'E':
					a_format.replace(pos, 2, this->era);
					break;
				case 'y':
					a_format.replace(pos, 2, this->year);
					break;
				case 'i':
					a_format.replace(pos, 2, this->immersiveName);
					break;
				default:
					// Unknown format specifier
					break;
				}
			}
			pos++;
		}
		return a_format;
	}
}
