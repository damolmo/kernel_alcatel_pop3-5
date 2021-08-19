#ifndef _PARISC_SIGINFO_H
#define _PARISC_SIGINFO_H

<<<<<<< HEAD
=======
#if defined(__LP64__)
#define __ARCH_SI_PREAMBLE_SIZE   (4 * sizeof(int))
#endif

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <asm-generic/siginfo.h>

#undef NSIGTRAP
#define NSIGTRAP	4

#endif
