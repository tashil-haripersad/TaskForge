# Provides g++, make, gdb and valgrind so the project can be compiled, run,
# and investigated entirely inside the container

FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        g++ \
        make \
        gdb \
        valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the whole flat project directory (headers, sources, main.cpp,
# Makefile) into the image
COPY . .

# Build the executable at image-build time so `docker run` can go straight
# to using it; `make clean && make` still works interactively too
RUN make

# Default command just runs the program; override with e.g.
#   docker run -it taskforge bash
# to get a shell for gdb/valgrind investigation instead
CMD ["./taskforge"]