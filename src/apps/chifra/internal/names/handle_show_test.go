package namesPkg

import (
	"bufio"
	"bytes"
	"fmt"
	"net"
	"os"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
)

type chifraRpcServer struct {
	proto.UnimplementedNamesServer
}

func (g *chifraRpcServer) SearchStream(request *proto.SearchRequest, stream proto.Names_SearchStreamServer) error {
	for i := 0; i < 13662; i++ {
		name := &types.Name{
			Name: fmt.Sprint(i),
		}
		if err := name.Send(stream); err != nil {
			return err
		}
	}

	return nil
}

func startServer() (*grpc.Server, *net.Listener) {
	rpcServer := grpc.NewServer()

	stub := &chifraRpcServer{}
	proto.RegisterNamesServer(rpcServer, stub)

	if err := os.RemoveAll(proto.SocketAddress()); err != nil {
		panic(err)
	}
	listener, err := net.Listen("unix", proto.SocketAddress())
	if err != nil {
		panic(err)
	}
	go func() {
		if err := rpcServer.Serve(listener); err != nil {
			panic(err)
		}
	}()

	return rpcServer, &listener
}

var s *grpc.Server

func setup() {
	s, _ = startServer()
}

func BenchmarkNamesOptions_HandleShow_Grpc(b *testing.B) {
	b.StopTimer()
	setup()
	defer func() {
		b.StopTimer()
		s.Stop()
		os.RemoveAll(proto.SocketAddress())
	}()

	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	b.StartTimer()
	err := opts.HandleShow()
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkNamesOptions_HandleShow_CommandLine(b *testing.B) {
	b.StopTimer()
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	b.StartTimer()
	// Emptying in-memory cache simulates calling this code multiple times
	// on the command line
	names.ClearCache()
	err := opts.HandleShow()
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkNamesOptions_HandleShow_Api(b *testing.B) {
	b.StopTimer()
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	b.StartTimer()
	// Note: we are not calling names.ClearCache() here
	err := opts.HandleShow()
	if err != nil {
		b.Fatal(err)
	}
}
