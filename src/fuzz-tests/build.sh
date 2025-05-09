#!/bin/bash -eu

export CXXFLAGS="${CXXFLAGS} -fsanitize=fuzzer-no-link"

bbcmake-env \
        install \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_PREFIX_PATH=/opt/bb/share/cmake \
        -DCMAKE_TOOLCHAIN_FILE="${CMAKE_TOOLCHAIN_FILE}" \
        -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
        -DCMAKE_VERBOSE_MAKEFILE=ON \
        --refroot=/ \
        --action=clean,build \
        -S "blazingmq-mirror"

INCLUDES="-I/src/blazingmq-mirror/fuzzers \
  -I/src/blazingmq-mirror/mocks \
  -I/src/blazingmq-mirror/src/applications/bmqtool \
  -I/src/blazingmq-mirror/src/groups/mqb/mqba \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbblp \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbc \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbcfg \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbcmd \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbscm \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbconfm \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbi \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbu \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbnet \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbstat \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbplug \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbs \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbsi \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbsl \
  -I/src/blazingmq-mirror/src/groups/mqb/mqbmock \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqa \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqimp \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqp \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqpi \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqscm \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqt \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqeval \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqc \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqscm \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqu \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqex \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqsys \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqst \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqstm \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqio \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqt \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqma \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqtsk \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqtst \
  -I/src/blazingmq-mirror/src/groups/bmq/bmqvt \
  -isystem /opt/bb/include \
  -I/opt/bb/share/cmake/BBClang-Toolchain/ \
  -I/opt/bb/include/clang-gcc-intrinsics-workaround \
  -I/src/blazingmq-mirror/cmake-build/linux_64_static_make_Debug/src/groups/bmq"


LIBS="  $SRC//blazingmq-mirror/cmake-build/linux_64_static_make_Debug/src/groups/mqb/libmqb.a \
  -ldl \
  $SRC/blazingmq-mirror/cmake-build/linux_64_static_make_Debug/src/groups/bmq/libbmq.a \
  //opt/bb/lib64/libz.a \
  //opt/bb/lib64/libfl.a \
  $SRC/blazingmq-mirror/cmake-build/linux_64_static_make_Debug/src/groups/bmq/libbmq.a \
  //opt/bb/lib64/libntc.a \
  //opt/bb/lib64/libnts.a \
  //opt/bb/lib64/libbal.a \
  //opt/bb/lib64/libbdl.a \
  //opt/bb/lib64/libinteldfp.a \
  //opt/bb/lib64/libpcre2.a \
  //opt/bb/lib64/libbsl.a \
  -lpthread \
  -lrt \
  //opt/bb/lib64/libryu.a"


# Fuzzer source filenames should end in "_fuzzer"
for fuzzer in bmqt_uri_fuzzer eval_fuzzer parseutil_fuzzer; do
  $CXX $CXXFLAGS $LIB_FUZZING_ENGINE \
  $SRC/fuzzers/${fuzzer}.cpp -std=gnu++20 \
  -o $OUT/${fuzzer} \
  $INCLUDES \
  $LIBS
done


# Zip the fuzzer binaries and place them in $OUT
find ${OUT} -type f -name "*_fuzzer" -exec zip {}.zip {} \;
