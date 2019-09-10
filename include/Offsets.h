#pragma once
#include "skse64_common/Relocation.h"

namespace Offsets {

	const RelocAddr<uintptr_t*> FormatTime_Hook = 0x5A6640;

	const RelocAddr<uintptr_t*> GetDaySuffix = 0x5A6FD0;

	const RelocAddr<uintptr_t*> of_str = 0x1DE8DE8;
	const RelocAddr<uintptr_t*> AM_str = 0x1DB40F0;
	const RelocAddr<uintptr_t*> PM_str = 0x1DB4108;

	const RelocAddr<uintptr_t*> MonthNameHolder = 0x1DB3F00;
	const RelocAddr<uintptr_t*> WeekDayNameHolder = 0x1DB3F60;
}
