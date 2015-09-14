#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=hisi_x5hd
BOARDNAME:=hisilicon setbox hd chip (ARM)
FEATURES:=squashfs ext4 nand usb pci pcie gpio
MAINTAINER:=hisilicon
CPU_TYPE:=cortex-a9

KERNEL_PATCHVER:=3.18

include $(INCLUDE_DIR)/target.mk

KERNELNAME:=zImage dtbs

$(eval $(call BuildTarget))
