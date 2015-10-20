#
# script for building openwrt image
#
#!/bin/sh

LOCAL_PATH=`pwd`
WORK_PATH=$LOCAL_PATH/openwrt-code

#create work folder
mkdir -p $WORK_PATH

if [ $? -ne 0 ]; then 
	echo "Can't create folder: $WORK_PATH, Please check  if you have sudo access."
	exit
fi

#goto work folder
echo "goto $WORK_PATH...."
cd $WORK_PATH

#get openwrt source code
echo "======================= download openwrt objects ==================="

CODE_URL=svn://svn.openwrt.org.cn/openwrt/trunk 
TRUNK=46386
LOCAL_TRUNK=openwrt-$TRUNK-code

svn co $CODE_URL -r $TRUNK $LOCAL_TRUNK

if [ $? -ne 0 ]; then
	echo "failed to download openwrt objects..., please check." 
	exit
fi

#get hi3718 target 
CONFIG_FILE=$LOCAL_PATH/target/config
CONFIG_DIST_FILE=$WORK_PATH/$LOCAL_TRUNK/.config
TARGET_DIR=$LOCAL_PATH/target/hisi_x5hd
TARGET_DIST_DIR=$WORK_PATH/$LOCAL_TRUNK/target/linux
LIBS_PACKAGE_DIR=$LOCAL_PATH/package
LIBS_PACKAGE_DIST_DIR=$WORK_PATH/$LOCAL_TRUNK/package/libs

echo $CONFIG_FILE

if [ -f $CONFIG_FILE ]; then
	cp -f $CONFIG_FILE $CONFIG_DIST_FILE
	cp -r $TARGET_DIR $TARGET_DIST_DIR	

else
	echo "The target and config for hisilicion are not existed, please check."
	exit
fi

#update openwrt packages
echo "======================= update openwrt pakcages ====================="
cd ./$LOCAL_TRUNK

./scripts/feeds update -a
./scripts/feeds install -a

cd -

#put our owns packages into openwrt package
echo "put local package into openwrt...."
cp -rf $LIBS_PACKAGE_DIR/* $LIBS_PACKAGE_DIST_DIR/

#make openwrt image 
echo "======================= building openwrt image ====================="

cd ./$LOCAL_TRUNK

cp -f $CONFIG_FILE $CONFIG_DIST_FILE

make V=99

