#ifndef _ASM_X86_CRASH_H
#define _ASM_X86_CRASH_H

<<<<<<< HEAD
=======
struct kimage;

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
int crash_load_segments(struct kimage *image);
int crash_copy_backup_region(struct kimage *image);
int crash_setup_memmap_entries(struct kimage *image,
		struct boot_params *params);

#endif /* _ASM_X86_CRASH_H */
