#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/kprobes.h>
#include <linux/stop_machine.h>

#include <asm/cacheflush.h>
=======
#include <linux/spinlock.h>
#include <linux/kprobes.h>
#include <linux/mm.h>
#include <linux/stop_machine.h>

#include <asm/cacheflush.h>
#include <asm/fixmap.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
#include <asm/smp_plat.h>
#include <asm/opcodes.h>

#include "patch.h"

struct patch {
	void *addr;
	unsigned int insn;
};

<<<<<<< HEAD
void __kprobes __patch_text(void *addr, unsigned int insn)
{
	bool thumb2 = IS_ENABLED(CONFIG_THUMB2_KERNEL);
	int size;

	if (thumb2 && __opcode_is_thumb16(insn)) {
		*(u16 *)addr = __opcode_to_mem_thumb16(insn);
		size = sizeof(u16);
	} else if (thumb2 && ((uintptr_t)addr & 2)) {
		u16 first = __opcode_thumb32_first(insn);
		u16 second = __opcode_thumb32_second(insn);
		u16 *addrh = addr;

		addrh[0] = __opcode_to_mem_thumb16(first);
		addrh[1] = __opcode_to_mem_thumb16(second);
=======
static DEFINE_RAW_SPINLOCK(patch_lock);

static void __kprobes *patch_map(void *addr, int fixmap, unsigned long *flags)
	__acquires(&patch_lock)
{
	unsigned int uintaddr = (uintptr_t) addr;
	bool module = !core_kernel_text(uintaddr);
	struct page *page;

	if (module && IS_ENABLED(CONFIG_DEBUG_SET_MODULE_RONX))
		page = vmalloc_to_page(addr);
	else if (!module && IS_ENABLED(CONFIG_DEBUG_RODATA))
		page = virt_to_page(addr);
	else
		return addr;

	if (flags)
		raw_spin_lock_irqsave(&patch_lock, *flags);
	else
		__acquire(&patch_lock);

	set_fixmap(fixmap, page_to_phys(page));

	return (void *) (__fix_to_virt(fixmap) + (uintaddr & ~PAGE_MASK));
}

static void __kprobes patch_unmap(int fixmap, unsigned long *flags)
	__releases(&patch_lock)
{
	clear_fixmap(fixmap);

	if (flags)
		raw_spin_unlock_irqrestore(&patch_lock, *flags);
	else
		__release(&patch_lock);
}

void __kprobes __patch_text_real(void *addr, unsigned int insn, bool remap)
{
	bool thumb2 = IS_ENABLED(CONFIG_THUMB2_KERNEL);
	unsigned int uintaddr = (uintptr_t) addr;
	bool twopage = false;
	unsigned long flags;
	void *waddr = addr;
	int size;

	if (remap)
		waddr = patch_map(addr, FIX_TEXT_POKE0, &flags);
	else
		__acquire(&patch_lock);

	if (thumb2 && __opcode_is_thumb16(insn)) {
		*(u16 *)waddr = __opcode_to_mem_thumb16(insn);
		size = sizeof(u16);
	} else if (thumb2 && (uintaddr & 2)) {
		u16 first = __opcode_thumb32_first(insn);
		u16 second = __opcode_thumb32_second(insn);
		u16 *addrh0 = waddr;
		u16 *addrh1 = waddr + 2;

		twopage = (uintaddr & ~PAGE_MASK) == PAGE_SIZE - 2;
		if (twopage && remap)
			addrh1 = patch_map(addr + 2, FIX_TEXT_POKE1, NULL);

		*addrh0 = __opcode_to_mem_thumb16(first);
		*addrh1 = __opcode_to_mem_thumb16(second);

		if (twopage && addrh1 != addr + 2) {
			flush_kernel_vmap_range(addrh1, 2);
			patch_unmap(FIX_TEXT_POKE1, NULL);
		}
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

		size = sizeof(u32);
	} else {
		if (thumb2)
			insn = __opcode_to_mem_thumb32(insn);
		else
			insn = __opcode_to_mem_arm(insn);

<<<<<<< HEAD
		*(u32 *)addr = insn;
		size = sizeof(u32);
	}

=======
		*(u32 *)waddr = insn;
		size = sizeof(u32);
	}

	if (waddr != addr) {
		flush_kernel_vmap_range(waddr, twopage ? size / 2 : size);
		patch_unmap(FIX_TEXT_POKE0, &flags);
	} else
		__release(&patch_lock);

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	flush_icache_range((uintptr_t)(addr),
			   (uintptr_t)(addr) + size);
}

static int __kprobes patch_text_stop_machine(void *data)
{
	struct patch *patch = data;

	__patch_text(patch->addr, patch->insn);

	return 0;
}

void __kprobes patch_text(void *addr, unsigned int insn)
{
	struct patch patch = {
		.addr = addr,
		.insn = insn,
	};

<<<<<<< HEAD
	if (cache_ops_need_broadcast()) {
		stop_machine(patch_text_stop_machine, &patch, cpu_online_mask);
	} else {
		bool straddles_word = IS_ENABLED(CONFIG_THUMB2_KERNEL)
				      && __opcode_is_thumb32(insn)
				      && ((uintptr_t)addr & 2);

		if (straddles_word)
			stop_machine(patch_text_stop_machine, &patch, NULL);
		else
			__patch_text(addr, insn);
	}
=======
	stop_machine(patch_text_stop_machine, &patch, NULL);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
}
