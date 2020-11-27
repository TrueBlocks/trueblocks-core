// Package main exports the example reverse-proxy implementation
// as a C/C++ shared library.
package main

import (
    "C"
    "context"
    "flag"

    "github.com/golang/glog"
    "github.com/Great-Hill-Corporation/trueblocks-core/src/go-apps/grpc-gateway"
)

type server struct {
    ch     <-chan error
    cancel func()
}

var (
    servers = make(map[int]*server)
)

//export SpawnGrpcGateway
func SpawnGrpcGateway(listenAddr, network, endpoint, swaggerDir *C.char) int {
    ctx := context.Background()
    ctx, cancel := context.WithCancel(ctx)

    opts := gateway.Options{
        Addr: C.GoString(listenAddr),
        GRPCServer: gateway.Endpoint{
            Network: C.GoString(network),
            Addr:    C.GoString(endpoint),
        },
        SwaggerDir: C.GoString(swaggerDir),
    }
    ch := make(chan error, 1)
    go func(ch chan<- error) {
        defer close(ch)
        if err := gateway.Run(ctx, opts); err != nil {
            glog.Error("grpc-gateway failed with an error: %v", err)
            ch <- err
        }
    }(ch)

    handle := len(servers) + 1
    servers[handle] = &server{
        ch:     ch,
        cancel: cancel,
    }
    return handle
}

//export WaitForGrpcGateway
func WaitForGrpcGateway(handle int) bool {
    s, ok := servers[handle]
    if !ok {
        glog.Errorf("invalid handle: %d", handle)
        return false
    }
    s.cancel()
    err := <-s.ch
    return err == nil
}

func main() {
    flag.Parse()
    defer glog.Flush()
}
