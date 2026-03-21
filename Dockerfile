FROM gcc:13 AS builder
WORKDIR /app
RUN apt-get update && apt-get install -y cmake

COPY . .

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_EXE_LINKER_FLAGS="-static-libstdc++ -static-libgcc" .. && \
    make -j$(nproc)

FROM debian:bookworm-slim
WORKDIR /app

COPY --from=builder /app/build/kv_store .

RUN useradd -m kvuser
USER kvuser
EXPOSE 6379

CMD ["./kv_store"]