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
	ma_result result;

	result = ma_engine_init(nullptr, &m_engine);
	if (result != MA_SUCCESS)
	{
		Logger::error("Mixer", "Oops! ded.");
		std::exit(-1);
	}

	ma_device* dev;
	ma_device_info dev_info;
	dev = ma_engine_get_device(&m_engine);
	ma_device_get_info(dev, ma_device_type_playback, &dev_info);

	Logger::info("Mixer", "Opened audio device:");
	Logger::info("Mixer", std::format("\tSample rate: {}Hz",
	                                  dev_info.nativeDataFormats[0].sampleRate));
	Logger::info("Mixer", std::format("\tChannels: {} sound ({})",
	                                  get_channels_name(dev_info.nativeDataFormats[0].channels),
	                                  dev_info.nativeDataFormats[0].channels));
	Logger::info("Mixer", std::format("\tFormat: {}",
	                                  ma_get_format_name(dev_info.nativeDataFormats[0].format)));
}

std::string Mixer::get_channels_name(u32 channels)
{
	if (channels > 2)
		return "surround";
	else if (channels > 1)
		return "stereo";
	else
		return "mono";
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

