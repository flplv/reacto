#ifndef ARRAY_H_
#define ARRAY_H_

#include "checks.h"
#include "log.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
#define __arr_auto_type(___zcxzzarg) auto
#else
#define __arr_auto_type(___zcxzzarg) typeof(___zcxzzarg)
#endif

struct array_private
{
    size_t max;
    size_t top;
};

typedef struct array_private array_t;

void array_init(array_t * obj, size_t max_length);
void array_deinit(array_t * obj);

void array_clear(array_t * obj);
size_t array_length(array_t * obj);
bool array_full(array_t * obj);


static inline size_t _array_insert_prepare(array_t * obj)
{
    check_ptr(obj, 0);

    if (array_full(obj))
    {
        log_warning("Trying to insert in a full array, data loss happened.");
        obj->top--;
    }

    return obj->top;
}

static inline void _array_insert_end(array_t * obj)
{
    check_ptr(obj);
    obj->top ++;
}

#define array_insert(obj, buffer, data) \
({ \
    array_t * _o = (obj); \
    buffer[_array_insert_prepare(_o)] = data; \
    _array_insert_end(_o); \
})

static inline size_t _array_remove_prepare(array_t * obj, size_t pos)
{
    check_ptr(obj, 0);

    if (pos >= obj->top)
        return 0;

    return obj->top - 1 - pos;
}

static inline void _array_remove_end(array_t * obj)
{
    check_ptr(obj);
    obj->top --;
}

#define array_remove(obj, buffer, pos) \
({ \
    array_t * _o = (obj); \
    size_t _p = (pos); \
    __arr_auto_type (buffer) _b = (buffer); \
    size_t n = _array_remove_prepare(_o, _p); \
    for (size_t i = 0; i < n; i++) \
        _b[_p + i] = _b[_p + i + 1]; \
    if (n) _array_remove_end(_o); \
})

#define array_free(obj, buffer, pos, freefunc) \
({ \
    array_t * _o = (obj); \
    size_t _p = (pos); \
    __arr_auto_type (buffer) _b = (buffer); \
    __arr_auto_type (freefunc) _f = (freefunc); \
    _f(_b[_p]); \
    for (size_t i = 0; i < _array_remove_prepare(_o, _p); i++) \
        _b[_p + i] = _b[_p + i + 1]; \
    _array_remove_end(_o); \
})

#define array_find(obj, buffer, comparefunc, seed) \
({ \
    int _r = -1; \
    array_t * _o = (obj); \
    __arr_auto_type (buffer) _b = (buffer); \
    __arr_auto_type (seed) _s = (seed); \
    __arr_auto_type (comparefunc) _f = (comparefunc); \
    for (size_t i = 0; i < array_length(_o); i++) \
        if (_f(_b[i], _s)) { _r = (int)i; break; } \
    _r; \
})


#endif /* ARRAY_H_ */
