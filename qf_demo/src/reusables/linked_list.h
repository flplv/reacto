/*
* Copyright (C) 2013 to 2014 by Felipe Lavratti
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in the
* Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
* AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stddef.h>

#ifndef __cplusplus
#define __ll_auto_type typeof
#define __ll_typeof typeof

#define __ll_container_of(ptr, type, member) ({ \
                const __ll_auto_type( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type, member) );})
#else
#ifndef __ll_container_of
#error "You need to include linked_list.hpp to use this module in C++"
#endif
#endif // __cplusplus

struct s_linked_list_head
{
    struct s_linked_list_head * next;
    struct s_linked_list_head * prev;
};

typedef struct s_linked_list_head linked_list_t;


/**
 * Initialize a linked_list_t within structure
 * @param
 */
#define linked_list_init(structure, field) \
({ \
        __ll_auto_type(structure) ___str = structure; \
        if (___str) {\
        	___str->field.next = (linked_list_t *)0;\
        	___str->field.prev = (linked_list_t *)0;\
        } \
})

/**
 * Get the number of nodes of the list.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define linked_list_count(ptr_to_current, field) \
({ \
        int ___ret = 0; \
        __ll_auto_type(ptr_to_current) ___cur = linked_list_first(ptr_to_current, field); linked_list_t * ___head;\
        if (___cur) ___ret++; \
        if (___ret) \
        { \
            ___head = &___cur->field; \
            if (___head->next) { while (___head->next){ ___head = ___head->next; ___ret++;}} \
        } \
        ___ret; \
})


/**
 * Get the next node.
 * @param ptr_to_current
 * @return Pointer to the next node.
 */
#define linked_list_next(ptr_to_current, field) \
({ \
        __ll_auto_type(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.next; \
           if (___head) { ___ret  = __ll_container_of(___head, __ll_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the previous node.
 * @param ptr_to_current
 * @return Pointer to the previous node.
 */
#define linked_list_previous(ptr_to_current, field) \
({ \
        __ll_auto_type(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.prev; \
           if (___head) { ___ret  = __ll_container_of(___head, __ll_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the last node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define linked_list_last(ptr_to_current, field) \
({ \
        __ll_auto_type(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
            ___head = &___ret->field; \
            if (___head->next) { while (___head->next) ___head = ___head->next;} \
            if (___head) { ___ret  = __ll_container_of(___head, __ll_typeof(*(ptr_to_current)), field); } \
            else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the first node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the first node of the list, including ptr_to_current.
 */
#define linked_list_first(ptr_to_current, field) \
({ \
        __ll_auto_type(ptr_to_current) ___ret = ptr_to_current; linked_list_t * ___head;\
        if (___ret) \
        { \
            ___head = &___ret->field; \
            if (___head->prev) { while (___head->prev) ___head = ___head->prev;} \
            if (___head) { ___ret  = __ll_container_of(___head, __ll_typeof(*(ptr_to_current)), field); } \
            else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Insert new node between current and current's next.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define linked_list_insert_after(ptr_to_current, ptr_to_new, field) \
({ \
        __ll_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        __ll_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.next = ___curr->field.next; \
            ___new->field.prev = &___curr->field; \
            ___curr->field.next = &___new->field; \
        } \
})

/**
 * Insert new node between current and current's previous.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define linked_list_insert_before(ptr_to_current, ptr_to_new, field) \
({ \
        __ll_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        __ll_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.prev = ___curr->field.prev; \
            ___new->field.next = &___curr->field; \
            ___curr->field.prev = &___new->field; \
        } \
})

/**
 * Remove the current node and return it.
 *
 * @param current
 * @return Return the new or old beginning of the list.
 */
#define linked_list_remove(ptr_to_current, field) \
({ \
        __ll_auto_type(ptr_to_current) ___cur = ptr_to_current; \
        __ll_auto_type(ptr_to_current) ___ret = (__ll_typeof(ptr_to_current))0; \
        linked_list_t * ___head_next = (linked_list_t*)0; \
        linked_list_t * ___head_prev = (linked_list_t*)0; \
        if (___cur) \
        { \
           ___head_next = ___cur->field.next; \
           ___head_prev = ___cur->field.prev; \
           if (___head_prev) ___head_prev->next = ___head_next; \
           if (___head_next) ___head_next->prev = ___head_prev; \
           linked_list_init(___cur, field); \
           if (___head_prev) ___ret = linked_list_first(__ll_container_of(___head_prev, __ll_typeof(*(ptr_to_current)), field), field); \
           else if(___head_next) ___ret = linked_list_first(__ll_container_of(___head_next, __ll_typeof(*(ptr_to_current)), field), field); \
        } \
        ___ret; \
})

/**
 * Free all list members,
 * but instead calling free itself, this function calls
 * user free function passed on free_callback.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any pointer to the a list node will be invalid.
 */
#define linked_list_free(ptr_to_current, field, free_callback) \
({ \
        __ll_auto_type(ptr_to_current) ___item = linked_list_first(ptr_to_current, field); \
        __ll_typeof(___item) ___item_to_delete; \
        while (___item) \
        { \
            ___item_to_delete = ___item; \
            ___item = linked_list_next(___item_to_delete, field); \
            free_callback(___item_to_delete); \
        } \
})

/**
 */
#define linked_list_find(ptr_to_current, field, comparator_func, comparator_seed) \
({ \
        __ll_auto_type(ptr_to_current) ___item = ptr_to_current; \
        while (___item) \
        { \
        	if (comparator_func(comparator_seed, ___item) == true) break; \
            ___item = linked_list_next(___item, field); \
        } \
        ___item; \
})

#endif /* LINKED_LIST_H_ */
