package proto

import (
	"context"
	"errors"
	"os"
	"path"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

const udsDefaultTimeout = 5 * time.Millisecond

var ErrServerNotRunning = errors.New("gRPC server not running")

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
	if !file.FileExists(SocketAddress()) {
		err = ErrServerNotRunning
		return
	}

	options := []grpc.DialOption{
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
	}

	connection, err = grpc.DialContext(ctx, "unix:"+SocketAddress(), options...)
	if err != nil {
		if errors.Is(err, context.DeadlineExceeded) {
			// If we got timeout, Unix Domain Socket is broken, so let's try to remove it
			// and report back that the server is not running
			err = os.Remove(SocketAddress())
			if err != nil {
				return
			}
			err = ErrServerNotRunning
		}
		return
	}

	client = NewNamesClient(connection)
	return
}
