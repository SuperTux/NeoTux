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

#include <format>
#include <miniaudio.h>
#include <miniaudio_libvorbis.h>

#include "mixer.hpp"
#include "sdl_exception.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"
#include "audio/sound_manager.hpp"
#include "audio/music_reader.hpp"

MAException::MAException(const std::string& what, int result) :
    std::runtime_error(std::format("{} (ma error: {})", what, result))
{}

struct Mixer::Impl
{
	ma_engine engine;
	ma_resource_manager resource_manager;

	ma_sound music;
	MusicData music_data;
};

Mixer g_mixer;

Mixer::Mixer():
    impl(std::make_unique<Impl>())
{
	ma_result result;

	ma_decoding_backend_vtable* decoders[] = {
	    ma_decoding_backend_libvorbis
	};

	ma_resource_manager_config resource_manager_cfg = ma_resource_manager_config_init();
	resource_manager_cfg.pCustomDecodingBackendUserData = nullptr;
	resource_manager_cfg.ppCustomDecodingBackendVTables = decoders;
	resource_manager_cfg.customDecodingBackendCount = sizeof(decoders) / sizeof(decoders[0]);

	result = ma_resource_manager_init(&resource_manager_cfg, &impl->resource_manager);
	if (result != MA_SUCCESS)
	{
		throw MAException("Failed to initialize resource manager", result);
	}

	ma_engine_config engine_cfg;
	engine_cfg = ma_engine_config_init();
	engine_cfg.pResourceManager = &impl->resource_manager;

	result = ma_engine_init(&engine_cfg, &impl->engine);
	if (result != MA_SUCCESS)
	{
		throw MAException("Failed to initialize engine", result);
	}

	ma_device* dev;
	ma_device_info dev_info;
	dev = ma_engine_get_device(&impl->engine);
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
	ma_sound_uninit(&impl->music);
	ma_engine_uninit(&impl->engine);
	ma_resource_manager_uninit(&impl->resource_manager);
}

bool
Mixer::is_playing_music()
{
	return ma_sound_is_playing(&impl->music);
}

void
Mixer::stop_playing_music()
{
	ma_sound_stop(&impl->music);
}

ma_engine*
Mixer::engine()
{
	return &impl->engine;
}

void
Mixer::play_sound(const std::string &filename)
{
	ma_sound* sound = g_sound_manager.load(filename);
	play_sound(sound);
}

void
Mixer::play_sound(ma_sound* sound)
{
	if (ma_sound_is_playing(sound))
		ma_sound_stop(sound);

	ma_result result = ma_sound_start(sound);
	if (result != MA_SUCCESS) {
		throw MAException("Failed to play sound", result);
	}
}

void
Mixer::play_music(std::string filename)
{
	if (filename.ends_with(".music"))
	{
		MusicReader reader;
		impl->music_data = reader.open(FS::path(filename));
		filename = FS::join(FS::parent_dir(filename), impl->music_data.file);
	} else {
		impl->music_data = {};
	}

	ma_result result;
	ma_sound_uninit(&impl->music);
	result = ma_sound_init_from_file(&impl->engine, FS::path(filename).c_str(),
	                                 MA_SOUND_FLAG_STREAM, nullptr, nullptr, &impl->music);

	if (result != MA_SUCCESS) {
		throw MAException(std::format("Failed to load music {}", FS::path(filename)), result);
	}

	ma_data_source* music_source = ma_sound_get_data_source(&impl->music);
	ma_uint32 samplerate;
	ma_data_source_get_data_format(music_source, nullptr, nullptr, &samplerate, nullptr, 67);

	ma_data_source_set_loop_point_in_pcm_frames(music_source, impl->music_data.loop_begin * samplerate,
	                                            impl->music_data.loop_at * samplerate);
	ma_sound_set_looping(&impl->music, MA_TRUE);

	result = ma_sound_start(&impl->music);
	if (result != MA_SUCCESS) {
		throw MAException(std::format("Failed to play music {}", FS::path(filename)), result);
	}

}
