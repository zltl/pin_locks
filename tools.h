#ifndef __TOOLS_H
#define __TOOLS_H

#include <stdint.h>

#define UL unsigned long

#ifndef __force
# define __force __attribute__((force))
#endif
#ifndef __bitwise
# define __bitwise __attribute__((bitwise))
#endif

#ifndef barrier
/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
# define barrier() __asm__ __volatile__("": : :"memory")
#endif

#ifndef __maybe_unused
# define __maybe_unused		__attribute__((unused))
#endif

#ifndef __packed
# define __packed		__attribute__((__packed__))
#endif

#ifndef __force
# define __force
#endif

#ifndef __weak
# define __weak			__attribute__((weak))
#endif

#ifndef likely
# define likely(x)		__builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
# define unlikely(x)		__builtin_expect(!!(x), 0)
#endif

#ifndef ACCESS_ONCE
# define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
#endif

#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#ifndef container_of
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
# define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#endif
