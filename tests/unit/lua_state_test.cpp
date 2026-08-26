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

#include <cassert>
#include <iostream>
#include "luacxx/luacxx.hpp"

void
doohickey(lua::LuaState state)
{
}

int
main()
{
	lua::LuaState state{};

	state.openlibs();

	doohickey(state);
	state.pushcfunction([](lua_State *_s) {
		((lua::LuaState)_s).pushstring("hi");
		return 1;
	}).setglobal("mycoolfunction");

	state.loadstring("return mycoolfunction()..' world'");
	state.pcall(0, LUA_MULTRET, 0);
	//state.pcall(0, LUA_MULTRET, 0);

	std::cout << "Got " << state.gettop().tostring() << std::endl;
	state.pop(); //bye

	return 0;
}
