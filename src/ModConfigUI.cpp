#include "ModConfigUI.h"

#include "Formatter.h"
#include "ImmersiveNames.h"
#include "Settings.h"
#include "Version.h"

#include <ModConfigUI/Localization.h>

namespace TimeFormatChanger
{
	const char* Translate(const char* a_key)
	{
		return ModConfigUI::Localization::Get(a_key);
	}

	// Immersive names override fields
	constexpr std::size_t FORMAT_FIELD_SIZE = 256;
	constexpr std::size_t IMMERSIVE_NAME_FIELD_SIZE = 64;

	char g_formatField[FORMAT_FIELD_SIZE]{};
	char g_formatNoYearField[FORMAT_FIELD_SIZE]{};
	char g_immersiveNameFields[24][IMMERSIVE_NAME_FIELD_SIZE]{};
	bool g_fieldsLoaded = false;

	void LoadSettingsIntoFields()
	{
		Settings* settings = Settings::GetSingleton();

		strncpy_s(g_formatField, FORMAT_FIELD_SIZE, settings->format.c_str(), _TRUNCATE);
		strncpy_s(g_formatNoYearField, FORMAT_FIELD_SIZE, settings->formatNoYear.c_str(), _TRUNCATE);

		for (std::int32_t i = 0; i < 24; ++i)
		{
			strncpy_s(g_immersiveNameFields[i], IMMERSIVE_NAME_FIELD_SIZE, settings->immersiveNameOverrides[i].c_str(), _TRUNCATE);
		}

		g_fieldsLoaded = true;
	}

	void EnsureFieldsLoaded()
	{
		if (!g_fieldsLoaded)
		{
			LoadSettingsIntoFields();
		}
	}

	// Presets
	struct Preset
	{
		const char* nameKey;
		const char* format;
		const char* formatNoYear;
		bool leadingZeroHour;
		bool leadingZeroDay;
		bool leadingZeroMonth;
	};

	constexpr Preset PRESETS[] = {
		{ "$TFC_Preset_Custom", nullptr, nullptr, false, false, false },
		{ "$TFC_Preset_Vanilla", "%D, %h:%m %a, %d%t%o%L, %E %y", "%D, %h:%m %a, %d%t%o%L", false, false, false },
		{ "$TFC_Preset_Vanilla24h", "%D, %H:%m, %d%t%o%L, %E %y", "%D, %H:%m, %d%t%o%L", false, false, false },
		{ "$TFC_Preset_AltDate12h", "%D, %h:%m %a, %L %d%t, %E %y", "%D, %h:%m %a, %L %d%t", false, false, false },
		{ "$TFC_Preset_AltDate24h", "%D, %H:%m, %L %d%t, %E %y", "%D, %H:%m, %L %d%t", false, false, false },
		{ "$TFC_Preset_AltDateImmersive", "%D %i, %L %d%t, %E %y", "%D %i, %L %d%t", false, false, false },
		{ "$TFC_Preset_ShortDate12h", "%D, %h:%m %a, %M.%d, %E %y", "%D, %h:%m %a, %M.%d", false, true, true },
		{ "$TFC_Preset_ShortDate24h", "%D, %H:%m, %M.%d, %E %y", "%D, %H:%m, %M.%d", false, true, true },
		{ "$TFC_Preset_ShortDateImmersive", "%D %i, %M.%d, %E %y", "%D %i, %M.%d", false, true, true },
		{ "$TFC_Preset_FullyImmersive", "%i", "%i", false, false, false },
	};
	constexpr std::int32_t PRESET_COUNT = static_cast<std::int32_t>(std::size(PRESETS));

	constexpr std::int32_t PRESET_CUSTOM_INDEX = 0;  // Index of the "Custom" sentinel preset

	std::int32_t GetMatchingPreset()
	{
		Settings* settings = Settings::GetSingleton();

		for (std::int32_t i = PRESET_CUSTOM_INDEX + 1; i < PRESET_COUNT; ++i)
		{
			if (settings->format == PRESETS[i].format &&
				settings->formatNoYear == PRESETS[i].formatNoYear &&
				settings->leadingZeroHour == PRESETS[i].leadingZeroHour &&
				settings->leadingZeroDay == PRESETS[i].leadingZeroDay &&
				settings->leadingZeroMonth == PRESETS[i].leadingZeroMonth)
			{
				return i;
			}
		}
		return PRESET_CUSTOM_INDEX;
	}

	void ApplyPreset(const Preset& a_preset)
	{
		Settings* settings = Settings::GetSingleton();

		settings->format = a_preset.format;
		settings->formatNoYear = a_preset.formatNoYear;
		settings->leadingZeroHour = a_preset.leadingZeroHour;
		settings->leadingZeroDay = a_preset.leadingZeroDay;
		settings->leadingZeroMonth = a_preset.leadingZeroMonth;
	}

	const char* const* GetPresetNames()
	{
		static std::vector<const char*> names = []() {
			std::vector<const char*> result;
			result.reserve(PRESET_COUNT);
			for (const Preset& preset : PRESETS)
			{
				result.push_back(Translate(preset.nameKey));
			}
			return result;
		}();
		return names.data();
	}

