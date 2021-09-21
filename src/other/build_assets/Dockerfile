FROM golang:1.14-buster as builder

ARG repo
ARG commit_sha
ARG test_target
ARG config_file

RUN apt-get update -y
RUN apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev \
        curl bash linux-headers-amd64 libsqlite3-dev xz-utils

# RUN apk add glibc-2.28-r0.apk

WORKDIR /root

RUN git clone --progress \
        https://github.com/$repo.git \
        /root/trueblocks-core && \
        cd /root/trueblocks-core && \
        git reset --hard $commit_sha && \
        mkdir -v build && \
        cd build && \
        cmake ../src && \
        make -j 4

ENTRYPOINT bash /root/trueblocks-core/src/other/build_assets/build_and_test.sh $test_target
