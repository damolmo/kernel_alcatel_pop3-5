#ifndef _ASM_POWERPC_IRQ_WORK_H
#define _ASM_POWERPC_IRQ_WORK_H

static inline bool arch_irq_work_has_interrupt(void)
{
	return true;
}
<<<<<<< HEAD
=======
extern void arch_irq_work_raise(void);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#endif /* _ASM_POWERPC_IRQ_WORK_H */
