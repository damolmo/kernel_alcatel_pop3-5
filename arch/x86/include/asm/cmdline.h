#ifndef _ASM_X86_CMDLINE_H
#define _ASM_X86_CMDLINE_H

int cmdline_find_option_bool(const char *cmdline_ptr, const char *option);
<<<<<<< HEAD
=======
int cmdline_find_option(const char *cmdline_ptr, const char *option,
			char *buffer, int bufsize);
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

#endif /* _ASM_X86_CMDLINE_H */
