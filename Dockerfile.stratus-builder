FROM debian:stretch

RUN mv /etc/apt/sources.list /etc/apt/sources.list.orig

RUN cat <<EOF > /etc/apt/sources.list
deb [trusted=yes] http://archive.debian.org/debian stretch main
deb [trusted=yes] http://archive.debian.org/debian-security stretch/updates main
deb [trusted=yes] http://archive.debian.org/debian stretch-proposed-updates main
EOF

RUN apt update \
 && apt upgrade -y \
 && apt install -y build-essential \
 && apt install -y gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf binutils-arm-linux-gnueabihf cpp-arm-linux-gnueabihf \
 && apt install -y build-essential

RUN update-alternatives --install /usr/bin/c++ c++ /usr/bin/arm-linux-gnueabihf-g++-6 30 \
 && update-alternatives --install /usr/bin/cc cc /usr/bin/arm-linux-gnueabihf-gcc-6 30 \
 && update-alternatives --install /lib/cpp cpp /usr/bin/arm-linux-gnueabihf-g++-6 30
