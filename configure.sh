#!/bin/bash
# Modify default system settings

# change default IP to 192.168.8.1
sed -i 's/192.168.1.1/192.168.8.1/g' package/base-files/files/bin/config_generate

# passwall
echo "src-git helloworld https://github.com/fw876/helloworld" >> feeds.conf.default