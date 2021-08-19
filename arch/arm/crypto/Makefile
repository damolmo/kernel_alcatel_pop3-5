#
# Arch-specific CryptoAPI modules.
#

obj-$(CONFIG_CRYPTO_AES_ARM) += aes-arm.o
obj-$(CONFIG_CRYPTO_AES_ARM_BS) += aes-arm-bs.o
<<<<<<< HEAD
obj-$(CONFIG_CRYPTO_AES_ARM32_CE) += aes-arm-ce.o
=======
obj-$(CONFIG_CRYPTO_AES_ARM_CE) += aes-arm-ce.o
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
obj-$(CONFIG_CRYPTO_SHA1_ARM) += sha1-arm.o
obj-$(CONFIG_CRYPTO_SHA1_ARM_NEON) += sha1-arm-neon.o
obj-$(CONFIG_CRYPTO_SHA256_ARM) += sha256-arm.o
obj-$(CONFIG_CRYPTO_SHA512_ARM_NEON) += sha512-arm-neon.o
<<<<<<< HEAD

ce-obj-$(CONFIG_CRYPTO_SHA2_ARM_CE) += sha2-arm-ce.o

ifneq ($(ce-obj-y)$(ce-obj-m),)
ifeq ($(call as-instr,.fpu crypto-neon-fp-armv8,y,n),y)
obj-y += $(ce-obj-y)
obj-m += $(ce-obj-m)
else
$(warning These ARMv8 Crypto Extensions modules need binutils 2.23 or higher)
$(warning $(ce-obj-y) $(ce-obj-m))
endif
endif

aes-arm-y  := aes-armv4.o aes_glue.o
aes-arm-bs-y	:= aesbs-core.o aesbs-glue.o
#aes-arm-ce-y  := aes-armv8-aarch32.o aes-armv8-aarch32-glue.o aes-armv8-bcm.o aes-armv8-bcm-glue.o
aes-arm-ce-y  := aes-armv8-bcm.o aes-armv8-bcm-glue.o ### Disable AES-CE for kernel_neon_begin( ) KE with network.
=======
obj-$(CONFIG_CRYPTO_SHA2_ARM_CE) += sha2-arm-ce.o

aes-arm-y	:= aes-armv4.o aes_glue.o
aes-arm-bs-y	:= aesbs-core.o aesbs-glue.o
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
sha1-arm-y	:= sha1-armv4-large.o sha1_glue.o
sha1-arm-neon-y	:= sha1-armv7-neon.o sha1_neon_glue.o
sha256-arm-neon-$(CONFIG_KERNEL_MODE_NEON) := sha256_neon_glue.o
sha256-arm-y	:= sha256-core.o sha256_glue.o $(sha256-arm-neon-y)
sha512-arm-neon-y := sha512-armv7-neon.o sha512_neon_glue.o
<<<<<<< HEAD
=======
aes-arm-ce-y	:= aes-ce-core.o aes-ce-glue.o
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
sha2-arm-ce-y	:= sha2-ce-core.o sha2-ce-glue.o

quiet_cmd_perl = PERL    $@
      cmd_perl = $(PERL) $(<) > $(@)

$(src)/aesbs-core.S_shipped: $(src)/bsaes-armv7.pl
	$(call cmd,perl)

$(src)/sha256-core.S_shipped: $(src)/sha256-armv4.pl
	$(call cmd,perl)

<<<<<<< HEAD

.PRECIOUS: $(obj)/sha256-core.S
=======
.PRECIOUS: $(obj)/aesbs-core.S $(obj)/sha256-core.S
>>>>>>> 21c1bccd7c23ac9673b3f0dd0f8b4f78331b3916
