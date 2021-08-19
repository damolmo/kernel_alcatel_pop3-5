#ifndef _ASM_X86_RMWcc
#define _ASM_X86_RMWcc

#ifdef CC_HAVE_ASM_GOTO

#define __GEN_RMWcc(fullop, var, cc, ...)				\
do {									\
<<<<<<< HEAD
	asm_volatile_goto (fullop "; j" cc " %l[cc_label]"		\
=======
	asm_volatile_goto (fullop "; j" #cc " %l[cc_label]"		\
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			: : "m" (var), ## __VA_ARGS__ 			\
			: "memory" : cc_label);				\
	return 0;							\
cc_label:								\
	return 1;							\
} while (0)

#define GEN_UNARY_RMWcc(op, var, arg0, cc) 				\
	__GEN_RMWcc(op " " arg0, var, cc)

#define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
	__GEN_RMWcc(op " %1, " arg0, var, cc, vcon (val))

#else /* !CC_HAVE_ASM_GOTO */

#define __GEN_RMWcc(fullop, var, cc, ...)				\
do {									\
	char c;								\
<<<<<<< HEAD
	asm volatile (fullop "; set" cc " %1"				\
=======
	asm volatile (fullop "; set" #cc " %1"				\
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
			: "+m" (var), "=qm" (c)				\
			: __VA_ARGS__ : "memory");			\
	return c != 0;							\
} while (0)

#define GEN_UNARY_RMWcc(op, var, arg0, cc)				\
	__GEN_RMWcc(op " " arg0, var, cc)

#define GEN_BINARY_RMWcc(op, var, vcon, val, arg0, cc)			\
	__GEN_RMWcc(op " %2, " arg0, var, cc, vcon (val))

#endif /* CC_HAVE_ASM_GOTO */

#endif /* _ASM_X86_RMWcc */
