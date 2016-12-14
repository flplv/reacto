#include "array.h"

void array_init(array_t * obj, size_t max_length)
{
    check_ptr(obj);

    obj->max = max_length;
    obj->top = 0;
}

void array_deinit(array_t * obj)
{
    check_ptr(obj);

    obj->max = 0;
    obj->top = 0;
}

void array_clear(array_t * obj)
{
    check_ptr(obj);
    obj->top = 0;
}

size_t array_length(array_t * obj)
{
    check_ptr(obj, 0);
    return obj->top;
}

bool array_full(array_t * obj)
{
    check_ptr(obj, true);
    return obj->top == obj->max;
}
