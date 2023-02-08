#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"    // filename
#include "lua_utils.h" // lua_preload
#include "utils.h"     // error_lua_

typedef struct {
  const char *path;
  lua_CFunction func;
} PathFunc;

int foo(lua_State *L) {
  int n = lua_gettop(L); /* number of arguments */
  lua_Number sum = 0.0;
  int i;
  for (i = 1; i <= n; i++) {
    if (!lua_isnumber(L, i))
      throw_lua_(L, "incorrect argument");
    sum += lua_tonumber(L, i);
  }
  lua_pushnumber(L, sum / n); /* first result */
  lua_pushnumber(L, sum);     /* second result */
  return 2;                   /* number of results */
}

// int add_path(lua_State *L) {
// 	int n = lua_gettop(L);
//   switch (n) {
// 		case 0:
// 		case 1:
// 		default:
// 			throw_lua_(L, "incorrect number argument");
// 	}
// 	return 1;
// }

bool _wildcard(char *s, char *p) {
  char *s_backup = NULL;
  char *p_backup = NULL;
  while (*s != '\0') {
    switch (*p) {
    case '*':
      s_backup = s;
      p_backup = ++p;
      break;
    case '?':
      goto next;
    default:
      if (*s == *p) {
      next:
        ++s;
        ++p;
      } else {
        if (s_backup == NULL)
          return false;
        s = ++s_backup;
        p = p_backup;
      }
    skip:
      break;
    }
  }
  while (*p == '*')
    ++p;

  return *p == '\0' ? true : false;
}

int wildcard(lua_State *L) {
  int n = lua_gettop(L); /* number of arguments */
  printf("C: %d\n", n);
  if (n != 1)
    throw_lua_(L, "incorrect number argument");
  if (!lua_isstring(L, 1))
    throw_lua_(L, "incorrect type for argument 1");
  const char *s = lua_tostring(L, 1);
  lua_pushstring(L, s);
  // lua_Number sum = 0.0;
  // int i;
  // for (i = 1; i <= n; i++) {
  //   if (!lua_isnumber(L, i)) {
  //     lua_pushliteral(L, "incorrect argument");
  //     lua_error(L);
  //   }
  //   sum += lua_tonumber(L, i);
  // }
  // lua_pushnumber(L, sum / n); /* first result */
  // lua_pushnumber(L, sum);     /* second result */
  return 1; /* number of results */
}

int nested(lua_State *L) {
  int n = lua_gettop(L); /* number of arguments */
  lua_pushcfunction(L, wildcard);
  return 1;
}

int main(int argc, char **argv) {
  char buff[256];
  int error;
  lua_State *L;

  L = luaL_newstate();
  luaL_openlibs(L);

  // -- Register default module
  lua_preload(L, "forg", "\
Forg = {}                                                                  \
Len = 0                                                                    \
                                                                           \
_ = function(path)                                                         \
	Len = Len + 1                                                            \
	return function(patterns)                                                \
		Forg[path] = patterns                                                  \
	end                                                                      \
end                                                                        \
");

  lua_register(L, "foo", foo);
  lua_register(L, "w", wildcard);
  lua_register(L, "n", nested);

  if (luaL_dofile(L, filename))
    error_lua_(L, "cannot run configuration file: %s", lua_tostring(L, -1));

  lua_settop(L, 0);

  lua_getglobal(L, "Len");
  printf("Len: %d\n", lua_tointeger(L, -1));
  lua_pop(L, 1);

  lua_getglobal(L, "Forg");

  /* table is in the stack at index 't' */
  lua_pushnil(L); /* first key */
  while (lua_next(L, 1) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    // printf("%s - %s\n", lua_typename(L, lua_type(L, -2)),
    //        lua_typename(L, lua_type(L, -1)));
    printf("%s\n", lua_tostring(L, -2));
    /* removes 'value'; keeps 'key' for next iteration */
    lua_pop(L, 1);
  }

  // lua_pushnumber(L, 4);
  // lua_pushnumber(L, 5);

  // lua_call(L, 1, 1);
  //
  // printf("%s\n", lua_tostring(L, -1));

  // lua_pop(L, 1);

  lua_close(L);

  return 0;
}
