#ifndef __ASM_IRQ_H
#define __ASM_IRQ_H

#include <asm-generic/irq.h>

<<<<<<< HEAD
extern void (*handle_arch_irq)(struct pt_regs *);
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
extern void migrate_irqs(void);
extern void set_handle_irq(void (*handle_irq)(struct pt_regs *));

#endif
