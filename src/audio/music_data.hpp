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
#ifndef SUPERTUX_SRC_AUDIO_MUSIC_DATA_HPP
#define SUPERTUX_SRC_AUDIO_MUSIC_DATA_HPP

#include <string>

#include "types.hpp"

struct MusicData
{
	std::string file;

	// TODO: I don't think having loop points determined in seconds allows for enough granularity...
	// perhaps support another option in the music file format for milliseconds ("loop-begin-ms")
	// or something even more granular like straight up sample frames to keep backwards compat.
	u32 loop_begin = 0;
	u32 loop_at    = UINT_MAX;
};

#endif  // SUPERTUX_SRC_AUDIO_MUSIC_DATA_HPP
