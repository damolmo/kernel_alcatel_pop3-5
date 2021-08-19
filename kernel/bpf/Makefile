obj-y := core.o
<<<<<<< HEAD
=======
CFLAGS_core.o += $(call cc-disable-warning, override-init)

>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
obj-$(CONFIG_BPF_SYSCALL) += syscall.o verifier.o
ifdef CONFIG_TEST_BPF
obj-$(CONFIG_BPF_SYSCALL) += test_stub.o
endif
