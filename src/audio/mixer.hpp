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

#ifndef SUPERTUX_SRC_AUDIO_MIXER_HPP
#define SUPERTUX_SRC_AUDIO_MIXER_HPP

#include <memory>

#include "types.hpp"

class MAException : public std::runtime_error
{
public:
	MAException(const std::string &what, i32 result);
};

struct ma_engine;
struct ma_sound;
class Mixer
{
	friend class SoundManager;

public:
	static std::string get_channels_name(u32 channels);

public:
	Mixer();
	~Mixer() = default;

	void shutdown();
	void play_sound(const std::string& filename);
	void play_sound(ma_sound* sound);
	void play_music(std::string filename);
	bool is_playing_music();
	void stop_playing_music();

	ma_engine* engine();

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

extern Mixer g_mixer;

#endif
