/*
 * ld script to make ARM Linux kernel
 * taken from the i386 version by Russell King
 * Written by Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */

#include <asm-generic/vmlinux.lds.h>
<<<<<<< HEAD
#include <asm/thread_info.h>
#include <asm/memory.h>
#include <asm/page.h>

#include "image.h"

#define ARM_EXIT_KEEP(x)	x
#define ARM_EXIT_DISCARD(x)
=======
#include <asm/kernel-pgtable.h>
#include <asm/thread_info.h>
#include <asm/memory.h>
#include <asm/page.h>
#include <asm/pgtable.h>

#include "image.h"

#define ARM_EXIT_KEEP(x)
#define ARM_EXIT_DISCARD(x)	x
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

OUTPUT_ARCH(aarch64)
ENTRY(_text)

jiffies = jiffies_64;

#define HYPERVISOR_TEXT					\
	/*						\
	 * Force the alignment to be compatible with	\
	 * the vectors requirements			\
	 */						\
	. = ALIGN(2048);				\
	VMLINUX_SYMBOL(__hyp_idmap_text_start) = .;	\
	*(.hyp.idmap.text)				\
	VMLINUX_SYMBOL(__hyp_idmap_text_end) = .;	\
	VMLINUX_SYMBOL(__hyp_text_start) = .;		\
	*(.hyp.text)					\
	VMLINUX_SYMBOL(__hyp_text_end) = .;

<<<<<<< HEAD
=======
#ifdef CONFIG_UNMAP_KERNEL_AT_EL0
#define TRAMP_TEXT					\
	. = ALIGN(PAGE_SIZE);				\
	VMLINUX_SYMBOL(__entry_tramp_text_start) = .;	\
	*(.entry.tramp.text)				\
	. = ALIGN(PAGE_SIZE);				\
	VMLINUX_SYMBOL(__entry_tramp_text_end) = .;
#else
#define TRAMP_TEXT
#endif

/*
 * The size of the PE/COFF section that covers the kernel image, which
 * runs from stext to _edata, must be a round multiple of the PE/COFF
 * FileAlignment, which we set to its minimum value of 0x200. 'stext'
 * itself is 4 KB aligned, so padding out _edata to a 0x200 aligned
 * boundary should be sufficient.
 */
PECOFF_FILE_ALIGNMENT = 0x200;

#ifdef CONFIG_EFI
#define PECOFF_EDATA_PADDING	\
	.pecoff_edata_padding : { BYTE(0); . = ALIGN(PECOFF_FILE_ALIGNMENT); }
#else
#define PECOFF_EDATA_PADDING
#endif

