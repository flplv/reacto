#ifndef INTERFACE_PRIVATE_H_
#define INTERFACE_PRIVATE_H_


typedef struct private_interface interface_t;

/* Behavior struct that selects function pointers to change implementation */
struct interface_behavior
{
    void (*set_number)(interface_t * obj, int number);
    int  (*get_number)(interface_t * obj);
    int  (*init)(interface_t * obj);
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



#endif /* QF_DEMO_SRC_PATTERNS_INTERFACE_PRIVATE_H_ */
