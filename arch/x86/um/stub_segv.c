/*
 * Copyright (C) 2004 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <sysdep/stub.h>
#include <sysdep/faultinfo.h>
#include <sysdep/mcontext.h>

void __attribute__ ((__section__ (".__syscall_stub")))
stub_segv_handler(int sig, siginfo_t *info, void *p)
{
<<<<<<< HEAD
	struct ucontext *uc = p;
=======
	ucontext_t *uc = p;
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

	GET_FAULTINFO_FROM_MC(*((struct faultinfo *) STUB_DATA),
			      &uc->uc_mcontext);
	trap_myself();
}

