#ifndef SIGNAL_SLOT_H_
#define SIGNAL_SLOT_H_

#include "linked_list.h"
#include <stdbool.h>

struct slot_private;
typedef struct slot_private slot_t;

struct signal_private;
typedef struct signal_private signal_t;

/*
 * Rules of the Handler:
 *  Handlers will be called in the order they were inserted.
 *  If a handler returns anything but 0, the call loop will stop and no other
 *   connected handler will be called.
 */
typedef int (*slot_handler_t) (slot_t *);

void signal_init(signal_t *);
void signal_deinit(signal_t *);
bool signal_is_connected(signal_t *, slot_t *);
void signal_emit(signal_t *);

void slot_init(slot_t *, slot_handler_t handler);
void slot_deinit(slot_t *);
void slot_connect(slot_t *, signal_t *);
int slot_disconnect(slot_t *, signal_t *);

struct signal_private
{
    struct slot_private * root;
};

struct slot_private
{
    signal_t * connection;
    slot_handler_t handler;
    linked_list_t ll;
};

#endif /* SIGNAL_SLOT_H_ */
