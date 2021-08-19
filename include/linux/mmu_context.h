#ifndef _LINUX_MMU_CONTEXT_H
#define _LINUX_MMU_CONTEXT_H

<<<<<<< HEAD
=======
#include <asm/mmu_context.h>

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
struct mm_struct;

void use_mm(struct mm_struct *mm);
void unuse_mm(struct mm_struct *mm);

<<<<<<< HEAD
=======
/* Architectures that care about IRQ state in switch_mm can override this. */
#ifndef switch_mm_irqs_off
# define switch_mm_irqs_off switch_mm
#endif

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#endif
