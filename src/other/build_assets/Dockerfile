FROM golang:1.14-alpine3.12 as builder

ARG commit_sha

RUN apk add --no-cache g++ gcc make cmake git nano libcurl python3 python3-dev \
        curl bash curl-dev linux-headers sqlite-dev

RUN apk --no-cache add ca-certificates wget
RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add glibc-2.28-r0.apk

WORKDIR /root

ADD https://api.github.com/repos/TrueBlocks/trueblocks-core/git/refs/heads/develop version.json
RUN git clone -b $commit_sha --single-branch --progress --depth 1 \
        https://github.com/TrueBlocks/trueblocks-core.git \
        /root/src

RUN cd /root/src && \
        mkdir -v build && \
        cd build && \
        bash ../src/other/install/docker/clean_for_docker.sh && \
        cmake ../src && \
        make -j 4

FROM alpine:3.12
WORKDIR /root

ARG test_target

RUN apk add --no-cache libstdc++ libgcc libcurl python3 python3-dev procps bash make jq
COPY --from=builder /root/src/bin /usr/local/bin
COPY --from=builder /root/.local/share/trueblocks /root/.local/share/trueblocks
COPY build_and_test.sh /root

# To make the shell easier to use
RUN apk add curl nano

ENTRYPOINT bash /root/build_and_test.sh $test_target
