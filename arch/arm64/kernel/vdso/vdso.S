/*
 * Copyright (C) 2012 ARM Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Will Deacon <will.deacon@arm.com>
 */

#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/const.h>
#include <asm/page.h>

<<<<<<< HEAD
	__PAGE_ALIGNED_DATA

	.globl vdso_start, vdso_end
=======
	.globl vdso_start, vdso_end
	.section .rodata
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	.balign PAGE_SIZE
vdso_start:
	.incbin "arch/arm64/kernel/vdso/vdso.so"
	.balign PAGE_SIZE
vdso_end:

	.previous
