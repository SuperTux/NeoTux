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

#pragma once
#include <memory>
#include <lua.hpp>

namespace lua
{

class LuaState
{
public:
	LuaState();
	// does not manage memory of m_L
	LuaState(const LuaState& state);
	LuaState(lua_State* L);
	~LuaState() = default;
	
	void openlibs();
	
	lua_State* get_state() const { return m_L.get(); }
private:
	// 
	std::unique_ptr<lua_State, decltype(&lua_close)> m_L;
};

} // namespace lua
