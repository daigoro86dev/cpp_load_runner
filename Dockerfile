FROM debian:bookworm-slim

# Install build and runtime dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    ca-certificates \
    curl \
    libcurl4 \
    libcurl4-openssl-dev \
    build-essential \
    cmake \
    g++ && \
    rm -rf /var/lib/apt/lists/*


WORKDIR /app

# Copy project files
COPY . .

# Build
RUN mkdir -p build && cd build && cmake .. && make -j

ENTRYPOINT ["./build/cpp_load_runner"]

# Default command (show help)
CMD ["--help"]
