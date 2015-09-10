# 
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

define Image/BuildKernel
	cp $(DTS_DIR)/hisi-x5hd2-dkb.dtb $(BIN_DIR)
	cp $(KDIR)/zImage $(BIN_DIR)/$(IMG_PREFIX)-zImage
endef

# Build sysupgrade image
define BuildFirmware/Generic
	dd if=$(KDIR)/zImage of=$(KDIR)/zImage.pad bs=64k conv=sync; \
	dd if=$(KDIR)/root.$(1) of=$(KDIR)/root.$(1).pad bs=128k conv=sync; \
	sh $(TOPDIR)/scripts/combined-image.sh \
		$(KDIR)/zImage.pad \
		$(KDIR)/root.$(1).pad \
		$(BIN_DIR)/$(IMG_PREFIX)-$(patsubst squashfs-%,squashfs,$(1))-sysupgrade.bin
endef

KERNEL_LOADADDR=0x02000000

define Image/Build/MKuImage                                                                                                       
     mkimage -A $(LINUX_KARCH) \
         -O linux -T kernel \
         -C $(1) -a $(KERNEL_LOADADDR) -e $(KERNEL_LOADADDR) \
         -n '$(call toupper,$(LINUX_KARCH)) OpenWrt Linux-$(LINUX_VERSION)' -d $(KDIR)/zImage $(BIN_DIR)/$(IMG_PREFIX)-uImage
endef

define Image/Build
	$(call Image/Build/$(1),$(1))
	$(call BuildFirmware/Generic,$(1))
	$(call Image/Build/MKuImage, "none")
endef

define Image/Build/squashfs
	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(1).img bs=131072 conv=sync
endef

$(eval $(call BuildImage))
