package namesPkg

import (
	"context"
	"io"
	"net"
	"os"

	pb "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/grpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

func (opts *NamesOptions) HandleTerms() error {
	ctx := context.Background()
	var fetchData func(modelChan chan types.Modeler[types.RawName], errorChan chan error)

	// Try RPC
	// client, err := rpc.Dial("unix", )
	dialer := func(ctx context.Context, addr string) (net.Conn, error) {
		var d net.Dialer
		return d.DialContext(ctx, "unix", addr)
	}
	options := []grpc.DialOption{
		grpc.WithTransportCredentials(insecure.NewCredentials()),
		grpc.WithBlock(),
		grpc.WithContextDialer(dialer),
	}
	conn, err := grpc.Dial("/tmp/trueblocks.sock", options...)
	if err == nil {
		defer conn.Close()
		client := pb.NewNamesClient(conn)
		// RPC server is running and available
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			stream, err := client.SearchStream(context.Background(), &pb.SearchRequest{
				Parts: utils.PointerOf(int64(opts.getType())),
				Terms: opts.Terms,
				Sort:  utils.PointerOf(int64(names.SortByAddress)),
			})
			if err != nil {
				errorChan <- err
			}
			for {
				result, err := stream.Recv()
				if err == io.EOF {
					break
				}
				if err != nil {
					errorChan <- err
				}
				modelChan <- types.NewNameFromGrpc(result)
			}
			// }
		}
	} else {
		// TODO(rpc): we don't have to print here
		logger.Info("Not using RPC server because of error", err)

		namesArray, err := names.LoadNamesArray(opts.Globals.Chain, opts.getType(), names.SortByAddress, opts.Terms)
		if err != nil {
			return err
		}
		if len(namesArray) == 0 {
			logger.Warn("No results for", os.Args)
			return nil
		}

		// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			for _, name := range namesArray {
				name := name
				modelChan <- &name
			}
		}
	}
	extra := map[string]interface{}{
		"expand":  opts.Expand,
		"prefund": opts.Prefund,
	}
	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
		Extra:      extra,
	})
}
