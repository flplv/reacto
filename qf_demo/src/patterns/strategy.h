#ifndef STRATEGY_H
#define STRATEGY_H

/*
 * Strategy Pattern
 *
 * Intent: Define a family of algorithms, encapsulate each one and make them
 * interchangeable in run time.
 *
 * Also know as:
 *  Policy, Interface
 *
 * When should be used:
 *
 *
 * How it works:
 *  It must be a Class, to store selected behavior;
 *  Client code selects behavior at the `init`.
 *  The strategy.c file redirect calls to implementations.
 *  Client depends only on this abstraction (header).
 */

typedef struct private_strategy strategy_t;

/* Behavior struct that selects function pointers to change implementation */
struct strategy_behavior
{
    void (*set_number)(strategy_t * obj, int number);
    int  (*get_number)(strategy_t * obj);
    int (*init)(strategy_t * obj);
    void (*deinit)(strategy_t * obj);
};

/* A struct/union to be allocated by the client */
struct private_strategy
{
    /*
     * Sometimes behaviors will not have the same private data, then an union
     *  will be required here to reserve fields of each strategy.
     */
    int number;
    const struct strategy_behavior * behavior;
};

/*
 * Available behaviors
 */
/* Normal behavior */
extern const struct strategy_behavior strategy_normal;
/* Twice behavior will duplicate the number set */
extern const struct strategy_behavior strategy_twice;

int strategy_init(strategy_t * obj, const struct strategy_behavior * behavior);
void strategy_deinit(strategy_t * obj);

void strategy_set_number(strategy_t * obj, int number);
int strategy_get_number(strategy_t * obj);

#endif
