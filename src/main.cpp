#include "skse64_common/BranchTrampoline.h" 
#include "skse64_common/skse_version.h"
#include "skse64/PluginAPI.h"

#include "version.h"
#include "Settings.h"
#include "Hooks.h"

static PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
static SKSEMessagingInterface* g_messaging = 0;

extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\TimeFormatChanger.log");
		SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::UseLogStamp(true);

		_MESSAGE("TimeFormatChanger v%s", TIMEFORMATCHANGER_VERSION_VERSTRING);

		a_info->infoVersion = PluginInfo::kInfoVersion;
		a_info->name = "TimeFormatChanger";
		a_info->version = 1;

		g_pluginHandle = a_skse->GetPluginHandle();

		if (a_skse->isEditor) {
			_FATALERROR("Loaded in editor, marking as incompatible!\n");
			return false;
		}

		if ((a_skse->runtimeVersion != RUNTIME_VERSION_1_5_97) && (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_80) && (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_73)) {
			_FATALERROR("Unsupported runtime version %08X!\n", a_skse->runtimeVersion);
			return false;
		}

		if (g_branchTrampoline.Create(1024 * 8)) {
			_MESSAGE("Branch trampoline creation successful");
		} else {
			_FATALERROR("Branch trampoline creation failed!\n");
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* a_skse)
	{
		_MESSAGE("TimeFormatChanger loaded");

		TimeFormatChanger::LoadSettings();
		_MESSAGE("Settings successfully loaded.");

		Hooks::InstallHook();

		return true;
	}
};
