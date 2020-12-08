#!/bin/sh
# Backup script for SHRP injection
# Part of this code belongs to TWRP's update-binary

ui_print() { $BOOTMODE && echo "$1" || echo -e "ui_print $1\nui_print" >> $OUTFD; }

# target partition without the slot suffix
target=/dev/block/bootdevice/by-name/boot;
# tmp folder, in order to not mess up
tmp=/dev/tmp/shrpinj;
rm -rf $tmp;
mkdir -p $tmp;

# let's cd into tmp
cd $tmp;

# Let's make some other folders
mkdir old_a;
mkdir old_b;
mkdir new_a;
mkdir new_b;

# cd into old dirs firstly, and run everything that's necessary, plus delete unnecessary files
for slot in a b; do
  slot=_$slot;
  cd old$slot;
  dd if=$target$slot of=boot$slot.img;
  magiskboot unpack -n -h boot$slot.img;
  rm -rf header kernel kernel_dtb boot$slot.img;
  cd $tmp;
done;
