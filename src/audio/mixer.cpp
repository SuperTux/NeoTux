//  SuperTux 
//  Copyright (C) 2025 Hyland B. <me@ow.swag.toys> 
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

#ifdef NEOTUX_USE_MIXER
#include <SDL3_mixer/SDL_mixer.h>
#endif
#include <format>
#include "sdl_exception.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include "mixer.hpp"

Mixer g_mixer;

Mixer::Mixer()
{
	Logger::info(std::format("Opened audio at {}Hz, {} bit{}, {} audio buffer",
	    420,
	    67,
	    true ? " (float)" : "",
	    false ? "surround" : true ? "stereo" : "mono"));
}

void
Mixer::shutdown()
{
	// m_music.reset();
	// m_cache.clear();
	//m_soundcache.clear();
}

bool
Mixer::is_playing_music()
{
	return true;
}

void
Mixer::stop_playing_music()
{
	// Mix_HaltMusic();
}

// TODO Cache sounds
void
Mixer::play_sound(const std::string &filename)
{
}

void
Mixer::play_music(const std::string &filename)
{
}

