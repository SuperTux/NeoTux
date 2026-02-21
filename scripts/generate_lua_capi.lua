#!/usr/bin/env lua5.4
--[[
  SuperTux
  Copyright (C) 2026 Hyland B. <me@ow.swag.toys>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
]]--

local _header_mode = (arg[3] == 'header')
local _data = dofile(arg[2] or 'lua_capi_data.lua')

-- Slips some text into our header line
function slip_in(filename, output, content)
	local fh = io.open(output, "w")
	local found = false

	for line in io.lines(filename) do
		if line == "#define __GENERATE_LUA_METHODS__" then
			fh:write("// BEGIN GENERATED BITS\n")
			fh:write(content .. '\n')
			fh:write("// END GENERATED BITS\n")
			found = true
		else
			fh:write(line .. '\n')
		end
	end
	
	if not found then
		print "-- couldn't find __GENERATE_LUA_METHODS__ macro!"
	end
end

function iterate_data(cb)
	for _, fun in pairs(_data) do
		local symbol
		local args = fun[2]
		local ret = fun[3] or 'void'
		if (fun[1] == '' or not fun[1]) then goto hell end
		
		if string.sub(fun[1], 0, 1) == '_' then
			symbol = string.sub(fun[1], 2)
		else
			goto hell
		end
		-- TODO: more error checkin
		
		cb(ret, fun[1], symbol, args)
		
		::hell::
	end
end

function split_arg(arg)
	start = string.find(arg, '@')
	if not start then return arg end
	return { string.sub(arg, 1, start-1), string.sub(arg, start+1) }
end

function split_arg_join(arg)
	return table.concat(split_arg(arg), ' ')
end

function flatten_args(args)
	if not args then return '' end
	
	if type(args) == 'string' then
		return split_arg_join(args)
	end
	
	local args_tbl = {}
	for i, v in pairs(args) do
		args_tbl[#args_tbl+1] = split_arg_join(v)
	end
	return table.concat(args_tbl, ', ')
end

function flatten_args_ret(args)
	if not args then return '' end
	
	if type(args) == 'string' then
		return split_arg(args)[2]
	end
	
	local args_tbl = {}
	for i, v in pairs(args) do
		args_tbl[#args_tbl+1] = split_arg(v)[2]
	end
	return table.concat(args_tbl, ', ')
end

function generate_header()
	print '-- generating headers'
	local res = {}
	
	iterate_data(function(ret, raw_sym, sym, args)
		res[#res+1] = string.format('%s %s (%s)', ret, sym, flatten_args(args))
	end)
	
	return table.concat(res, '\n')
end

function generate_cxx()
	print '-- generating cpp'
	local res = {}
	
	-- TODO: flatten args
	iterate_data(function(ret, raw_sym, sym, args)
		res[#res+1] =
			string.format('%s LuaState::%s (%s) { %s lua%s(%s%s); }',
				ret,
				sym,
				flatten_args(args),
				-- {
				ret ~= 'void' and 'return' or '',
				raw_sym,
				args and '_L, ' or '_L',
				flatten_args_ret(args))
			
	end)
	
	return table.concat(res, '\n')
end

slip_in(arg[1], "output.hpp", _header_mode and generate_header() or generate_cxx())

