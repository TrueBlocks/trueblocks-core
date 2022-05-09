FROM golang:1.18-buster as builder

ARG repo
ARG commit_sha
ARG branch
ARG test_target

ENV TEST_TARGET=$test_target

RUN apt-get update -y
RUN apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev \
        curl bash linux-headers-amd64 xz-utils jq

WORKDIR /root

RUN git clone --progress https://github.com/$repo.git /root/trueblocks-core && \
        cd /root/trueblocks-core && \
        git checkout $branch && \
        mkdir -v build && \
        cd build && \
        cmake ../src && \
        make -j 4

ENTRYPOINT bash /root/trueblocks-core/src/other/build_assets/remote_testing/build_and_test.sh $TEST_TARGET
