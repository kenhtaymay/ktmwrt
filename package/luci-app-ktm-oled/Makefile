#
# Copyright (C) 2020 Nate Ding
# Edit by Kenh Tay May
#
#
# This is free software, licensed under the GUN General Public License v3.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
PKG_NAME:=luci-app-ktm-oled
LUCI_Title:=LuCI support for ssd1306 0.91\' 138x32 display
LUCI_PKGARCH:=all
LUCI_DEPENDS:=+i2c-tools +coreutils-nohup +libuci +speedtest-cli
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_LICENSE:=GPLv3
PKG_LINCESE_FILES:=LICENSE
PKF_MAINTAINER:=natelol <natelol@github.com>

include $(TOPDIR)/feeds/luci/luci.mk

# call BuildPackage - OpenWrt buildroot signature
