#!/bin/bash

# Compiles fftw3 for Android
# Make sure you have NDK_DIR defined in .bashrc or .bash_profile

echo "++++++++++++++++++++++++++++++++++++++++++++"
echo "Build Thirdparty library $1 $2"
echo "++++++++++++++++++++++++++++++++++++++++++++"

show_help() {
	echo "Usage: bulid_android.sh --arch=VALUE1 --platform=VALUE2 [--ndk_path=VALUE3 | --compiler_prefix=VALUE4] [--mode=debug, release default]"
	echo "e.g.: For android platform"
	echo "e.g.: ./build.sh --arch=armv7-a --platform=android --ndk_path=/home/gsc/android-ndk-r12b [--mode=debug]"
	echo "e.g.: For linux platform"
	echo "e.g.: ./build.sh --arch=arm --platform=linux --compiler_prefix=arm-openwrt-linux- --compiler_path=/path [--mode=release]"
	echo "e.g.: For x86 platform"
	echo "e.g.: For x86_64  ./build.sh --arch=x86_64 --platform=linux [--mode=debug]"
}

if [ $# -lt 2 ] || [ $# -gt 5 ]; then
	show_help
	exit 1;
fi

while :; do
	case "$1" in
		-h|--help)
			show_help
			exit
			;;
		--arch=?*)
			ARCH=${1#*=}
			;;
		--platform=?*)
			PLATFORM=${1#*=}
			;;
		--ndk_path=?*)
			NDK_PATH=${1#*=}
			;;
		--compiler_prefix=?*)
			COMPILER_PREFIX=${1#*=}
			;;
		--compiler_path=?*)
			COMPILER_PATH=${1#*=}
			;;
		--mode=?*)
			BUILD_MODE=${1#*=}
			;;
		*)
			break
			;;
	esac
	shift
done

if [ "${ARCH}" = "x86_64" ] ; then
	ARCH=x86_64
fi

CROSS_HOST=${COMPILER_PREFIX%?}

##################################################export android 
if [ "$PLATFORM" = "android" ]; then
export NDK_PATH=${NDK_PATH}
export PATH="$NDK_PATH/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/:$PATH"
fi

export THIRD_LIB=`pwd`/3rdparty

BUILDROOT_DIR=`pwd`

if [ ! -d build ]; then
	mkdir build && cd build
else
	cd build
fi

if [ "${BUILD_MODE}" != "debug" ]; then
	BUILD_MODE="release"
fi

if [ "${PLATFORM}" = "linux" ]; then
	export CROSS_HOST="${CROSS_HOST}"
	export COMPILER_PATH="${COMPILER_PATH}"
	cmake -DGNULINUX_PLATFORM=ON -DTARGET_ARCH=${ARCH} -DBUILD_MODE=${BUILD_MODE} -DCOMPILER_PATH=${COMPILER_PATH} -DCOMPILER_PREFIX=${COMPILER_PREFIX} ..
elif [ "${PLATFORM}" = "android" ]; then
	cmake -DANDROID_PLATFORM=ON -DTARGET_ARCH=${ARCH} -DBUILD_MODE=${BUILD_MODE} ..
fi

make VERBOSE=1 

