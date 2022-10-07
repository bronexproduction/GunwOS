#!/bin/bash

set -e

#VBoxManage.exe storageattach GunwOS --storagectl Floppy --port 0 --device 0 --medium none
#VBoxManage.exe closemedium disk build/GunwOS.vmdk --delete
#VBoxManage.exe convertdd build/gunwos.img build/GunwOS.vmdk --format VMDK
#VBoxManage.exe storageattach GunwOS --storagectl Floppy --port 0 --device 0 --medium build/GunwOS.vmdk --type fdd
VBoxManage.exe startvm --putenv VBOX_GUI_DBG_ENABLED=true GunwOS
