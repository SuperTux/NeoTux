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
#include "music_reader.hpp"

MusicReader::MusicReader()
{

}

MusicData MusicReader::open(const std::string& filename)
{
	SexpElt root;
	MusicData out;

	root = m_parser.read_file(filename);

	if (!root.is_list())
		return {};

	root = root.get_list();
	if (root.get_value() == "supertux-music")
		Logger::debug("If it walks like a supertux-music and quacks like a supertux-music...");

	SexpElt elt;
	elt = root.find_car("file");
	if (!elt.is_valid())
		Logger::fatal("MusicReader", "File not specified");
	out.file = elt.next().get_value();

	elt = root.find_car("loop-begin");
	if (elt.is_valid())
		out.loop_begin = elt.next().get_int_or(out.loop_begin);

	elt = root.find_car("loop-at");
	if (elt.is_valid())
		out.loop_at = elt.next().get_int_or(out.loop_at);

	return out;
}
