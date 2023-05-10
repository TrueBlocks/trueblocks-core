package proto

import (
	"context"
	"os"
	"path"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

const udsDefaultTimeout = 5 * time.Millisecond

func SocketAddress() string {
	return path.Join(os.TempDir(), "trueblocks.sock")
}

func GetContext() (context.Context, context.CancelFunc) {
	timeout := config.GetRootConfig().Grpc.UdsTimeout
	if timeout == 0 {
		timeout = udsDefaultTimeout
	}
	return context.WithTimeout(context.Background(), timeout)
}

func Connect(ctx context.Context) (connection *grpc.ClientConn, client NamesClient, err error) {
	options := []grpc.DialOption{
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
	}

	connection, err = grpc.DialContext(ctx, "unix:"+SocketAddress(), options...)
	if err != nil {
		return
	}

	client = NewNamesClient(connection)
	return
}
