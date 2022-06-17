#!/bin/bash
# Modify default system settings

# change default IP to 192.168.8.1
sed -i 's/192.168.1.1/192.168.8.1/g' package/base-files/files/bin/config_generate
sed -i 's/ImmortalWrt/KTMWrt/g' package/base-files/files/bin/config_generate

# passwall
echo "src-git kenzok8 https://github.com/kenzok8/openwrt-packages" >> feeds.conf.default
echo "src-git modeminfo https://github.com/koshev-msk/luci-app-modeminfo.git" >> feeds.conf.default