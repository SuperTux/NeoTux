#ifndef GEN_LUAFUNC
#	define GEN_LUAFUNC(...)
#	error No GEN_LUAFUNC macro defined!
#endif

#ifdef GEN_BODY
#	define GEN_LUABODY(RETURN, FNAME, ...) { RETURN lua_##FNAME (_L __VA_OPT__(,) __VA_ARGS__); }
#else
#	define GEN_LUABODY(...)
#endif

GEN_LUAFUNC(void, absindex, int idx)
GEN_LUABODY(    , absindex,     idx)
GEN_LUAFUNC(void, arith, int op)
GEN_LUABODY(    , arith,     op)
GEN_LUAFUNC(void, call, int nargs, int nresults)
GEN_LUABODY(    , call,     nargs,     nresults)
GEN_LUAFUNC(void, callk, int nargs, int nresults, lua_KContext ctx, lua_KFunction k)
GEN_LUABODY(    , callk,     nargs,     nresults,              ctx,               k)
GEN_LUAFUNC(int,    checkstack, int n)
GEN_LUABODY(return, checkstack,     n)
GEN_LUAFUNC(void, close, void)
GEN_LUABODY(    , close)
GEN_LUAFUNC(void, closeslot, int index)
GEN_LUABODY(    , closeslot,     index)
GEN_LUAFUNC(void, closethread, lua_State *from)
GEN_LUABODY(    , closethread,            from)
GEN_LUAFUNC(void, compare, int index1, int index2, int op)
GEN_LUABODY(    , compare,     index1,     index2,     op)
GEN_LUAFUNC(void, concat, int n)
GEN_LUABODY(    , concat,     n)
GEN_LUAFUNC(void, copy, int fromidx, int toidx)
GEN_LUABODY(    , copy,     fromidx,     toidx)
GEN_LUAFUNC(void, createtable, int narr, int nrec)
GEN_LUABODY(    , createtable,     narr,     nrec)
GEN_LUAFUNC(void, dump, lua_Writer writer, void *data, int strip)
GEN_LUABODY(    , dump,            writer,       data,     strip)
GEN_LUAFUNC(void, error, void)
GEN_LUABODY(    , error)
GEN_LUAFUNC(void, getallocf, void **ud)
GEN_LUABODY(    , getallocf,        ud)
GEN_LUAFUNC(void, getfield, int index, const char* k)
GEN_LUABODY(    , getfield,     index,             k)
GEN_LUAFUNC(void, getextraspace, void)
GEN_LUABODY(    , getextraspace)

// throw these away; they're one use
#undef GEN_BODY
#undef GEN_LUABODY
#undef GEN_LUAFUNC
