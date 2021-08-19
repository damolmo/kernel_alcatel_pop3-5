/*
 * Generate definitions needed by the preprocessor.
 * This code generates raw asm output which is post-processed
 * to extract and format the required data.
 */

#define __GENERATING_BOUNDS_H
/* Include headers that define the enum constants of interest */
#include <linux/page-flags.h>
#include <linux/mmzone.h>
#include <linux/kbuild.h>
#include <linux/log2.h>
#include <linux/spinlock_types.h>

<<<<<<< HEAD
void foo(void)
=======
int main(void)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
{
	/* The enum constants to put into include/generated/bounds.h */
	DEFINE(NR_PAGEFLAGS, __NR_PAGEFLAGS);
	DEFINE(MAX_NR_ZONES, __MAX_NR_ZONES);
#ifdef CONFIG_SMP
	DEFINE(NR_CPUS_BITS, ilog2(CONFIG_NR_CPUS));
#endif
	DEFINE(SPINLOCK_SIZE, sizeof(spinlock_t));
	/* End of constants */
<<<<<<< HEAD
=======

	return 0;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}
