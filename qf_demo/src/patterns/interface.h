#ifndef INTERFACE_H
#define INTERFACE_H

/*
 * Interface Pattern
 *
 * Intent: Define a contract where every code involved will depend on.
 *
 * Also know as:
 *
 *
 * When should be used:
 *  Every time you have 1:N dependency: a 1 client must use N objects with
 *   different implementations.
 *  Menu navigation, event abstraction, etc.
 *
 *
 * How it works:
 *  It must be a Class, to store selected behavior;
 *  Client code selects behavior at the `init`.
 *  The .c file redirect calls to implementations.
 *  Client depends only on this abstraction (header).
 */

typedef struct private_interface interface_t;

/* Behavior struct that selects function pointers to change implementation */
struct interface_behavior
{
    void (*set_number)(interface_t * obj, int number);
    int  (*get_number)(interface_t * obj);
    int (*init)(interface_t * obj);
    void (*deinit)(interface_t * obj);
};

/* A struct/union to be allocated by the client */
struct private_interface
{
    /*
     * Sometimes behaviors will not have the same private data, then an union
     *  will be required here to reserve fields of each interface.
     */
    int number;
    const struct interface_behavior * behavior;
};

/*
 * Available behaviors
 */
/* Normal behavior */
extern const struct interface_behavior interface_normal;
/* Twice behavior will duplicate the number set */
extern const struct interface_behavior interface_twice;
/* Plus behavior will add the number set */
extern const struct interface_behavior interface_plus;


int interface_init(interface_t * obj, const struct interface_behavior * behavior);
void interface_deinit(interface_t * obj);

void interface_set_number(interface_t * obj, int number);
int interface_get_number(interface_t * obj);

#endif
