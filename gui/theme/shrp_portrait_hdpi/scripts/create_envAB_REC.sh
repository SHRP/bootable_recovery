#!/bin/sh

#Creating env
mkdir -p tmp/work
cd tmp/work

#Pulling Recovery From Block
dd if=/dev/block/bootdevice/by-name/recovery of=recovery.img

#unpacking rec
magiskboot unpack -h recovery.img