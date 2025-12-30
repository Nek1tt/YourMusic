FROM debian:bookworm

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    git \
    qt6-base-dev \
    qt6-declarative-dev \
    libqt6websockets6-dev \
    libgl-dev \
    libvulkan-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY frontend/ /app/frontend/

WORKDIR /app/frontend

RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 4

CMD ["./build/musicproject"]
