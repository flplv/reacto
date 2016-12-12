#ifndef SIGNALSLOT_DEMO_H_
#define SIGNALSLOT_DEMO_H_

#include "linked_list.h"
#include <stdbool.h>

struct slot_demo_private;
typedef struct slot_demo_private slot_demo_t;

struct signal_demo_private;
typedef struct signal_demo_private signal_demo_t;

/*
 * Rules of the Handler:
 *  Handlers will be called in the order they were inserted.
 *  If a handler returns anything but 0, the call loop will stop and no other
 *   connected handler will be called.
 */
typedef int (*slot_demo_handler_t) (slot_demo_t * slot, int, char *);

void signal_demo_init(signal_demo_t *);
void signal_demo_deinit(signal_demo_t *);
bool signal_demo_is_connected(signal_demo_t *, slot_demo_t *);
void signal_demo_emit(signal_demo_t *, int n, char * buf);

void slot_demo_init(slot_demo_t *, slot_demo_handler_t handler);
void slot_demo_deinit(slot_demo_t *);
void slot_demo_connect(slot_demo_t *, signal_demo_t *);
int slot_demo_disconnect(slot_demo_t *, signal_demo_t *);

struct signal_demo_private
{
    struct slot_demo_private * root;
};

struct slot_demo_private
{
    signal_demo_t * connection;
    slot_demo_handler_t handler;
    linked_list_t ll;
};

#endif /* SIGNALSLOT_DEMO_H_ */
