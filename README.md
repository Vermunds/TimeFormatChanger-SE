# Time Format Changer
[![Nexus Mods](https://img.shields.io/badge/NexusMods-Download-orange)](https://www.nexusmods.com/skyrimspecialedition/mods/28921)
[![GitHub release](https://img.shields.io/github/v/release/Vermunds/TimeFormatChanger-SE)](https://github.com/Vermunds/TimeFormatChanger-SE/releases)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](./LICENSE.md)

A mod for The Elder Scrolls V: Skyrim - Special Edition.

Allows you to change the time and date format used by the game to anything you want.

The mod is released as a FOMOD installer.

## Features
- Change between 12 and 24 hour time formats
- Change between different date formats
- Hide certain parts of the time or date
- Replace the in-game clock with an "immersive" one. Instead of showing you the precise time, it will show you "Afternoon" or "Night"

The mod comes with some presets, to make it easier to use. However, if you want to create your own format, you can change it in the provided .ini file. The mod is highly customizable.
If you want to use the "immersive" clock, you can also change the names of the parts of the day on a hourly basis.

## INI configuration
The configuration file is located at:

`<Skyrim install folder>/Data/SKSE/Plugins/TimeFormatChanger.ini`

The configuration file is fully commented for clarity. Lines starting with # are comments and have no effect in-game.
Delete the .ini file and launch the game once to regenerate the defaults in case you lost them.
Note that some mod managers may place this file in different locations than the one above. In this case, refer to the documentation of your mod manager of choice.

## Custom format
Use these tags to create a custom format. These tags will be replaced with the actual values in-game:

- `%D` - Day of week
- `%h` - Hours (in 12h format)
- `%H` - Hours (in 24h format)
- `%m` - Minutes
- `%a` - AM/PM
- `%d` - Day of month
- `%t` - Suffix after the day (In the english version of the game: "st", "nd", "rd" or "th")
- `%o` - " of " (used between the date and the month: 17th of ..., contains spaces)
- `%M` - Month of year (as a number)
- `%L` - Month of year (full name)
- `%E` - Era (always 4E)
- `%y` - Year
- `%i` - Immersive time of day

Example (vanilla format): `%D, %h:%m %a, %d%t%o%M, %E %y`  
will turn into: `Turdas, 1:13 PM, 21st of Last Seed, 4E 201`

Warning: too long strings may get trimmed (unless you use a mod to fix this issue). The actual character limit depends on the content. It is recommended to try the longest possible date before use.
You can force display any date if you change the format code to that specific date, like this:

`sTimeFormat="Loredas, 11:13 PM, 21st of Morning Star, 4E 201"`

This will always display "Loredas, 11:13 PM, 21st of Morning Star, 4E 201" regardless of the actual in-game time.

To view/change the immersive time of day values, refer to the .ini configuration file.

## Download
Available on [Nexusmods](https://www.nexusmods.com/skyrimspecialedition/mods/28921).

## Build
To build this mod refer to my wrapper project [here](https://github.com/Vermunds/SkyrimSE-Mods).

## License
This software is available under the MIT License. See LICENSE.md for details.
