# Go Version
FROM golang:1.22.0-bullseye

ARG repo
ARG commit_sha
ARG branch

RUN apt-get update -y
RUN apt-get install -y curl bash g++ gcc make cmake git
RUN nano libcurl3-dev python3 python3-dev jq unzip
RUN apt-get install -y linux-headers-amd64 xz-utils

curl -fsSL https://deb.nodesource.com/setup_20.x | bash -
apt-get install -y nodejs

RUN go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28
RUN go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@v1.2

WORKDIR /root

RUN curl -LO https://github.com/protocolbuffers/protobuf/releases/download/v22.3/protoc-22.3-linux-x86_64.zip
RUN unzip protoc-22.3-linux-x86_64.zip -d $HOME/.local
RUN cp $HOME/.local/bin/protoc /usr/local/bin

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
