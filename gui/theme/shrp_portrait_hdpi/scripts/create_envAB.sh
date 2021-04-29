#!/bin/sh

#Creating env
mkdir -p tmp/work
cd tmp/work

#Pulling Recovery From Block
dd if=/dev/block/bootdevice/by-name/boot of=recovery.img

#unpacking rec
/system/bin/magiskboot unpack -h recovery.img
