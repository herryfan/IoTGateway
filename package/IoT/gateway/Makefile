#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=IoTGateway
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/IoTGateway
  SECTION:=net
  CATEGORY:=Network
  TITLE:= IoT Gateway service
  URL:= https://github.com/hisilicon
endef

define Package/IoTGateway/description
	IoT Gateway service
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/source $(PKG_BUILD_DIR)
	$(CP) ./files/Makefile $(PKG_BUILD_DIR)
endef

#define Build/Compile
#	$(MAKE) -C $(PKG_BUILD_DIR)
#endef

define Build/Install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/gateway $(PKG_INSTALL_DIR)/usr/bin
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/gateway $(1)/usr/bin
endef
define Package/$(PKG_NAME)/Install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,IoTGateway))
