# Go Version
FROM golang:1.22.0-bullseye as builder

ARG repo
ARG commit_sha
ARG branch

RUN apt-get update -y
RUN apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev \
        curl bash linux-headers-amd64 xz-utils jq unzip

RUN go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28
RUN go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@v1.2

WORKDIR /root

RUN curl -LO https://github.com/protocolbuffers/protobuf/releases/download/v22.3/protoc-22.3-linux-x86_64.zip
RUN unzip protoc-22.3-linux-x86_64.zip -d $HOME/.local
RUN cp $HOME/.local/bin/protoc /usr/local/bin
#ENV PATH="$PATH:$HOME/.local/bin"

RUN git clone --progress https://github.com/$repo.git /root/trueblocks-core && \
        cd /root/trueblocks-core && \
        git checkout $branch && \
        mkdir -v build && \
        cd build && \
        cmake ../src && \
        make -j 4

ENTRYPOINT bash /root/trueblocks-core/src/other/build_assets/remote_testing/build_and_test.sh
