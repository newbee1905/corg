#ifndef UTILS_H
#define UTILS_H

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdlib.h> // fopen

#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a > b ? a : b)

#define check(a)                                                               \
  if (a != NULL)                                                               \
  free(a)

#define error_lua_(L, fmt, ...)                                                \
  do {                                                                         \
    fprintf(stderr, "\033[31mforg:\033[0m " fmt "\n", ##__VA_ARGS__);          \
    lua_pop(L, 1);                                                             \
    lua_close(L);                                                              \
    exit(1);                                                                   \
  } while (0)

#define throw_lua_(L, msg)                                                     \
  do {                                                                         \
    lua_pushliteral(L, msg);                                                   \
    lua_error(L);                                                              \
  } while (0)

#define error_(fmt, ...)                                                       \
  do {                                                                         \
    fprintf(stderr, "\033[31mforg:\033[0m " fmt "\n", ##__VA_ARGS__);          \
    exit(1);                                                                   \
  } while (0)

#endif // UTILS_H
