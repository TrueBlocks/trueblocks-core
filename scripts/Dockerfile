# Go Version
FROM golang:1.22.0-bullseye

ARG repo
ARG commit_sha
ARG branch

RUN apt-get update -y
RUN apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev \
        curl bash linux-headers-amd64 xz-utils jq unzip

WORKDIR /root

RUN git clone --progress https://github.com/$repo.git /root/trueblocks-core && \
        cd /root/trueblocks-core && \
        git checkout $branch && \
        git submodule update --init --recursive && \
        mkdir -v build && \
        cd build && \
        ../scripts/go-work-sync.sh && \
        cmake ../src && \
        make -j 4

ENTRYPOINT bash /root/trueblocks-core/scripts/build-and-test.sh
