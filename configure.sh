#!/bin/bash
# Modify default system settings

# change default IP to 192.168.8.1
# uci set system.cfg01e48a.zonename='Asia/Ho Chi Minh'
# uci set system.cfg01e48a.timezone='<+07>-7'
sed -i 's/192.168.1.1/192.168.8.1/g' package/base-files/files/bin/config_generate
sed -i 's/ImmortalWrt/KTMWrt/g' package/base-files/files/bin/config_generate
sed -i 's/UTC/<+07>-7/g' package/base-files/files/bin/config_generate

# passwall
echo "src-git kenzok8 https://github.com/kenhtaymay/openwrt-packages.git" >> feeds.conf.default
echo "src-git modemfeed https://github.com/kenhtaymay/modemfeed.git" >> feeds.conf.default