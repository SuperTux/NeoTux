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

#ifndef SUPERTUX_SRC_AUDIO_MIXER_HPP
#define SUPERTUX_SRC_AUDIO_MIXER_HPP

#include <SDL3_mixer/SDL_mixer.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "audio/sound_manager.hpp"
#include "config.h"
#include "types.hpp"

class Mixer
{
	friend class SoundManager;

public:
	static std::string get_channels_name(u32 channels);

public:
	Mixer();
	~Mixer();

	void shutdown();
	void play_sound(const std::string &filename);
	void play_sound(Sound sound);
	void play_music(std::string filename);
	bool is_playing_music();
	void stop_playing_music();

	MIX_Mixer *engine();

private:
	std::size_t create_sound_track(MIX_Audio *audio);

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

extern Mixer g_mixer;

#endif
