cd /Users/apple/Documents/mediaframework/mlt/build/ || return
cmake .. \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE=/Users/apple/Library/Android/sdk/ndk/24.0.8215888/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=android-20 \
-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
-DCMAKE_ANDROID_NDK=/Users/apple/Library/Android/sdk/ndk/24.0.8215888 \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_SYSTEM_NAME=Android \
-DCMAKE_SYSTEM_VERSION=20 \
-DANDROID=true \
-DCMAKE_INSTALL_PREFIX=packed
#make -j 8

cmake --build . --target install --config Debug