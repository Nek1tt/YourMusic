FROM debian:bookworm-slim

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
     ca-certificates \
     curl \
     unzip \
     build-essential \
     cmake \
     pkg-config \
     linux-libc-dev \
     zip \
     git \
     wget \
     libssl-dev \
     default-libmysqlclient-dev \
     libmariadb-dev-compat \
     autoconf \
     automake \
     autoconf-archive \
 && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg \
 && /opt/vcpkg/bootstrap-vcpkg.sh

RUN /opt/vcpkg/vcpkg install boost-system boost-thread boost-mysql boost-beast openssl 
RUN /opt/vcpkg/vcpkg install nlohmann-json

WORKDIR /app
COPY ../../common/    ./common/
COPY ../../include/streaming ./include/streaming/
COPY ../../src/streaming    ./src/streaming/

WORKDIR /app/src/streaming

RUN cmake -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
          -DCMAKE_PREFIX_PATH=/opt/vcpkg/installed/x64-linux \
          -B build -S . \
    && cmake --build build -- -j$(nproc)

RUN apt-get update \
 && apt-get purge -y cmake autoconf automake autoconf-archive \
 && apt-get autoremove -y \
 && apt-get install -y --no-install-recommends libgcc-s1 libstdc++6 libquadmath0 \
 && rm -rf /var/lib/apt/lists/*


EXPOSE 8084
ENTRYPOINT ["/app/src/streaming/build/streaming-service"]