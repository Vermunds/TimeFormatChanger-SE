#pragma once

#include "RE/TESGlobal.h"

namespace Hooks
{
	class GameTimeHolder
	{
	public:
		void* unk00;			    // 00
		RE::TESGlobal* year;	    // 08
		RE::TESGlobal* month;	    // 10
		RE::TESGlobal* day;		    // 18
		RE::TESGlobal* hour;	    // 20
		RE::TESGlobal* weekday;	    // 28
		RE::TESGlobal* timescale;   // 30 
		UInt32 gameDaysPassedInt;   // 38
		float gameDaysPassedFloat;  // 3C

		//possibly more members...
	};

	void InstallHook();
}
