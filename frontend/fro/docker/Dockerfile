FROM debian:bullseye

RUN apt-get update && apt-get install -y \
    cmake \
    qtbase5-dev \
    qtdeclarative5-dev \
    g++ \
    make \
    git \
    x11-apps \
    libgl1-mesa-dev \
    && apt-get clean


WORKDIR /app

COPY ../ /app

RUN mkdir -p build && cd build && cmake .. && cmake --build .

CMD ["./build/musicproject"]
