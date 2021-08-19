#
# The stub may be linked into the kernel proper or into a separate boot binary,
# but in either case, it executes before the kernel does (with MMU disabled) so
# things like ftrace and stack-protector are likely to cause trouble if left
# enabled, even if doing so doesn't break the build.
#
cflags-$(CONFIG_X86_32)		:= -march=i386
cflags-$(CONFIG_X86_64)		:= -mcmodel=small
cflags-$(CONFIG_X86)		+= -m$(BITS) -D__KERNEL__ $(LINUX_INCLUDE) -O2 \
				   -fPIC -fno-strict-aliasing -mno-red-zone \
<<<<<<< HEAD
				   -mno-mmx -mno-sse -DDISABLE_BRANCH_PROFILING
=======
				   -mno-mmx -mno-sse
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

cflags-$(CONFIG_ARM64)		:= $(subst -pg,,$(KBUILD_CFLAGS))
cflags-$(CONFIG_ARM)		:= $(subst -pg,,$(KBUILD_CFLAGS)) \
				   -fno-builtin -fpic -mno-single-pic-base

<<<<<<< HEAD
KBUILD_CFLAGS			:= $(cflags-y) \
=======
KBUILD_CFLAGS			:= $(cflags-y) -DDISABLE_BRANCH_PROFILING \
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
				   $(call cc-option,-ffreestanding) \
				   $(call cc-option,-fno-stack-protector)

GCOV_PROFILE			:= n

lib-y				:= efi-stub-helper.o
lib-$(CONFIG_EFI_ARMSTUB)	+= arm-stub.o fdt.o

CFLAGS_fdt.o			+= -I$(srctree)/scripts/dtc/libfdt/
