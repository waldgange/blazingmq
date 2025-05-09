FROM artprod.dev.bloomberg.com/ctosec-fuzzing/bfuzz-builder


RUN apt-get update && apt-get install -y \
 bbcmake bbcmake-env bbtoolchain-cmake-debhelper binutils build-base cmake      \
 cmake-bb-instrumentation-toolchain cmake-bde-helpers cmake-compiler-helpers    \
 cmake-configure-bb-target cmake-feeds-helpers cmake-jinja2-codegen             \
 coreutils diffutils fbd-metadata feeds20-gtest-helper gawk                     \
 liba-apinisvcmsg-dev liba-csl-dev liba-fcobas-dev liba-fctkafka-dev            \
 liba-fctkafka-test-dev liba-fctssl-dev liba-fdmapp-dev liba-fdmjson-dev        \
 liba-fdmrms-dev liba-fdmrp-dev liba-fdmrsf-dev liba-felegacy-dev               \
 liba-fsvzk-dev liba-ossl-dev libapi-dev libapx-dev libare-dev libart-dev       \
 libbae-dev libbal-dev libbas-dev libbbfilexfer-dev libbccsvcmsg-dev            \
 libbce-dev libbde-dev libbdl-dev libbenchmark-dev libblprapidjson-dev          \
 libbmsdiscovery-dev libboost-headers-dev libbsc-dev libbsi-dev libbsl-dev      \
 libbte-dev libcmdbasclientmock-dev libcmdbasclienttcp-dev libcmdtst-dev        \
 libcmdtstmain-dev libdbutil-dev libe-ipc-dev libepsrtmtrhtypes-dev             \
 libexpat-dev libfbd-dev libfcc-dev libfco-dev libfct-dev libfct-test-dev       \
 libfdm-dev libflm-dev libfsv-dev libftr-dev libgmock-dev libgtest-dev          \
 libgto-dev libgut-dev libhus-dev libinteldfp-dev libitickers-dev               \
 liboid-registry-dev librapidjson-v1-dev librbl-dev librcm-dev                  \
 librstasvcmsg-dev librstasvcomsg-dev libsescoretypes-dev libsysutil-dev        \
 libtaggeddataapi-dev libtaggeddatafieldids-dev libtickerrequestout-dev perl    \
 pkg-config python-pyyaml python3.8 python3.8-dev python3.8-jinja2              \
 python3.8-pyyaml rdp-cmakemodules                                              \
 bde-internal-tools libbasrtcpusvcmsg-dev libdcssclientasync-dev                \
 libdcssclient-dev libapimagntsvcmsg-dev libapimtrmsg-dev libapu-dev            \
 librdp2bamssvcmsg-dev

COPY . $SRC/

WORKDIR $SRC
