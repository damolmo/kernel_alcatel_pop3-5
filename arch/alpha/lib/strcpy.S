/*
 * arch/alpha/lib/strcpy.S
 * Contributed by Richard Henderson (rth@tamu.edu)
 *
 * Copy a null-terminated string from SRC to DST.  Return a pointer
 * to the null-terminator in the source.
 */
<<<<<<< HEAD

=======
#include <asm/export.h>
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
	.text

	.align 3
	.globl strcpy
	.ent strcpy
strcpy:
	.frame $30, 0, $26
	.prologue 0

	mov	$16, $0		# set up return value
	mov	$26, $23	# set up return address
	unop
	br	__stxcpy	# do the copy

	.end strcpy
<<<<<<< HEAD
=======
	EXPORT_SYMBOL(strcpy)
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
