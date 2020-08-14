#include "SKSE/SKSE.h"

#include "version.h"
#include "Settings.h"
#include "Hooks.h"

#include <winuser.h>
#include <spdlog\include\spdlog\sinks\msvc_sink.h>
#include <spdlog\include\spdlog\sinks\basic_file_sink.h>
#include <spdlog\spdlog.h>
#include <filesystem>

void ShowErrorMessage(std::string a_error)
{
	MessageBoxA(nullptr, a_error.c_str(), "Time Format Changer - Error", MB_OK | MB_ICONERROR | MB_DEFBUTTON2 | MB_SYSTEMMODAL);
}

extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		auto path = SKSE::log::log_directory() / "TimeFormatChanger.log";
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true);
		auto log = std::make_shared<spdlog::logger>("global log", std::move(sink));

		log->set_level(spdlog::level::trace);
		log->flush_on(spdlog::level::trace);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v", spdlog::pattern_time_type::local);

		SKSE::log::info("Time Format Changer v" + std::string(TIMEFORMATCHANGER_VERSION_VERSTRING));

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "Time Format Changer";
		a_info->version = 1;

		if (a_skse->IsEditor()) {
			SKSE::log::critical("Loaded in editor, marking as incompatible!");
			return false;
		}

		if (a_skse->RuntimeVersion() < SKSE::RUNTIME_1_5_3) {
			SKSE::log::critical("Unsupported runtime version " + a_skse->RuntimeVersion().string());
			ShowErrorMessage("Unsupported runtime version " + a_skse->RuntimeVersion().string() + "\n\nPlease update your game to a more recent version to use this mod.");
			return false;
		}

		//Check if Address Library is available
		std::string fileName = "Data/SKSE/Plugins/version-" + a_skse->RuntimeVersion().string() + ".bin";
		if (!std::filesystem::exists(fileName))
		{
			SKSE::log::critical("Address Library for SKSE Plugins not found for current runtime version " + a_skse->RuntimeVersion().string());
			ShowErrorMessage("Address Library for SKSE Plugins not found for current runtime version " + a_skse->RuntimeVersion().string() + "\nThe mod will be disabled.");
			return false;
		}

		if (SKSE::AllocTrampoline(2 << 3))
		{
			SKSE::log::info("Trampoline creation successful.");
		}
		else {
			SKSE::log::critical("Trampoline creation failed!");
			ShowErrorMessage("Trampoline creation failed!\nThe mod will be disabled.");
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(SKSE::LoadInterface* a_skse)
	{
		SKSE::log::info("Time Format Changer loaded.");

		TimeFormatChanger::LoadSettings();
		SKSE::log::info("Settings loaded.");

		TimeFormatChanger::InstallHook();
		SKSE::log::info("Hooks installed.");

		return true;
	}
};
