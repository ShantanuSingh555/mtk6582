#!/bin/bash

./unpackimg.sh recovery.img
rm split_img/recovery.img-zImage
cp zImage split_img/
rm zImage
cd split_img
mv zImage recovery.img-zImage
chmod 664 recovery.img-zImage
cd .. 
./repackimg.sh
cp image-new.img ../
cd ../
mv image-new.img aq5-$(date +%Y%m%d).img
