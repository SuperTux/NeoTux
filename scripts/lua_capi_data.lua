local VOID = 'void'
local INT = 'int'
local CFUNC = 'lua_CFunction'
return {
	{ '_absindex', 'int@idx' },
	{ '_arith', 'int@op', CFUNC },
	{ '_call', {'int@nargs','int@nresults'} },
	{ '_callk', {'int@nargs','int@nresults','lua_KContext@ctx','lua_KFunction@k'} },
	{ '_checkstack', 'int@n', INT },
	{ '_close' },
	{ '_closeslot', 'int@index' },
	{ '_closethread', 'lua_State *@from' },
	{ '_compare' },
	{ '_concat', 'int@n' },
	{ '_copy', {'int@fromidx','int@toidx'} },
	{ '_createtable', {'int@narr','int@nrec'} },
	{ '_dump', {'lua_Writer@writer','void *@data','int @strip'} },
	{ '_error' },
	{ '_getallocf', 'void **@ud' },
	{ '_getfield', {'int@index','const char *@k'} },
	-- continue...
	--{ '', '' },
}