	// Previews
	bool BuildPreview(std::string& a_format, std::string& a_formatNoYear)
	{
		RE::Calendar* calendar = RE::Calendar::GetSingleton();
		if (!calendar || !RE::GameSettingCollection::GetSingleton())
		{
			return false;
		}

		Settings* settings = Settings::GetSingleton();

		Formatter formatter{};
		formatter.Populate(calendar);

		a_format = formatter.GetFormattedTime(settings->format);
		a_formatNoYear = formatter.GetFormattedTime(settings->formatNoYear);
		return true;
	}

	// The menu shows the settings in its own fields, so they have to be reloaded after a reset.
	void RestoreDefaultsAndReloadFields()
	{
		RestoreDefaults();
		LoadSettingsIntoFields();
	}

	// Pages
	void DrawFormatPage(ModConfigUI::Renderer& a_renderer)
	{
		EnsureFieldsLoaded();

		Settings* settings = Settings::GetSingleton();

		a_renderer.SeparatorText(Translate("$TFC_Section_Preset"));

		std::int32_t selection = GetMatchingPreset();

		if (a_renderer.Combo(Translate("$TFC_Preset"), &selection, GetPresetNames(), PRESET_COUNT) && PRESETS[selection].format)
		{
			ApplyPreset(PRESETS[selection]);
			SaveSettings();
			LoadSettingsIntoFields();
		}
		a_renderer.ItemTooltip(Translate("$TFC_Preset_Tooltip"));

		a_renderer.SeparatorText(Translate("$TFC_Section_FormatSettings"));

		if (a_renderer.InputText(Translate("$TFC_Format"), g_formatField, FORMAT_FIELD_SIZE, FORMAT_DEFAULT_VALUE, Translate("$TFC_Format_Tooltip")))
		{
			settings->format = g_formatField;
			SaveSettings();
		}

		if (a_renderer.InputText(Translate("$TFC_FormatNoYear"), g_formatNoYearField, FORMAT_FIELD_SIZE, FORMAT_NO_YEAR_DEFAULT_VALUE, Translate("$TFC_FormatNoYear_Tooltip")))
		{
			settings->formatNoYear = g_formatNoYearField;
			SaveSettings();
		}

		if (a_renderer.Checkbox(Translate("$TFC_LeadingZeroHour"), &settings->leadingZeroHour, LEADING_ZERO_HOUR_DEFAULT_VALUE, Translate("$TFC_LeadingZeroHour_Tooltip")))
		{
			SaveSettings();
		}

		if (a_renderer.Checkbox(Translate("$TFC_LeadingZeroDay"), &settings->leadingZeroDay, LEADING_ZERO_DAY_DEFAULT_VALUE, Translate("$TFC_LeadingZeroDay_Tooltip")))
		{
			SaveSettings();
		}

		if (a_renderer.Checkbox(Translate("$TFC_LeadingZeroMonth"), &settings->leadingZeroMonth, LEADING_ZERO_MONTH_DEFAULT_VALUE, Translate("$TFC_LeadingZeroMonth_Tooltip")))
		{
			SaveSettings();
		}

		a_renderer.SeparatorText(Translate("$TFC_Section_Preview"));

		std::string preview;
		std::string previewNoYear;
		if (BuildPreview(preview, previewNoYear))
		{
			// Same labels the fields above use, so the two sections line up.
			a_renderer.LabeledText(Translate("$TFC_Format"), preview.c_str());
			a_renderer.LabeledText(Translate("$TFC_FormatNoYear"), previewNoYear.c_str());
		}
		else
		{
			a_renderer.TextDisabled(Translate("$TFC_Preview_Unavailable"));
		}

		a_renderer.SeparatorText(Translate("$TFC_Section_FormatHints"));
		a_renderer.TextWrappedMuted(Translate("$TFC_FormatHints"));
	}

	void DrawImmersiveNamesPage(ModConfigUI::Renderer& a_renderer)
	{
		EnsureFieldsLoaded();

		Settings* settings = Settings::GetSingleton();

		a_renderer.TextWrappedMuted(Translate("$TFC_ImmersiveNames_Description"));

		a_renderer.SeparatorText(Translate("$TFC_Section_ImmersiveNames"));

		for (std::int32_t i = 0; i < 24; ++i)
		{
			std::string label = std::format("{:02d}:00 - {:02d}:00", i, i + 1);

			// The stored default is an empty override, which stands for the built in name, so that is the one named.
			const char* defaultName = GetDefaultImmersiveName(i);
			if (a_renderer.InputTextWithHint(label.c_str(), defaultName, g_immersiveNameFields[i], IMMERSIVE_NAME_FIELD_SIZE, defaultName, nullptr))
			{
				settings->immersiveNameOverrides[i] = g_immersiveNameFields[i];
				SaveSettings();
			}
		}
	}

	void InstallModConfigUI()
	{
		static constexpr ModConfigUI::ModInfo MOD_INFO{
			.pluginName = Version::NAME.data(),
			.displayName = Version::FORMATTED_NAME.data(),
			.version = Version::STRING.data(),
			.author = Version::AUTHOR.data(),
			.description = "$TFC_Description",
			.nexusUrl = "https://www.nexusmods.com/skyrimspecialedition/mods/28921",
			.sourceUrl = "https://github.com/Vermunds/TimeFormatChanger-SE"
		};

		static constexpr ModConfigUI::Page PAGES[] = {
			{ "$TFC_Page_Format", &DrawFormatPage },
			{ "$TFC_Page_ImmersiveNames", &DrawImmersiveNamesPage }
		};

		ModConfigUI::Install(MOD_INFO, PAGES, &RestoreDefaultsAndReloadFields);
	}
}
