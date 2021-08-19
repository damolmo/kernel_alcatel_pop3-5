#ifndef _ASM_ARM_XEN_EVENTS_H
#define _ASM_ARM_XEN_EVENTS_H

#include <asm/ptrace.h>
#include <asm/atomic.h>

enum ipi_vector {
	XEN_PLACEHOLDER_VECTOR,

	/* Xen IPIs go here */
	XEN_NR_IPIS,
};

static inline int xen_irqs_disabled(struct pt_regs *regs)
{
	return raw_irqs_disabled_flags(regs->ARM_cpsr);
}

<<<<<<< HEAD
#define xchg_xen_ulong(ptr, val) atomic64_xchg(container_of((ptr),	\
=======
#define xchg_xen_ulong(ptr, val) atomic64_xchg(container_of((long long*)(ptr),\
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
							    atomic64_t,	\
							    counter), (val))

#endif /* _ASM_ARM_XEN_EVENTS_H */
