#!/bin/bash

#get changes from  GitHub 
echo 'Get changes from GitHub'
git pull
echo 'Start building...'

#Downloading Toolchain
echo 'Downloading Toolchain to build'
git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8 ~/arm-eabi-4.8
echo 'Done! Downloaded Toolchain Succefully'

# Set options
export ARCH=arm
export CROSS_COMPILE=~/arm-eabi-4.8/bin/arm-eabi-
export KBUILD_BUILD_USER=Yuvraj
export KBUILD_BUILD_HOST=TheThestral

#For checking errors
echo 'Removing Existing kernel...'
rm -rf arch/arm/boot/zImage


echo 'Configuring Defconfig'
make aq5_defconfig >/dev/null
echo 'Building zImage'
make -j4 zImage >/dev/null 2>aq5-$(date +%Y%m%d).log

#check errors
if [ ! -f arch/arm/boot/zImage ]
then
    echo "BUILD ERRORS!"
    megaput aq5-$(date +%Y%m%d).log
else
    echo "BUILD Completed!"
    echo "zipping zImage"
    cp arch/arm/boot/zImage AIK/
    cd AIK && ./simulate.sh
    cd ../
    megaput aq5-$(date +%Y%m%d).img
fi
#write worktime
echo $[$SECONDS / 60]
echo 'minutes'
echo $[$SECONDS % 60]
echo 'seconds'
