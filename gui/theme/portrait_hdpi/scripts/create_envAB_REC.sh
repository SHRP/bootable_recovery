#!/bin/sh

#Creating env
mkdir -p tmp/work
cd tmp/work

#Pulling Recovery From Block
dd if=/dev/block/bootdevice/by-name/recovery of=recovery.img

#unpacking rec
/system/bin/magiskboot unpack -h recovery.img
