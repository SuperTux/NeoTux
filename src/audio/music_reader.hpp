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
#ifndef SUPERTUX_SRC_AUDIO_MUSIC_READER_HPP
#define SUPERTUX_SRC_AUDIO_MUSIC_READER_HPP

#include "audio/music_data.hpp"
#include "util/logger.hpp"
#include "util/sexp.hpp"

class MusicReader
{
public:
	MusicReader();

	MusicData open(const std::string &filename);

private:
	SexpParser m_parser;
};

#endif
