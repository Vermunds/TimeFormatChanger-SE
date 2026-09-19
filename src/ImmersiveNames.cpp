#include "ImmersiveNames.h"
#include "Settings.h"

#include <ModConfigUI/Localization.h>

namespace
{
	static constexpr std::array<const char*, 24> IMMERSIVE_NAME_KEYS = {
		"$TFC_ImmersiveName_00", "$TFC_ImmersiveName_01", "$TFC_ImmersiveName_02", "$TFC_ImmersiveName_03",
		"$TFC_ImmersiveName_04", "$TFC_ImmersiveName_05", "$TFC_ImmersiveName_06", "$TFC_ImmersiveName_07",
		"$TFC_ImmersiveName_08", "$TFC_ImmersiveName_09", "$TFC_ImmersiveName_10", "$TFC_ImmersiveName_11",
		"$TFC_ImmersiveName_12", "$TFC_ImmersiveName_13", "$TFC_ImmersiveName_14", "$TFC_ImmersiveName_15",
		"$TFC_ImmersiveName_16", "$TFC_ImmersiveName_17", "$TFC_ImmersiveName_18", "$TFC_ImmersiveName_19",
		"$TFC_ImmersiveName_20", "$TFC_ImmersiveName_21", "$TFC_ImmersiveName_22", "$TFC_ImmersiveName_23"
	};
}

namespace TimeFormatChanger
{

	const char* GetDefaultImmersiveName(std::int32_t a_hour)
	{
		if (a_hour < 0 || a_hour >= 24)
		{
			return "";
		}

		return ModConfigUI::Localization::Get(IMMERSIVE_NAME_KEYS[a_hour]);
	}

	const char* GetImmersiveName(std::int32_t a_hour)
	{
		if (a_hour < 0 || a_hour >= 24)
		{
			return "";
		}

		const std::string& overrideName = Settings::GetSingleton()->immersiveNameOverrides[a_hour];
		return overrideName.empty() ? GetDefaultImmersiveName(a_hour) : overrideName.c_str();
	}
}
