#ifndef _M68K_BUG_H
#define _M68K_BUG_H

#ifdef CONFIG_MMU
#ifdef CONFIG_BUG
#ifdef CONFIG_DEBUG_BUGVERBOSE
#ifndef CONFIG_SUN3
#define BUG() do { \
	printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); \
<<<<<<< HEAD
=======
	barrier_before_unreachable(); \
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	__builtin_trap(); \
} while (0)
#else
#define BUG() do { \
	printk("kernel BUG at %s:%d!\n", __FILE__, __LINE__); \
<<<<<<< HEAD
=======
	barrier_before_unreachable(); \
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	panic("BUG!"); \
} while (0)
#endif
#else
#define BUG() do { \
<<<<<<< HEAD
=======
	barrier_before_unreachable(); \
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	__builtin_trap(); \
} while (0)
#endif

#define HAVE_ARCH_BUG
#endif
#endif /* CONFIG_MMU */

#include <asm-generic/bug.h>

#endif
