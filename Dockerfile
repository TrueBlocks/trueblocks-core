# Build trueblocks-core in stock Go builder container
FROM golang:1.17-alpine as builder

# Install additional build depenedencies
RUN apk --no-cache add g++ gcc make cmake git nano libcurl python3 python3-dev \
        curl bash curl-dev linux-headers sqlite-dev

# Add source to container
ADD . /trueblocks-core

# Build trueblocks-core
# make -j <cpu_cores> (4+1 is a safe enough number)
RUN cd /trueblocks-core && mkdir build && cd build && cmake ../src && make -j 5

# Switch to an alpine container
FROM alpine:latest

# Install binary dependencies and nice to haves
RUN apk --no-cache add ca-certificates gzip libstdc++ libgcc libcurl python3 python3-dev procps bash curl nano

# Copy files from builder
COPY --from=builder /trueblocks-core/bin /usr/local/bin
COPY --from=builder /root/.local/bin/chifra /root/.local/bin/chifra
COPY --from=builder /root/.local/share/trueblocks /root/.local/share/trueblocks

# If no command is provided return help
CMD ["chifra", "--help"]
