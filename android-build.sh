# Copyright 2016 Silent Circle, LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#!/bin/bash

# cmake must be in the path
export ANDROID_NDK=/opt/android-ndk-r10e
export PATH=$ANDROID_NDK:$PATH

export AUTOMATED_BUILD=1

if [ "x$SC_BUILD_TYPE" = "xDEVELOP" ]; then
    BUILD_TYPE=Debug
    echo "*** building develop configuration"
else
   BUILD_TYPE="Release"
   echo "*** building release configuration"
fi

rm -rf buildSalAndroid
mkdir buildSalAndroid
cd buildSalAndroid

echo "###### If AUTOMATED_BUILD is set then ignore 2 cmake errors regarding OBJECT library #####"
cmake -DANDROID=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..

if make android; then
    echo "Android build OK"
else
    exit 1
fi

cd ..

libraryHome=jni/armeabi-v7a/
mkdir -p $libraryHome
ls buildSalAndroid/android/obj/local/armeabi-v7a
cp 'buildSalAndroid/android/obj/local/armeabi-v7a/libsalamander++.a'  $libraryHome
cp 'protobuf/android/obj/local/armeabi-v7a/libprotobuf-cpp-lite.a' $libraryHome
