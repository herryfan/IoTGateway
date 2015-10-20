#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libcoap
PKG_VERSION:=4.1.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/libcoap

PKG_CAT:=zcat
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
    
include $(INCLUDE_DIR)/package.mk

define Package/libcoap
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:= coap library
  URL:= @SF/libcoap
endef

define Package/libcoap/description
	coap library
endef

define Build/Install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/include
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/lib
	$(CP) $(PKG_BUILD_DIR)/*.h $(PKG_INSTALL_DIR)/include
	$(CP) $(PKG_BUILD_DIR)/lib*.* $(PKG_INSTALL_DIR)/lib
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/include/*.h $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/lib/lib*.* $(1)/usr/lib/
endef

define Package/libcoap/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/lib/lib*.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libcoap))
