LOCAL_PATH=/Users/apple/Documents/mediaframework/mlt
# LOCAL_PATH=/Users/apple/Documents/mediaframework/mlt
LOCAL_NDK_PATH=/Users/apple/Library/Android/sdk/ndk/21.4.7075529
# LOCAL_NDK_PATH=/Users/apple/Library/Android/sdk/ndk/24.0.8215888/
LOCAL_DEP_PATH=/Users/apple/Documents/git-test-project/ffmpeg/build-android/android/arm64-v8a/lib/

cd $LOCAL_PATH/build-android/ || return
cmake .. \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_TOOLCHAIN_FILE=$LOCAL_NDK_PATH/build/cmake/android.toolchain.cmake \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=android-20 \
-DCMAKE_ANDROID_ARCH_ABI=arm64-v8a \
-DCMAKE_ANDROID_NDK=$LOCAL_NDK_PATH \
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
-DCMAKE_SYSTEM_NAME=Android \
-DCMAKE_SYSTEM_VERSION=20 \
-DANDROID=true \
-DCMAKE_INSTALL_PREFIX=packed \
-DANDROID_LIB_PATH=$LOCAL_DEP_PATH
#make -j 8

#cmake --build . --target install --config Debug