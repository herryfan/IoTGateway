#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ACE
PKG_VERSION:=6.3.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://download.dre.vanderbilt.edu/previous_versions

PKG_CAT:=zcat
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
ACE_ROOT:=$(PKG_BUILD_DIR)/ACE_wrappers
export ACE_ROOT
    
include $(INCLUDE_DIR)/package.mk

define Package/ACE
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:= ace library
  DEPENDS:=+libstdcpp
  URL:= http://download.dre.vanderbilt.edu/previous_versions
endef

define Package/ACE/description
	ACE library
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	cp -rf $(FILES_DIR)/* $(PKG_BUILD_DIR)/
	echo '#!/bin/sh' > $(PKG_BUILD_DIR)/build_ace.sh
	echo 'cp $(DL_DIR)/$(PKG_NAME)-$(PKG_VERSION).tar.gz $(PKG_BUILD_DIR)/' >> $(PKG_BUILD_DIR)/build_ace.sh
	echo 'tar xvf ./$(PKG_NAME)-$(PKG_VERSION).tar.gz' >> $(PKG_BUILD_DIR)/build_ace.sh
	echo 'export ACE_ROOT=$(PKG_BUILD_DIR)/ACE_wrappers' >> $(PKG_BUILD_DIR)/build_ace.sh
	chmod u+x $(PKG_BUILD_DIR)/build_ace.sh
	(cd $(PKG_BUILD_DIR) && ./build_ace.sh)
endef

define Build/Install
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/include/ace
	$(INSTALL_DIR) $(PKG_INSTALL_DIR)/usr/lib
	$(CP) -rf $(PKG_BUILD_DIR)/ACE_wrappers/lib/* $(PKG_INSTALL_DIR)/usr/lib
	$(CP) -rf $(PKG_BUILD_DIR)/ACE_wrappers/ace $(PKG_INSTALL_DIR)/usr/include
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/include/ace $(1)/usr/include/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libACE.so* $(1)/usr/lib/
endef

define Package/ACE/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libACE.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,ACE,+libstdcpp))
