/*
 * Cache flush operations for the Hexagon architecture
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef _ASM_CACHEFLUSH_H
#define _ASM_CACHEFLUSH_H

<<<<<<< HEAD
#include <linux/cache.h>
#include <linux/mm.h>
#include <asm/string.h>
#include <asm-generic/cacheflush.h>
=======
#include <linux/mm_types.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

/* Cache flushing:
 *
 *  - flush_cache_all() flushes entire cache
 *  - flush_cache_mm(mm) flushes the specified mm context's cache lines
 *  - flush_cache_page(mm, vmaddr, pfn) flushes a single page
 *  - flush_cache_range(vma, start, end) flushes a range of pages
 *  - flush_icache_range(start, end) flush a range of instructions
 *  - flush_dcache_page(pg) flushes(wback&invalidates) a page for dcache
 *  - flush_icache_page(vma, pg) flushes(invalidates) a page for icache
 *
 *  Need to doublecheck which one is really needed for ptrace stuff to work.
 */
#define LINESIZE	32
#define LINEBITS	5

<<<<<<< HEAD
=======
#define flush_cache_all()			do { } while (0)
#define flush_cache_mm(mm)			do { } while (0)
#define flush_cache_dup_mm(mm)			do { } while (0)
#define flush_cache_range(vma, start, end)	do { } while (0)
#define flush_cache_page(vma, vmaddr, pfn)	do { } while (0)
#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 0
#define flush_dcache_page(page)			do { } while (0)
#define flush_dcache_mmap_lock(mapping)		do { } while (0)
#define flush_dcache_mmap_unlock(mapping)	do { } while (0)
#define flush_icache_page(vma, pg)		do { } while (0)
#define flush_icache_user_range(vma, pg, adr, len)	do { } while (0)
#define flush_cache_vmap(start, end)		do { } while (0)
#define flush_cache_vunmap(start, end)		do { } while (0)

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/*
 * Flush Dcache range through current map.
 */
extern void flush_dcache_range(unsigned long start, unsigned long end);

/*
 * Flush Icache range through current map.
 */
<<<<<<< HEAD
#undef flush_icache_range
=======
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
extern void flush_icache_range(unsigned long start, unsigned long end);

/*
 * Memory-management related flushes are there to ensure in non-physically
 * indexed cache schemes that stale lines belonging to a given ASID aren't
 * in the cache to confuse things.  The prototype Hexagon Virtual Machine
 * only uses a single ASID for all user-mode maps, which should
 * mean that they aren't necessary.  A brute-force, flush-everything
 * implementation, with the name xxxxx_hexagon() is present in
 * arch/hexagon/mm/cache.c, but let's not wire it up until we know
 * it is needed.
 */
extern void flush_cache_all_hexagon(void);

/*
 * This may or may not ever have to be non-null, depending on the
 * virtual machine MMU.  For a native kernel, it's definitiely  a no-op
 *
 * This is also the place where deferred cache coherency stuff seems
 * to happen, classically...  but instead we do it like ia64 and
 * clean the cache when the PTE is set.
 *
 */
static inline void update_mmu_cache(struct vm_area_struct *vma,
					unsigned long address, pte_t *ptep)
{
	/*  generic_ptrace_pokedata doesn't wind up here, does it?  */
}

<<<<<<< HEAD
#undef copy_to_user_page
static inline void copy_to_user_page(struct vm_area_struct *vma,
					     struct page *page,
					     unsigned long vaddr,
					     void *dst, void *src, int len)
{
	memcpy(dst, src, len);
	if (vma->vm_flags & VM_EXEC) {
		flush_icache_range((unsigned long) dst,
		(unsigned long) dst + len);
	}
}

=======
void copy_to_user_page(struct vm_area_struct *vma, struct page *page,
		       unsigned long vaddr, void *dst, void *src, int len);

#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	memcpy(dst, src, len)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

extern void hexagon_inv_dcache_range(unsigned long start, unsigned long end);
extern void hexagon_clean_dcache_range(unsigned long start, unsigned long end);

#endif
