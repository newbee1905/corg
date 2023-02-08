#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h> // for assert
#include <stddef.h> // for size_t
#include <stdlib.h> // for malloc/realloc/free

#define _VECTOR_DEFAULT_CAPACITY 4

#define vector_set_capacity(vec, cap)                                          \
  if (vec) {                                                                   \
    (vec)[-1] = (size_t)(cap);                                                 \
  }

#define vector_set_length(vec, length)                                         \
  if (vec) {                                                                   \
    (vec)[-2] = (size_t)(length);                                              \
  }
#define vector_set_size(vec, size) vector_set_length(vec, size)

#define vector_capacity(vec) ((vec) ? (size_t)(vec)[-1] : (size_t)(0))

#define vector_size(vec) ((vec) ? (size_t)(vec)[-2] : (size_t)(0))
#define vector_length(vec) ((vec) ? (size_t)(vec)[-2] : (size_t)(0))

#define vector_is_empty(vec) (vector_size(vec) == 0)

#define _vector_grow(vec, length)                                              \
  do {                                                                         \
    const size_t v_sz = (length) * sizeof(*(vec)) + 2 * sizeof(size_t);        \
    if (!(vec)) {                                                              \
      size_t *v_tmp = malloc(v_sz);                                            \
      assert(v_tmp);                                                           \
      (vec) = (void *)(&v_tmp[2]);                                             \
      vector_set_capacity((vec), (_VECTOR_DEFAULT_CAPACITY));                  \
      vector_set_length((vec), 0);                                             \
    } else {                                                                   \
      size_t *v_tmp = realloc(&((size_t *)(vec))[-2], (v_sz));                 \
      assert(v_tmp);                                                           \
      (vec) = (void *)(&v_tmp[2]);                                             \
      vector_set_capacity((vec), (length));                                    \
    }                                                                          \
  } while (0)

#define vector_push_back(vec, a)                                               \
  do {                                                                         \
    size_t cap = vector_capacity(vec);                                         \
    if (vector_length(vec) == cap)                                             \
      _vector_grow((vec), (cap ? cap << 1 : _VECTOR_DEFAULT_CAPACITY));        \
    (vec)[(vec)[-2]++] = a;                                                    \
  } while (0)

#define vector_pop_back(vec) (vector_length(vec)) ? (--(vec)[-2]) : (size_t)(0)

#define vector_free(vec)                                                       \
  if (vec) {                                                                   \
    free(&((size_t *)(vec))[-2]);                                              \
  }

#endif // VECTOR_H
