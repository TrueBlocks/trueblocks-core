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
		name := &types.SimpleName{
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

func init() {
	s, _ = startServer()
}

func BenchmarkNamesOptions_HandleTerms_Grpc(b *testing.B) {
	b.Cleanup(func() {
		// Remove the socket
		os.RemoveAll(proto.SocketAddress())
	})

	// s, _ := startServer(b.N)
	defer s.Stop()

	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	err := opts.HandleTerms()
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkNamesOptions_HandleTerms_CommandLine(b *testing.B) {
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	// Emptying in-memory cache simulates calling this code multiple times
	// on the command line
	names.EmptyInMemoryCache()
	err := opts.HandleTerms()
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkNamesOptions_HandleTerms_Api(b *testing.B) {
	buf := bytes.NewBuffer([]byte{})
	w := bufio.NewWriter(buf)

	opts := defaultNamesOptions
	opts.Globals.Format = "txt"
	opts.Globals.Writer = w

	// Note: we are not calling names.EmptyInMemoryCache() here
	err := opts.HandleTerms()
	if err != nil {
		b.Fatal(err)
	}
}
