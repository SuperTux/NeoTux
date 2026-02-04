//  SuperTux
//  Copyright (C) 2025 MatusGuy <martusguy@proton.me>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef SUPERTUX_SRC_AUDIO_SOUND_MANAGER
#define SUPERTUX_SRC_AUDIO_SOUND_MANAGER

#include <miniaudio.h>

#include <memory>

class SoundManager
{
public:
	SoundManager();

	ma_sound *load(const std::string &path);

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

extern SoundManager g_sound_manager;

#endif  // SUPERTUX_SRC_AUDIO_SOUND_MANAGER
