#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/Generic
  NAME:=hisilicon xh5d soc
  PACKAGES:=
endef

define Profile/Generic/Description
	Package set compatible with x5hd
endef

$(eval $(call Profile,Generic))

