#include "Hooks.h"
#include "Formatter.h"
#include "Settings.h"

namespace TimeFormatChanger
{
	void FormatTime_Hook(RE::Calendar* a_calendar, char* a_str, std::uint64_t a_bufferSize, bool a_showYear)
	{
		Settings* settings = Settings::GetSingleton();

		Formatter formatter{};
		formatter.Populate(a_calendar);

		std::string formattedString;

		if (a_showYear)
		{
			formattedString = formatter.GetFormattedTime(settings->format);
		}
		else
		{
			formattedString = formatter.GetFormattedTime(settings->formatNoYear);
		}

		strcpy_s(a_str, a_bufferSize, formattedString.c_str());
	}

	void InstallHook()
	{
		SKSE::GetTrampoline().write_branch<5>(REL::ID{ 36311 }.address(), (std::uintptr_t)FormatTime_Hook);
	}
}
