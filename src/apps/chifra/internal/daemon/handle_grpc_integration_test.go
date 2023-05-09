package daemonPkg

import (
	"context"
	"net"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/grpc/test/bufconn"
)

func startServer(t *testing.T) (*grpc.Server, *bufconn.Listener) {
	rpcServer := grpc.NewServer()
	proto.RegisterNamesServer(rpcServer, &chifraRpcServer{})

	listener := bufconn.Listen(1024 * 1024)
	go func() {
		if err := rpcServer.Serve(listener); err != nil {
			panic(err)
		}
	}()

	return rpcServer, listener
}

func createClient(t *testing.T, listener *bufconn.Listener) (*grpc.ClientConn, proto.NamesClient) { //, context.CancelFunc) {
	options := []grpc.DialOption{
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithContextDialer(func(ctx context.Context, s string) (net.Conn, error) {
			return listener.Dial()
		}),
	}

	ctx := context.Background()
	connection, err := grpc.DialContext(ctx, "bufnet", options...)
	if err != nil {
		t.Fatal(err)
	}

	client := proto.NewNamesClient(connection)
	return connection, client
}

func TestNameSubscribe(t *testing.T) {
	server, listener := startServer(t)
	defer server.Stop()

	connection, client := createClient(t, listener)
	defer connection.Close()

	response, err := client.Create(context.Background(), &proto.CreateRequest{
		Chain: "test",
		Name: &proto.Name{
			Address: "0xdeadbeef",
			Name:    "burger",
		},
	})
	if err != nil {
		t.Fatal(err)
	}
	if !response.GetSuccess() {
		t.Fatal("failed")
	}
}
