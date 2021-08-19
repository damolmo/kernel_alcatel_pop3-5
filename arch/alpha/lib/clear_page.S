/*
 * arch/alpha/lib/clear_page.S
 *
 * Zero an entire page.
 */
<<<<<<< HEAD

=======
#include <asm/export.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	.text
	.align 4
	.global clear_page
	.ent clear_page
clear_page:
	.prologue 0

	lda	$0,128
	nop
	unop
	nop

1:	stq	$31,0($16)
	stq	$31,8($16)
	stq	$31,16($16)
	stq	$31,24($16)

	stq	$31,32($16)
	stq	$31,40($16)
	stq	$31,48($16)
	subq	$0,1,$0

	stq	$31,56($16)
	addq	$16,64,$16
	unop
	bne	$0,1b

	ret
	nop
	unop
	nop

	.end clear_page
<<<<<<< HEAD
=======
	EXPORT_SYMBOL(clear_page)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
