#ifndef REUSABLES_MACROS_H_
#define REUSABLES_MACROS_H_

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#endif /* REUSABLES_MACROS_H_ */
