# ProtoBuffers

https://developers.google.com/protocol-buffers/docs/proto3

- Benefits:
  - Binary transfer is much faster than JSON
  - Supports Syncronous and Async queries
  - Supports unary and bi-directional streaming (lower memery pressure)
  - Supports automatically generating code
    - Export to 10 languages including Go, C++, Python
    - Export to OpenAPI 3.0 specs for help file generation (Swagger)
- Downside:
  - Not backwards compatible to current Ethereum JSON-RPC (solution: use a gateway)
  - Installation is a pain (see below)
  - Open Question: how to handle big numbers (as a string??)

# Buf

https://docs.buf.build/

A nice looking tool to build, lint, test and manage protobuffer builds including the ability to add linting of protobufs to CI

# Possible Architecture

https://docs.google.com/presentation/d/16KtLZOYZOLWxYq_vzVAAsfkjPFEElEFZjxDYeLy1vjQ/edit?usp=sharing

# Installing and enabling grpc-gateway

https://github.com/grpc-ecosystem/grpc-gateway

- create a new folder (`mkdir ./gateway && cd gateway`)
- create a file called tools.go with the following contents:

  ```
  package tools
  import (
      _ "github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-grpc-gateway"
      _ "github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-openapiv2"
      _ "google.golang.org/grpc/cmd/protoc-gen-go-grpc"
      _ "google.golang.org/protobuf/cmd/protoc-gen-go"
  )
  ```

- run `go mod init trueblocks.io/tools`
- run `go mod tidy`
- run `go install github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-grpc-gateway github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-openapiv2 google.golang.org/protobuf/cmd/protoc-gen-go google.golang.org/grpc/cmd/protoc-gen-go-grpc`

This installs the grpc-gateway extention for protoc. Next, edit your .proto file to include instructions to protoc to generate the JSON RPC transcoder (see instructions at the link).

We choose the second option under item 4.

```
additional .proto modifications to use a custom mapping
relies on parameters in the .proto file to set custom HTTP mappings
```

# Building

- Require user to have grpc/protobufs already installed
- Build as part of cmake (intrusive)
- Bazel?

# CMake

- It's easy to add gRPC to a CMake build
- The 'first build' time is takes a while, but not thereafter
- Can we use a submodule? (I don't know what this means.)

# Bazel

https://docs.bazel.build/versions/master/install.html

- A lot of the gRPC stuff uses the Bazel build system
- Easy to use and install, but Mac users have to download using curl

# vcpkg

```
// install vcpkg package manager on your system
// using the official instructions
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
// install gRPC using vcpkg package manager
vcpkg install grpc
```

# grpc-httpjson-transcoding

https://github.com/grpc-ecosystem/grpc-httpjson-transcoding

- License: Apache
- Language: c++
- Build System: bazel

# Unordered NOTES

Agenda:

    Summary
    What is the licensing issue?
    Do we want MIT code to infect our code?
    Do we want to build a
      JSON RPC
      TG-Silkwork Hybrid
      Silkwork

Links:

https://medium.com/@amsokol.com/tutorial-how-to-develop-go-grpc-microservice-with-http-rest-endpoint-middleware-kubernetes-daebb36a97e9

https://medium.com/@thatcher/why-choose-between-grpc-and-rest-bc0d351f2f84

https://www.reddit.com/r/golang/comments/7lhfau/grpc_pros_and_cons/
