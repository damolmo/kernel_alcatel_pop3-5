#ifndef __ASM_IRQ_WORK_H
#define __ASM_IRQ_WORK_H

<<<<<<< HEAD
#ifdef CONFIG_SMP

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <asm/smp.h>

static inline bool arch_irq_work_has_interrupt(void)
{
	return !!__smp_cross_call;
}

<<<<<<< HEAD
#else

static inline bool arch_irq_work_has_interrupt(void)
{
	return false;
}

#endif

=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif /* __ASM_IRQ_WORK_H */
