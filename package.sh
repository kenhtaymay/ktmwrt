#!/bin/bash

# Add luci-app-smstools3
git clone --depth=1 https://github.com/koshev-msk/luci-app-smstools3

# Add luci-app-mmconfig : configure modem cellular bands via mmcli utility
git clone --depth=1 https://github.com/koshev-msk/luci-app-mmconfig

echo $PWD
mv $GITHUB_WORKSPACE/package/luci-app-ktm-tools .
