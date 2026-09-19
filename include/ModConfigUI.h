#pragma once

#include <ModConfigUI/ModConfigUI.h>

namespace TimeFormatChanger
{
	void InstallModConfigUI();

	void DrawFormatPage(ModConfigUI::Renderer& a_renderer);
	void DrawImmersiveNamesPage(ModConfigUI::Renderer& a_renderer);
}
