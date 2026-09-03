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

#include "sound_manager.hpp"

#include <SDL3_mixer/SDL_mixer.h>

#include <string>
#include <unordered_map>

#include "audio/mixer.hpp"
#include "util/filesystem.hpp"

struct SoundManager::Impl
{
	std::unordered_map<std::string, Sound> sounds;
};

SoundManager g_sound_manager;

SoundManager::SoundManager()
    : impl(std::make_unique<Impl>())
{
}

Sound
SoundManager::load(const std::string &path)
{
	auto it = impl->sounds.find(path);
	if (it == impl->sounds.end())
	{
		Sound out;
		out.audio = MIX_LoadAudio(g_mixer.engine(), path.c_str(), true);
		out.track = g_mixer.create_sound_track(out.audio);
		impl->sounds.insert({ path, out });
		return out;
	}
	else
	{
		return it->second;
	}
}
