##
## This builds Faust for the ARM 7 architecture, using Debian buster.
## The version of faust that is built is VERY constrained - it consists
## of the Faust binary, compiled to support conly CPP output, and the
## architecture file for the Stratus
##
## THAT however, is not the end of the story - the Stratus runs 
## Debian 9. Debian 9 does not come with a CMAKE of a recent-enough
## version to allow direct compilation of Faust - hence buster.
##
## HOWEVER buster comes with glibc 8 with 7 compatibility - not present
## in Debian 9 - so the faust executable built on buster won't run on
## the stratus.
##
## THIS then is where it gets a bit complicated - we have to 
##
##   * Deliver the glibc-2.7 libraries and put them in a special place
##   * Patch the faust binary to load its dependencies from there
##
## At the successful end of docker build, the file /root/faust-stratus.tgz
## will have been created. This can then be copied to the stratus system
## root folder ("/") and untarred there in order to give access to the full
## Faust DSP-to-Stratus SO chain
##
FROM debian:buster

RUN apt update \
 && apt install -y build-essential cmake pkg-config zip curl docker python3-pip

RUN apt install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabihf cpp-arm-linux-gnueabihf  \
 && update-alternatives --install /usr/bin/c++ c++ /usr/bin/arm-linux-gnueabihf-g++-8 30 \
 && update-alternatives --install /usr/bin/cc cc /usr/bin/arm-linux-gnueabihf-gcc-8 30 \
 && update-alternatives --install /lib/cpp cpp /usr/bin/arm-linux-gnueabihf-g++-8 30

ENV PATCHELF_VERSION=0.18.0
ENV GLIBC_2_7_DIR=/lib-glib-27
 
ENV FAUST_VERSION=2.72.14
ENV FAUST_PREFIX=/usr/local
ENV FAUST_BIN_DIR=${FAUST_PREFIX}/bin
ENV FAUST_ARCH_DIR=${FAUST_PREFIX}/share/faust
ENV FAUST_LIB_DIR=${FAUST_PREFIX}/lib

WORKDIR /tmp/
ENV WORKDIR=/tmp

ADD ci ci
ADD srcs srcs
ADD resources resources
ADD tests tests
 
RUN ./ci/scripts/install-faust.sh
RUN ./ci/scripts/install-stratus-faust.sh
RUN ./ci/scripts/patch-faust.sh
RUN ./ci/scripts/install-stratus-python.sh
RUN ./ci/scripts/stage-sdk.sh
