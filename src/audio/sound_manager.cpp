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

#include <string>
#include <unordered_map>

#include "audio/mixer.hpp"
#include "util/filesystem.hpp"

struct SoundManager::Impl
{
	std::unordered_map<std::string, ma_sound> sounds;
};

SoundManager g_sound_manager;

SoundManager::SoundManager()
    : impl(std::make_unique<Impl>())
{
}

ma_sound *
SoundManager::load(const std::string &path)
{
	auto it = impl->sounds.find(path);
	if (it == impl->sounds.end())
	{
		auto new_it   = impl->sounds.insert({ path, {} });
		ma_sound *out = &new_it.first->second;
		ma_sound_init_from_file(g_mixer.engine(), FS::path(path).c_str(),
		                        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, out);
		return out;
	}
	else
	{
		return &it->second;
	}
}
