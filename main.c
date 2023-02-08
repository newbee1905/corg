#include <dirent.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdio.h> // memeset, printf
#include <stdlib.h>
#include <string.h> // strcpy

#include "config.h" // filename
#include "utils.h"  // error_lua_
#include "vec.h"    // vec

int main(int argc, char **argv) {
  // Read config table
  vec(char *) dirlist = NULL;
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
        char *d_name = malloc(strlen(dir->d_name));
        assert(d_name);
        strncpy(d_name, dir->d_name, strlen(dir->d_name));
        vecpush(dirlist, d_name);
      }
    }
    closedir(d);
  }

  short *dirskip = malloc(veclen(dirlist) * sizeof(short));
  memset(dirskip, 1, veclen(dirlist));

  // Setting up lua
  lua_State *L;

  L = luaL_newstate();
  luaL_openlibs(L);

  if (luaL_dofile(L, filename))
    error_lua_(L, "cannot run configuration file: %s", lua_tostring(L, -1));

  // Set the config table to a variable
  // Get it back later in a look
  lua_setglobal(L, "config");
  lua_settop(L, 0);

  // Cycling through returned table
  lua_getglobal(L, "config");
  lua_pushnil(L); /* first key */
  while (lua_next(L, 1) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    // printf("%s - %s\n", lua_typename(L, lua_type(L, -2)),
    //        lua_typename(L, lua_type(L, -1)));
    const char *dest = lua_tostring(L, -2);
		printf("[%s]\n", dest);
    lua_gettable(L, -2);
    lua_pushnil(L);
    while (lua_next(L, 1) != 0) {
      printf("%s - %s\n", lua_typename(L, lua_type(L, -2)),
             lua_typename(L, lua_type(L, -1)));
      lua_pop(L, 1);
    }
    /* removes 'value'; keeps 'key' for next iteration */
    lua_pop(L, 1);
  }

  vecfree(dirlist);
  free(dirskip);
  return 0;
}
