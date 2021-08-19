/*
 * arch/xtensa/include/asm/traps.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2012 Tensilica Inc.
 */
#ifndef _XTENSA_TRAPS_H
#define _XTENSA_TRAPS_H

#include <asm/ptrace.h>

/*
 * handler must be either of the following:
 *  void (*)(struct pt_regs *regs);
 *  void (*)(struct pt_regs *regs, unsigned long exccause);
 */
extern void * __init trap_set_handler(int cause, void *handler);
extern void do_unhandled(struct pt_regs *regs, unsigned long exccause);
void secondary_trap_init(void);

static inline void spill_registers(void)
{
#if XCHAL_NUM_AREGS > 16
	__asm__ __volatile__ (
<<<<<<< HEAD
		"	call12	1f\n"
=======
		"	call8	1f\n"
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
		"	_j	2f\n"
		"	retw\n"
		"	.align	4\n"
		"1:\n"
<<<<<<< HEAD
		"	_entry	a1, 48\n"
		"	addi	a12, a0, 3\n"
#if XCHAL_NUM_AREGS > 32
		"	.rept	(" __stringify(XCHAL_NUM_AREGS) " - 32) / 12\n"
		"	_entry	a1, 48\n"
		"	mov	a12, a0\n"
		"	.endr\n"
#endif
		"	_entry	a1, 48\n"
#if XCHAL_NUM_AREGS % 12 == 0
		"	mov	a8, a8\n"
#elif XCHAL_NUM_AREGS % 12 == 4
		"	mov	a12, a12\n"
#elif XCHAL_NUM_AREGS % 12 == 8
		"	mov	a4, a4\n"
#endif
		"	retw\n"
		"2:\n"
		: : : "a12", "a13", "memory");
=======
#if XCHAL_NUM_AREGS == 32
		"	_entry	a1, 32\n"
		"	addi	a8, a0, 3\n"
		"	_entry	a1, 16\n"
		"	mov	a12, a12\n"
		"	retw\n"
#else
		"	_entry	a1, 48\n"
		"	call12	1f\n"
		"	retw\n"
		"	.align	4\n"
		"1:\n"
		"	.rept	(" __stringify(XCHAL_NUM_AREGS) " - 16) / 12\n"
		"	_entry	a1, 48\n"
		"	mov	a12, a0\n"
		"	.endr\n"
		"	_entry	a1, 16\n"
#if XCHAL_NUM_AREGS % 12 == 0
		"	mov	a12, a12\n"
#elif XCHAL_NUM_AREGS % 12 == 4
		"	mov	a4, a4\n"
#elif XCHAL_NUM_AREGS % 12 == 8
		"	mov	a8, a8\n"
#endif
		"	retw\n"
#endif
		"2:\n"
		: : : "a8", "a9", "memory");
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#else
	__asm__ __volatile__ (
		"	mov	a12, a12\n"
		: : : "memory");
#endif
}

#endif /* _XTENSA_TRAPS_H */
