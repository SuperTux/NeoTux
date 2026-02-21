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

#include "luacxx.hpp"
#include <lauxlib.h>

#define _L (m_L.get())

template <class T>
static void
noop_delete(T* U)
{}

namespace lua
{

LuaState::LuaState()
	: m_L(nullptr, lua_close)
{
	m_L.reset(luaL_newstate());
}

// should we use a weak pointer for these instead?
LuaState::LuaState(lua_State* L)
	: m_L(L, ::noop_delete)
{}

LuaState::LuaState(const LuaState& state)
	: LuaState(state.get_state())
{}

void
LuaState::openlibs()
{
	// TODO better control over this/sandboxing, whatever
	luaL_openlibs(_L);
}

#define __GENERATE_LUA_METHODS__

} // namespace lua

#ifdef __GENERATE_LUA_METHODS__
#	error luacxx.cpp has not been generated yet. You may not use this on its own.
#endif
