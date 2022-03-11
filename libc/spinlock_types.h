#ifndef TOS_LIBC_SPINLOCK_TYPES_H
#define TOS_LIBC_SPINLOCK_TYPES_H

typedef volatile int spinlock_t;

#define spin_lock(name) \
    while(!__sync_bool_compare_and_swap((name), 0, 1)) \
    { \
        asm("pause"); \
    }
#define spin_unlock(name) \
        __sync_synchronize(); \
        *(name) = 0;

#endif //TOS_LIBC_SPINLOCK_TYPES_H
