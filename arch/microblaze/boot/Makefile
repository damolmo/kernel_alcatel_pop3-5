#
# arch/microblaze/boot/Makefile
#

targets := linux.bin linux.bin.gz linux.bin.ub simpleImage.%

OBJCOPYFLAGS := -R .note -R .comment -R .note.gnu.build-id -O binary

$(obj)/linux.bin: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.ub: $(obj)/linux.bin FORCE
	$(call if_changed,uimage)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

$(obj)/linux.bin.gz: $(obj)/linux.bin FORCE
	$(call if_changed,gzip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'

quiet_cmd_cp = CP      $< $@$2
	cmd_cp = cat $< >$@$2 || (rm -f $@ && echo false)

<<<<<<< HEAD
quiet_cmd_strip = STRIP   $@
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start vmlinux -o $@

UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)
=======
quiet_cmd_strip = STRIP   $< $@$2
	cmd_strip = $(STRIP) -K microblaze_start -K _end -K __log_buf \
				-K _fdt_start $< -o $@$2

UIMAGE_LOADADDR = $(CONFIG_KERNEL_BASE_ADDR)
UIMAGE_IN = $@
UIMAGE_OUT = $@.ub
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916

$(obj)/simpleImage.%: vmlinux FORCE
	$(call if_changed,cp,.unstrip)
	$(call if_changed,objcopy)
	$(call if_changed,uimage)
<<<<<<< HEAD
	$(call if_changed,strip)
	@echo 'Kernel: $@ is ready' ' (#'`cat .version`')'
=======
	$(call if_changed,strip,.strip)
	@echo 'Kernel: $(UIMAGE_OUT) is ready' ' (#'`cat .version`')'
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916


clean-files += simpleImage.*.unstrip linux.bin.ub
