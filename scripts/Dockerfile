# Go Version
FROM golang:1.25.1-bookworm

# Declare build arguments so they're visible inside the Dockerfile
ARG repo
ARG commit_sha
ARG branch

RUN set -eux ; apt-get update -y
RUN set -eux ; apt install -y g++ gcc make cmake git nano libcurl3-dev python3 python3-dev
RUN set -eux ; apt install -y curl bash linux-headers-amd64 xz-utils jq unzip

# Switch into /root
WORKDIR /root

# Debug: confirm we have the correct values and show environment
RUN echo "=== BUILD ARGUMENTS ===" && \
    echo "repo=${repo}" && \
    echo "branch=${branch}" && \
    echo "commit_sha=${commit_sha}" && \
    echo "PWD=$(pwd)" && \
    echo "=== END BUILD ARGUMENTS ==="

# 1) Clone
RUN set -eux ; git clone --progress "https://github.com/${repo}.git" /root/trueblocks-core

# 2) Move into the cloned repo
WORKDIR /root/trueblocks-core

# Debug: Show current directory and contents
RUN echo "=== POST-CLONE DEBUG ===" && \
    echo "PWD=$(pwd)" && \
    echo "Directory contents:" && \
    ls -la && \
    echo "Git status:" && \
    git status && \
    echo "=== END POST-CLONE DEBUG ==="

# 3) Check out the desired branch
RUN set -eux ; git checkout "${branch}"

# Debug: Show git status after checkout
RUN echo "=== POST-CHECKOUT DEBUG ===" && \
    echo "PWD=$(pwd)" && \
    echo "Current branch:" && \
    git branch && \
    echo "Git status:" && \
    git status && \
    echo "=== END POST-CHECKOUT DEBUG ==="

# 4) Update/init submodules
RUN set -eux ; git submodule update --init --recursive

# 5) Make a build folder
RUN set -eux ; mkdir -v build

# 6) Switch into that folder
WORKDIR /root/trueblocks-core/build

# Debug: Show build directory setup
RUN echo "=== BUILD DIRECTORY DEBUG ===" && \
    echo "PWD=$(pwd)" && \
    echo "Build directory contents:" && \
    ls -la && \
    echo "Parent directory contents:" && \
    ls -la ../ && \
    echo "scripts directory check:" && \
    ls -la ../scripts/ && \
    echo "=== END BUILD DIRECTORY DEBUG ==="

# 7) Optionally sync go.work modules (if your script does that)
RUN echo "=== RUNNING go-work-sync.sh ===" && \
    ../scripts/go-work-sync.sh 2>&1 | tee /tmp/go-work-sync.log && \
    echo "=== go-work-sync.sh completed ==="

# 8) Run CMake with better error reporting
RUN echo "=== RUNNING CMAKE ===" && \
    cmake ../src 2>&1 | tee /tmp/cmake.log || \
    (echo "CMAKE FAILED - Contents of cmake.log:" && cat /tmp/cmake.log && exit 1)

# 9) Run make with verbose output and better error reporting
RUN echo "=== RUNNING MAKE ===" && \
    make VERBOSE=1 -j1 2>&1 | tee /tmp/make.log || \
    (echo "MAKE FAILED - Last 50 lines of make.log:" && tail -50 /tmp/make.log && exit 1)

# Debug final setup before entrypoint
RUN echo "=== FINAL CONTAINER SETUP ===" && \
    echo "Build directory contents:" && \
    ls -la && \
    echo "Makefile check:" && \
    (ls -la Makefile || echo "No Makefile found") && \
    echo "test-all target check:" && \
    (make -n test-all 2>&1 | head -10 || echo "Cannot check test-all target") && \
    echo "scripts directory:" && \
    (ls -la ../scripts/ | head -10 || echo "Cannot list scripts directory") && \
    echo "build-and-test.sh permissions:" && \
    (ls -la ../scripts/build-and-test.sh || echo "Cannot find build-and-test.sh") && \
    echo "=== END FINAL SETUP ==="

# Finally, define your entrypoint (if you want to run tests or something)
ENTRYPOINT ["bash", "/root/trueblocks-core/scripts/build-and-test.sh"]
