#!/bin/sh
# Restore script for SHRP injection
# Part of this code belongs to TWRP's update-binary

ui_print() { $BOOTMODE && echo "$1" || echo -e "ui_print $1\nui_print" >> $OUTFD; }
abort() { ui_print "$1"; exit 1; }

# target partition without the slot suffix
target=/dev/block/bootdevice/by-name/boot;
# tmp folder, in order to not mess up
tmp=/dev/tmp/shrpinj;

# let's copy our own magiskboot binary to our temporary dir
cp /sbin/magiskboot $tmp;
tool=$tmp/magiskboot;

# also copy magiskboot 19.3 to our temp dir, needed for hexpatches.
cp /sbin/magiskbootnt $tmp;
toolnt=$tmp/magiskbootnt;

# let's cd into tmp
cd $tmp;

# Let's backup new boot imgs firstly
for slots in a b; do
  slot=_$slots;
  cd new$slot;
  dd if=$target$slot of=bootnew$slot.img;
  magiskboot unpack -n -h bootnew$slot.img;
  cd $tmp;
done

# Fun part: replace every ramdisk.cpio, then reflash everything
for slots in a b; do
  cd $tmp;
  slot=_$slots;
  rm -rf new$slot/ramdisk.cpio;
  mv old$slot/ramdisk.cpio new$slot/ramdisk.cpio;
  cd new$slot;

  # kernel string want_initramfs -> skip_initramfs (Magisk)
  $toolnt hexpatch kernel 77616E745F696E697472616D6673 736B69705F696E697472616D6673 || ui_print "[D] 1st hexpatch failed.";
  ui_print "[D] 1st hexpatch went good!";
  # kernel string trip_initramfs -> skip_initramfs (SuperSU)
  $toolnt hexpatch kernel 747269705F696E697472616D6673 736B69705F696E697472616D6673 || ui_print "[D] 2nd hexpatch failed.";
  ui_print "[D] 2nd hexpatch went good!";

  # boot.img header cmdline remove skip_override (flar2 patch)
  sed -i "s|$(grep '^cmdline=' header | cut -d= -f2-)|$(grep '^cmdline=' header | cut -d= -f2- | sed -e 's/skip_override//' -e 's/  */ /g' -e 's/[ \t]*$//')|" header || ui_print "[D] 3rd patch (flar) failed";
  ui_print "[D] 3rd patch (flar) went good!";

  $tool repack bootnew$slot.img;
  dd if=new-boot.img of=$target$slot;
  rm -f new-boot.img;
done;

# Cleanup!
cd /;
rm -rf /dev/tmp;
