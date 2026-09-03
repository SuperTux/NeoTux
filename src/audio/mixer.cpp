//  SuperTux
//  Copyright (C) 2026 Hyland B. <me@ow.swag.toys>
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

#include "mixer.hpp"

#include <format>

#include "audio/music_reader.hpp"
#include "audio/sound_manager.hpp"
#include "sdl_exception.hpp"
#include "util/filesystem.hpp"
#include "util/logger.hpp"

struct Mixer::Impl
{
	MIX_Mixer *engine;

	MIX_Track *music_track;
	MusicData music_data;

	/// Not to be confused with music. Each sound has its own SDL3 mixer track.
	std::vector<MIX_Track *> sound_tracks;
};

Mixer g_mixer;

Mixer::Mixer()
    : impl(std::make_unique<Impl>())
{
	if (!MIX_Init())
		throw SDLException("SDL3_mixer initialization failed");

	impl->engine = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
	if (!impl->engine)
		throw SDLException("Mixer object creation failed");

	impl->music_track = MIX_CreateTrack(engine());
	if (!impl->music_track)
		throw SDLException("Music track creation failed");

	// Logger::info("Mixer", "Opened audio device:");
	// Logger::info("Mixer",
	// std::format("\tSample rate: {}Hz", dev_info.nativeDataFormats[0].sampleRate));
	// Logger::info("Mixer", std::format("\tChannels: {} sound ({})",
	// get_channels_name(dev_info.nativeDataFormats[0].channels),
	// dev_info.nativeDataFormats[0].channels));
	// Logger::info("Mixer", std::format("\tFormat: {}",
	// ma_get_format_name(dev_info.nativeDataFormats[0].format)));
}

Mixer::~Mixer()
{
	shutdown();
}

std::string
Mixer::get_channels_name(u32 channels)
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
	if (impl->engine)
		MIX_DestroyMixer(impl->engine);
}

bool
Mixer::is_playing_music()
{
	return MIX_TrackPlaying(impl->music_track);
}

void
Mixer::stop_playing_music()
{
	MIX_StopTrack(impl->music_track, 0);
}

MIX_Mixer *
Mixer::engine()
{
	return impl->engine;
}

std::size_t
Mixer::create_sound_track(MIX_Audio *audio)
{
	MIX_Track *track = MIX_CreateTrack(engine());
	if (!track)
		throw SDLException("Failed to create sound track");

	MIX_SetTrackAudio(track, audio);
	MIX_TagTrack(track, "sounds");
	impl->sound_tracks.push_back(track);
	return impl->sound_tracks.size() - 1;
}

void
Mixer::play_sound(const std::string &filename)
{
	play_sound(g_sound_manager.load(FS::path(filename)));
}

void
Mixer::play_sound(Sound sound)
{
	MIX_Track *const track = impl->sound_tracks[sound.track];
	if (MIX_TrackPlaying(track))
		MIX_StopTrack(track, 0);

	if (!MIX_PlayTrack(track, 0))
		throw SDLException("Failed to play sound track");
}

void
Mixer::play_music(std::string filename)
{
	if (filename.ends_with(".music"))
	{
		MusicReader reader;
		impl->music_data = reader.open(FS::path(filename));
		filename         = FS::join(FS::parent_dir(filename), impl->music_data.file);
	}
	else
	{
		impl->music_data = {};
	}

	// FIXME: doesn't seem like MusicData::file ever worked :/
	MIX_Audio *music = MIX_LoadAudio(engine(), FS::path(filename).c_str(), false);
	if (!music)
		throw SDLException(std::format("Failed to load music '{}'", FS::path(filename)));

	if (!MIX_SetTrackAudio(impl->music_track, music))
		throw SDLException(std::format("Failed to set music track audio '{}'", FS::path(filename)));

	SDL_PropertiesID playprops = SDL_CreateProperties();
	SDL_SetNumberProperty(playprops, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
	SDL_SetNumberProperty(playprops, MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER,
	                      impl->music_data.loop_begin * 1000);
	SDL_SetNumberProperty(playprops, MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER,
	                      (impl->music_data.loop_at == UINT32_MAX)
	                          ? -1
	                          : impl->music_data.loop_at * 1000);
	if (!MIX_PlayTrack(impl->music_track, playprops))
		throw SDLException(std::format("Failed to play music '{}'", FS::path(filename)));

	SDL_DestroyProperties(playprops);
}
