# For X86_64 compilation
if(TARGET_ARCH STREQUAL "x86_64" AND GNULINUX_PLATFORM)
	message("-- TARGET_ARCH type: ${TARGET_ARCH}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2")

#For 32bit ARM&linux compilation
elseif( (TARGET_ARCH STREQUAL "armv7-a" AND GNULINUX_PLATFORM) OR (TARGET_ARCH STREQUAL "arm" AND GNULINUX_PLATFORM))
	#	SET(ASM_OPTIONS "-x assembler-with-cpp")
	#	SET(ASM_OPTIONS "-x assembler-with-c")
	#set(CMAKE_C_FLAGS "-march=armv7-a -mfloat-abi=hard -mfpu=vfp3")
	set(CMAKE_C_FLAGS "-march=armv7-a -mfloat-abi=hard -g -mfpu=neon -fPIC -falign-functions=16 -falign-loops=16")
	set(CMAKE_CXX_FLAGS "-std=c++11 -march=armv7-a -g -mfloat-abi=hard -mfpu=neon -fPIC -fpermissive -falign-functions=16 -falign-loops=16")
	SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CFLAGS}")
	#set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${ASM_OPTIONS}")
	set(CMAKE_C_COMPILER "${COMPILER_PATH}${COMPILER_PREFIX}gcc")
	set(CMAKE_CXX_COMPILER "${COMPILER_PATH}${COMPILER_PREFIX}g++")
	set(CMAKE_ASM_COMPILER "${COMPILER_PATH}${COMPILER_PREFIX}as")
	find_program(CMAKE_AR NAMES "${COMPILER_PREFIX}ar")
	find_program(CMAKE_RANLIB NAMES "${COMPILER_PREFIX}ranlib")
	set(CMAKE_FIND_ROOT_PATH "${COMPILER_PATH}")
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search headers and libraries in the target environment only.
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONL)
#For 64bit ARM64&linux compilation
elseif(TARGET_ARCH STREQUAL "aarch64" OR TARGET_ARCH STREQUAL "arm64" OR TARGET_ARCH STREQUAL "armv8-a")
		set(CMAKE_C_COMPILER ${COMPILER_PREFIX}gcc)
		set(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}g++)
		set(CMAKE_ASM_COMPILER ${COMPILER_PREFIX}as)
		find_program(CMAKE_AR NAMES "${COMPILER_PREFIX}ar")
		find_program(CMAKE_RANLIB NAMES "${COMPILER_PREFIX}ranlib")
#For Android armv7/armv8 compilation
elseif (ANDROID_PLATFORM)
    if(NOT DEFINED ENV{ANDROID_API_LEVEL})
		set(ANDROID_API_LEVEL 21)
	else()
		set(ANDROID_API_LEVEL $ENV{ANDROID_API_LEVEL})
	endif()

	if(NOT DEFINED ENV{ARM_ANDROID_TOOLCHAIN_VERSION})
		set(ARM_ANDROID_TOOLCHAIN_VERSION 4.9)
	else()
		set(ARM_ANDROID_TOOLCHAIN_VERSION $ENV{ARM_ANDROID_TOOLCHAIN_VERSION})
	endif()

	if(TARGET_ARCH STREQUAL "armv7-a" OR TARGET_ARCH STREQUAL "arm")
		set(ANDROID_NDK_PLATFORMS_ARCH_SUFFIX "arm")
		set(ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX "arm-linux-androideabi")
	elseif(TARGET_ARCH STREQUAL "aarch64" OR TARGET_ARCH STREQUAL "armv8")
		 if(ANDROID_API_LEVEL LESS "21")
		    message(FATAL_ERROR "Aarch64 target is only availiable under ANDROID_API_LEVEL version 21 and later. Current ANDROID_API_LEVEL is ${ANDROID_API_LEVEL}.")
		 endif()
		set(ANDROID_NDK_PLATFORMS_ARCH_SUFFIX "arm64")
		set(ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX "aarch64-linux-android")
	else()
		message(FATAL_ERROR "No ANDROID_TARGET_ARCH is specified, availiable target architectures are: armv7, aarch64.")
	endif()

	#NDK_SYSROOT_PATH is used in compiler's '--sysroot' flags
	set(NDK_SYSROOT_PATH "$ENV{NDK_PATH}/platforms/android-${ANDROID_API_LEVEL}/arch-${ANDROID_NDK_PLATFORMS_ARCH_SUFFIX}/")

	set(ANDROID_TOOLCHAIN_PATH "$ENV{NDK_PATH}/toolchains/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-${ARM_ANDROID_TOOLCHAIN_VERSION}/prebuilt/linux-x86_64/bin")

	#change toolchain name according to your configuration
	set(CMAKE_C_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-gcc)
	set(CMAKE_CXX_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-g++)
	set(CMAKE_ASM_COMPILER ${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-as)

	find_program(CMAKE_AR NAMES "${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-ar")
	find_program(CMAKE_RANLIB NAMES "${ANDROID_TOOLCHAIN_PATH}/${ANDROID_NDK_TOOLCHAIN_CROSS_PREFIX}-ranlib")

	if(ARM_HARD_FLOAT)
		set(FLOAT_ABI "hard")
	else()
		set(FLOAT_ABI "softfp")
	endif()

	#TODO: Fine tune pic and pie flag for executable, share library and static library.
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${NDK_SYSROOT_PATH} -fPIC")

	# Adding cflags for armv7. Aarch64 does not need such flags.
	if(${TARGET_ARCH} STREQUAL "armv7-a" OR ${TARGET_ARCH} STREQUAL "arm")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv7-a  -mfpu=neon")
		set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} ${ASM_OPTIONS}")
	endif()
	if(ARM_HARD_FLOAT)
		# "--no-warn-mismatch" is needed for linker to suppress linker error about not all functions use VFP register to pass argument, eg.
		#   .../arm-linux-androideabi/bin/ld: error: ..../test-float.o
		#           uses VFP register arguments, output does not
		# There is call convension mismatch between NDK's crt*.o and ne10's object files.
		# crt*.o still uses softfp while ne10's object files use hard floating point.
		# Refer $NDK/tests/device/hard-float/jni/Android.mk for more details.
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--no-warn-mismatch")
	endif(ARM_HARD_FLOAT)
endif()

message(WARNING "-- Loaded toolchain:
		${CMAKE_C_COMPILER}
		${CMAKE_CXX_COMPILER}
		${CMAKE_ASM_COMPILER}")
message(WARNING "-- CMAKE_C_FLAGS:
		${CMAKE_C_FLAGS}")

if(BUILD_MODE STREQUAL "debug")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -O0 -DDEBUG -g -Wall -Wno-unused-but-set-variable")
	message("-- Building type: DEBUG")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing -O2 -DNDEBUG")
	message("-- Building type: RELEASE")
endif()
