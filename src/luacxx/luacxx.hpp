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

#define GEN_LUAFUNC_IDX(_ret, _name, NOOP, ...) inline _ret _name (__VA_ARGS__)
#define INDEX_SPECIALIZATION
#define GET_INDEX get_index()
#include "gen_luafunc_data.hpp"

	//void len() { lua_len(m_L.get(), get_index()); }

private:
	int m_index;
};

#define GEN_LUAFUNC(_ret, _name, ...) inline _ret LuaState::_name (__VA_ARGS__)
#define GEN_BODY
#include "gen_luafunc_data.hpp"

} // namespace lua
