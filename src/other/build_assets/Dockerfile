FROM golang:1.14-alpine3.12 as builder

ARG repo
ARG commit_sha
ARG test_target

RUN apk add --no-cache g++ gcc make cmake git nano libcurl python3 python3-dev \
        curl bash curl-dev linux-headers sqlite-dev

RUN apk --no-cache add ca-certificates wget
RUN wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub
RUN wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/2.28-r0/glibc-2.28-r0.apk
RUN apk add glibc-2.28-r0.apk

WORKDIR /root

ADD https://api.github.com/repos/$repo/git/refs/heads/$commit_sha version.json
RUN git clone -b $commit_sha --single-branch --progress --depth 1 \
        https://github.com/$repo.git \
        /root/trueblocks-core

RUN cd /root/trueblocks-core && \
        mkdir -v build && \
        cd build && \
        cmake ../src && \
        make -j 4

ENTRYPOINT bash /root/trueblocks-core/src/other/build_assets/build_and_test.sh $test_target
