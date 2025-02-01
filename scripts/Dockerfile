# Go Version
FROM golang:1.23.1-bullseye

# Declare build arguments so they're visible inside the Dockerfile
ARG repo
ARG commit_sha
ARG branch

RUN set -eux ; apt-get update -y
RUN set -eux ; apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev
RUN set -eux ; apt install -y curl bash linux-headers-amd64 xz-utils jq unzip

# Switch into /root
WORKDIR /root

# Debug: confirm we have the correct values
RUN echo "DEBUG: repo=${repo}, branch=${branch} commit_sha=${commit_sha}"

# 1) Clone
RUN set -eux ; git clone --progress "https://github.com/${repo}.git" /root/trueblocks-core

# 2) Move into the cloned repo
WORKDIR /root/trueblocks-core

# 3) Check out the desired branch
RUN set -eux ; git checkout "${branch}"

# 4) Update/init submodules
RUN set -eux ; git submodule update --init --recursive

# 5) Make a build folder
RUN set -eux ; mkdir -v build

# 6) Switch into that folder
WORKDIR /root/trueblocks-core/build

# 7) Optionally sync go.work modules (if your script does that)
RUN set -eux ; ../scripts/go-work-sync.sh

# 8) Run CMake
RUN set -eux ; cmake ../src

# 9) Run make (compile)
RUN set -eux ; make -j4

# Finally, define your entrypoint (if you want to run tests or something)
ENTRYPOINT ["bash", "/root/trueblocks-core/scripts/build-and-test.sh"]
