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

#include <miniaudio_libvorbis.h>

Mixer g_mixer;

Mixer::Mixer()
{
	ma_result result;

	ma_decoding_backend_vtable* decoders[] = {
	    ma_decoding_backend_libvorbis
	};

	ma_resource_manager_config resource_manager_cfg = ma_resource_manager_config_init();
	resource_manager_cfg.pCustomDecodingBackendUserData = nullptr;
	resource_manager_cfg.ppCustomDecodingBackendVTables = decoders;
	resource_manager_cfg.customDecodingBackendCount = sizeof(decoders) / sizeof(decoders[0]);

	result = ma_resource_manager_init(&resource_manager_cfg, &m_resource_manager);
	if (result != MA_SUCCESS)
	{
		Logger::error("Mixer", "Oops! ded.");
		std::exit(-1);
	}

	ma_engine_config engine_cfg;
	engine_cfg = ma_engine_config_init();
	engine_cfg.pResourceManager = &m_resource_manager;

	result = ma_engine_init(&engine_cfg, &m_engine);
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
	ma_sound_uninit(&m_music);
	ma_engine_uninit(&m_engine);
	ma_resource_manager_uninit(&m_resource_manager);
	// m_music.reset();
	// m_cache.clear();
	//m_soundcache.clear();
}

bool
Mixer::is_playing_music()
{
	return ma_sound_is_playing(&m_music);
}

void
Mixer::stop_playing_music()
{
	ma_sound_stop(&m_music);
}

void
Mixer::play_sound(const std::string &filename)
{
	ma_result result = ma_engine_play_sound(&m_engine, FS::path(filename).c_str(), nullptr);
	if (result != MA_SUCCESS) {
		throw std::runtime_error(std::format("Failed to load/play sound {} (ma error: {})",
		                                     FS::path(filename),
		                                     (int)result));
	}
}

void
Mixer::play_music(std::string filename)
{
	if (filename.ends_with(".music"))
	{
		MusicReader reader;
		m_music_data = reader.open(FS::path(filename));
		filename = FS::join(FS::parent_dir(filename), m_music_data.file);
	} else {
		m_music_data = {};
	}

	ma_result result;
	result = ma_sound_init_from_file(&m_engine, FS::path(filename).c_str(),
	                                 0, nullptr, nullptr, &m_music);

	if (result != MA_SUCCESS) {
		throw std::runtime_error(std::format("Failed to load music {} (ma error: {})",
		                                     FS::path(filename),
		                                     (int)result));
	}

	ma_data_source* music_source = ma_sound_get_data_source(&m_music);
	ma_uint32 samplerate;
	ma_data_source_get_data_format(music_source, nullptr, nullptr, &samplerate, nullptr, 67);

	ma_data_source_set_loop_point_in_pcm_frames(music_source, m_music_data.loop_begin * samplerate,
	                                            m_music_data.loop_at * samplerate);
	ma_sound_set_looping(&m_music, MA_TRUE);

	ma_sound_start(&m_music);
}

