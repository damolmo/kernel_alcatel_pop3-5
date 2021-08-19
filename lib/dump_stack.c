/*
 * Provide a default dump_stack() function for architectures
 * which don't implement their own.
 */

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/atomic.h>

static void __dump_stack(void)
{
	dump_stack_print_info(KERN_DEFAULT);
	show_stack(NULL, NULL);
}

/**
 * dump_stack - dump the current task information and its stack trace
 *
 * Architectures can override this implementation by implementing its own.
 */
#ifdef CONFIG_SMP
static atomic_t dump_lock = ATOMIC_INIT(-1);

asmlinkage __visible void dump_stack(void)
{
<<<<<<< HEAD
=======
	unsigned long flags;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	int was_locked;
	int old;
	int cpu;

	/*
	 * Permit this cpu to perform nested stack dumps while serialising
	 * against other CPUs
	 */
<<<<<<< HEAD
	preempt_disable();

retry:
=======
retry:
	local_irq_save(flags);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	cpu = smp_processor_id();
	old = atomic_cmpxchg(&dump_lock, -1, cpu);
	if (old == -1) {
		was_locked = 0;
	} else if (old == cpu) {
		was_locked = 1;
	} else {
<<<<<<< HEAD
		cpu_relax();
=======
		local_irq_restore(flags);
		/*
		 * Wait for the lock to release before jumping to
		 * atomic_cmpxchg() in order to mitigate the thundering herd
		 * problem.
		 */
		do { cpu_relax(); } while (atomic_read(&dump_lock) != -1);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		goto retry;
	}

	__dump_stack();

	if (!was_locked)
		atomic_set(&dump_lock, -1);

<<<<<<< HEAD
	preempt_enable();
=======
	local_irq_restore(flags);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}
#else
asmlinkage __visible void dump_stack(void)
{
	__dump_stack();
}
#endif
EXPORT_SYMBOL(dump_stack);