#if defined(CONFIG_DEBUG_ALIGN_RODATA)
#define ALIGN_DEBUG_RO			. = ALIGN(1<<SECTION_SHIFT);
#define ALIGN_DEBUG_RO_MIN(min)		ALIGN_DEBUG_RO
#elif defined(CONFIG_DEBUG_RODATA)
#define ALIGN_DEBUG_RO			. = ALIGN(1<<PAGE_SHIFT);
#define ALIGN_DEBUG_RO_MIN(min)		ALIGN_DEBUG_RO
#else
#define ALIGN_DEBUG_RO
#define ALIGN_DEBUG_RO_MIN(min)		. = ALIGN(min);
#endif

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
SECTIONS
{
	/*
	 * XXX: The linker does not define how output sections are
	 * assigned to input sections when there are multiple statements
	 * matching the same input section name.  There is no documented
	 * order of matching.
	 */
	/DISCARD/ : {
		ARM_EXIT_DISCARD(EXIT_TEXT)
		ARM_EXIT_DISCARD(EXIT_DATA)
		EXIT_CALL
		*(.discard)
		*(.discard.*)
	}

	. = PAGE_OFFSET + TEXT_OFFSET;

	.head.text : {
		_text = .;
		HEAD_TEXT
	}
<<<<<<< HEAD
#ifdef CONFIG_MTK_AEE_MRDUMP
	.mrdump ALIGN (0x400): {
	}
#endif
=======
	ALIGN_DEBUG_RO
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	.text : {			/* Real text segment		*/
		_stext = .;		/* Text and read-only data	*/
			__exception_text_start = .;
			*(.exception.text)
			__exception_text_end = .;
			IRQENTRY_TEXT
			TEXT_TEXT
			SCHED_TEXT
			LOCK_TEXT
			HYPERVISOR_TEXT
<<<<<<< HEAD
=======
			TRAMP_TEXT
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			*(.fixup)
			*(.gnu.warning)
		. = ALIGN(16);
		*(.got)			/* Global offset table		*/
	}

<<<<<<< HEAD
	RO_DATA(PAGE_SIZE)
	EXCEPTION_TABLE(8)
	NOTES
	_etext = .;			/* End of text and rodata section */

	. = ALIGN(PAGE_SIZE);
=======
	ALIGN_DEBUG_RO
	_etext = .;			/* End of text section */

	RO_DATA(PAGE_SIZE)
	EXCEPTION_TABLE(8)
	NOTES
	ALIGN_DEBUG_RO

	ALIGN_DEBUG_RO_MIN(PAGE_SIZE)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	__init_begin = .;

	INIT_TEXT_SECTION(8)
	.exit.text : {
		ARM_EXIT_KEEP(EXIT_TEXT)
	}
<<<<<<< HEAD
	. = ALIGN(16);
=======

	ALIGN_DEBUG_RO_MIN(16)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	.init.data : {
		INIT_DATA
		INIT_SETUP(16)
		INIT_CALLS
		CON_INITCALL
		SECURITY_INITCALL
		INIT_RAM_FS
	}
	.exit.data : {
		ARM_EXIT_KEEP(EXIT_DATA)
	}

	PERCPU_SECTION(64)

	. = ALIGN(PAGE_SIZE);
	__init_end = .;

	. = ALIGN(4);
	.altinstructions : {
		__alt_instructions = .;
		*(.altinstructions)
		__alt_instructions_end = .;
	}
	.altinstr_replacement : {
		*(.altinstr_replacement)
	}

	. = ALIGN(PAGE_SIZE);
	_data = .;
	_sdata = .;
	RW_DATA_SECTION(64, PAGE_SIZE, THREAD_SIZE)
<<<<<<< HEAD
=======
	PECOFF_EDATA_PADDING
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	_edata = .;

	BSS_SECTION(0, 0, 0)

	. = ALIGN(PAGE_SIZE);
	idmap_pg_dir = .;
	. += IDMAP_DIR_SIZE;
	swapper_pg_dir = .;
	. += SWAPPER_DIR_SIZE;

<<<<<<< HEAD
=======
#ifdef CONFIG_ARM64_SW_TTBR0_PAN
	reserved_ttbr0 = .;
	. += RESERVED_TTBR0_SIZE;
#endif

#ifdef CONFIG_UNMAP_KERNEL_AT_EL0
	tramp_pg_dir = .;
	. += PAGE_SIZE;
#endif

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	_end = .;

	STABS_DEBUG

	HEAD_SYMBOLS
}

/*
 * The HYP init code can't be more than a page long.
 */
ASSERT(((__hyp_idmap_text_start + PAGE_SIZE) > __hyp_idmap_text_end),
       "HYP init code too big")

<<<<<<< HEAD
=======
#ifdef CONFIG_UNMAP_KERNEL_AT_EL0
ASSERT((__entry_tramp_text_end - __entry_tramp_text_start) == PAGE_SIZE,
	"Entry trampoline text too big")
#endif
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
/*
 * If padding is applied before .head.text, virt<->phys conversions will fail.
 */
ASSERT(_text == (PAGE_OFFSET + TEXT_OFFSET), "HEAD is misaligned")
