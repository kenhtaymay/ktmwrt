#
# Copyright (C) 2022 by Kenh Tay May
#
#
# This is free software, licensed under the GUN General Public License v3.
# See /LICENSE for more information.
#
# Oled edited from https://github.com/NateLol/luci-app-oled

include $(TOPDIR)/rules.mk
PKG_NAME:=luci-app-ktm-tools
LUCI_Title:=LuCI support for ssd1306 0.91\' 138x32 display
LUCI_PKGARCH:=all
LUCI_DEPENDS:=+i2c-tools +coreutils-nohup +libuci +speedtest-cli +kmod-ipt-ipopt
PKG_VERSION:=1.0
PKG_RELEASE:=1.0

PKG_LICENSE:=GPLv3
PKG_LINCESE_FILES:=LICENSE
PKF_MAINTAINER:=han <tin@hs2t.com>

include $(TOPDIR)/feeds/luci/luci.mk

# call BuildPackage - OpenWrt buildroot signature
