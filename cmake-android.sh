export ANDROID_NATIVE_API_LEVEL=android-9
export ANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8
export ANDROID_NDK=~/Development/android-ndk-r9b-darwin-x86/

cmake -DANDROID_TOOLCHAIN_NAME=${ANDROID_TOOLCHAIN_NAME} \
      -DANDROID_NDK=${ANDROID_NDK} \
      -DANDROID_NATIVE_API_LEVEL=${ANDROID_NATIVE_API_LEVEL} \
      -DCMAKE_TOOLCHAIN_FILE=../cmake/android.toolchain.cmake ..
