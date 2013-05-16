# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_STL := gnustl_static
#APP_CPPFLAGS = -fexceptions -std=gnu++0x
#STLPORT_FORCE_REBUILD := true
#APP_OPTIM=release
APP_OPTIM=debug
APP_ABI := armeabi #x86 #armeabi-v7a

APP_PLATFORM := android-14
