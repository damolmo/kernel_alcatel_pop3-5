#ifndef _SPARC_BUG_H
#define _SPARC_BUG_H

#ifdef CONFIG_BUG
#include <linux/compiler.h>

#ifdef CONFIG_DEBUG_BUGVERBOSE
void do_BUG(const char *file, int line);
#define BUG() do {					\
	do_BUG(__FILE__, __LINE__);			\
<<<<<<< HEAD
	__builtin_trap();				\
} while (0)
#else
#define BUG()		__builtin_trap()
=======
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
#else
#define BUG() do {					\
	barrier_before_unreachable();			\
	__builtin_trap();				\
} while (0)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif

#define HAVE_ARCH_BUG
#endif

#include <asm-generic/bug.h>

struct pt_regs;
void __noreturn die_if_kernel(char *str, struct pt_regs *regs);

#endif
