#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <lualib.h>
#include <lauxlib.h>
#include <lua.h>

/**
* @param L current lua state
* @param key Get table[key] as string
* @param out_str the output string
* @return return 1 if reached end of the modules table
* Assume that table is on the top stack
*/
int getfieldbyid_str(lua_State * L, const int key, char ** out_str);


/**
* @param L current lua state
* @param name Get name as bool, if it is not set, return false
* @param out_str the output bool
*/
bool lua_getbool(lua_State * L, const char * name);

void lua_preload(lua_State * L, const char * modname, const char * modcode);

#endif // LUA_UTILS_H
