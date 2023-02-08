#ifndef VEC_H
#define VEC_H

#include <assert.h> // for assert
#include <stddef.h> // size_t
#include <stdlib.h> // for malloc/realloc/free

#define _VECTOR_DEFAULT_CAPACITY 16

#define vec(type) type *

#define vector_set_capacity(v, cap)                                            \
  if (v) {                                                                     \
    (v)[-1] = (size_t)(cap);                                                   \
  }

#define vector_set_length(v, len)                                              \
  if (v) {                                                                     \
    (v)[-2] = (size_t)(len);                                                   \
  }

#define vector_capacity(v) ((v) ? (size_t)((v)[-1]) : (size_t)0)
#define vector_length(v) ((v) ? (size_t)((v)[-2]) : (size_t)0)

#define _vector_grow(v, length)                                                \
  do {                                                                         \
    const size_t v_sz = (length) * sizeof(*(v)) + 2 * sizeof(size_t);          \
    if (!(v)) {                                                                \
      size_t *v_tmp = malloc(v_sz);                                            \
      assert(v_tmp);                                                           \
      (v) = (void *)(&v_tmp[2]);                                               \
      vector_set_length((v), 0);                                               \
    } else {                                                                   \
      size_t *_v_tmp = &((size_t *)(v))[-2];                                   \
      size_t *v_tmp = realloc(_v_tmp, (v_sz));                                 \
      assert(v_tmp);                                                           \
      (v) = (void *)(&v_tmp[2]);                                               \
    }                                                                          \
    vector_set_capacity((v), (length));                                        \
  } while (0)

#define vector_push_back(v, a)                                                 \
  do {                                                                         \
    size_t cap = vector_capacity((v));                                         \
    size_t len = vector_length((v));                                           \
    if (cap == vector_length(v))                                               \
      _vector_grow((v), (cap ? (cap << 1) : (_VECTOR_DEFAULT_CAPACITY)));      \
    (v)[len] = (a);                                                            \
    vector_set_length((v), len + 1);                                           \
  } while (0)

#define vector_pop_back(v) (vector_length(v)) ? (--(v)[-2]) : (size_t)(0)

#define vector_free(v)                                                         \
  do {                                                                         \
    size_t *_v_tmp = &((size_t *)(v))[-2];                                     \
    free(_v_tmp);                                                              \
  } while (0)

// simpler sytnax
#define set_veccap(v, cap) vector_set_capacity(vec, cap)
#define set_veclen(v, cap) vector_set_length(vec, cap)
#define veccap(v) vector_capacity(v)
#define veclen(v) vector_length(v)
#define vecpush(v, val) vector_push_back(v, val)
#define vecpop(v) vector_pop_back(v)
#define vecfree(v) vector_free((v))

#endif // VEC_H
