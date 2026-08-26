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

#include "config.h"

#include <memory>
#include <lua.hpp>

namespace lua
{

class LuaState;
class LuaStateIndex;

class LuaState
{
public:
	friend class LuaStateIndex;
	LuaState();
	// does not manage memory of m_L
	LuaState(const LuaState& state);
	LuaState(lua_State* L);
	~LuaState() = default;

	void openlibs();

#define GEN_LUAFUNC(_ret, _name, ...) _ret _name (__VA_ARGS__);
#include "gen_luafunc_data.hpp"

	lua_State* get_state() const { return m_L.get(); }

private:
	std::unique_ptr<lua_State, decltype(&lua_close)> m_L;
};

class LuaStateIndex : public LuaState
{
	friend class LuaState;

public:
	~LuaStateIndex() = default;

private:
	LuaStateIndex(int index, lua_State* L)
		: LuaState(L)
		, m_index(index)
	{}

public:
	inline int get_index() { return m_index; }

	inline bool isboolean()       { return LuaState::isboolean(get_index()); }
	inline bool iscfunction()     { return LuaState::iscfunction(get_index()); }
	inline bool isfunction()      { return LuaState::isfunction(get_index()); }
	inline bool isinteger()       { return LuaState::isinteger(get_index()); }
	inline bool islightuserdata() { return LuaState::islightuserdata(get_index()); }
	inline bool isnil()           { return LuaState::isnil(get_index()); }
	inline bool isnone()          { return LuaState::isnone(get_index()); }
	inline bool isnoneornil()     { return LuaState::isnoneornil(get_index()); }
	inline bool isnumber()        { return LuaState::isnumber(get_index()); }
	inline bool isstring()        { return LuaState::isstring(get_index()); }
	inline bool istable()         { return LuaState::istable(get_index()); }
	inline bool isthread()        { return LuaState::isthread(get_index()); }
	inline bool isuserdata()      { return LuaState::isuserdata(get_index()); }

	inline bool toboolean()        { return LuaState::toboolean(get_index()); }
	inline lua_CFunction tocfunction()  { return LuaState::tocfunction(get_index()); }
	inline lua_Integer tointeger() { return LuaState::tointeger(get_index()); }
	inline lua_Number tonumber()   { return LuaState::tonumber(get_index()); }
	// TODO: wrap these
	inline const char* tostring()  { return LuaState::tostring(get_index()); }
	inline lua_State* tothread()   { return LuaState::tothread(get_index()); }
	// END TODO
	inline void* touserdata()      { return LuaState::touserdata(get_index()); }

	//void len() { lua_len(m_L.get(), get_index()); }

private:
	int m_index;
};

#define GEN_LUAFUNC(_ret, _name, ...) inline _ret LuaState::_name (__VA_ARGS__)
#define GEN_BODY
#include "gen_luafunc_data.hpp"

} // namespace lua
