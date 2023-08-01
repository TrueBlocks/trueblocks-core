package namesPkg

import (
	"context"
	"errors"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
)

func (opts *NamesOptions) HandleTerms() error {
	chain := opts.Globals.Chain
	var fetchData func(modelChan chan types.Modeler[types.RawName], errorChan chan error)

	apiMode := opts.Globals.IsApiMode()

	var conn *grpc.ClientConn
	var client proto.NamesClient
	var grpcErr error

	if !apiMode {
		// Try RPC
		grpcCtx, grpcCancel := proto.GetContext()
		defer grpcCancel()
		conn, client, grpcErr = proto.Connect(grpcCtx)
	}
	if !apiMode && grpcErr == nil {
		defer conn.Close()
		// RPC server is running and available
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			opts.fetchFromGrpc(client, modelChan, errorChan)
		}
	} else {
		// Report the error only if we know that the server is running or the user wants us
		// to be verbose
		if grpcErr != nil && (!errors.Is(grpcErr, proto.ErrServerNotRunning) || opts.Globals.Verbose) {
			logger.Error("gRPC connection error:", grpcErr)
			logger.Warn("falling back to file-based search")
		}

		namesArray, err := names.LoadNamesArray(chain, opts.getType(), names.SortByAddress, opts.Terms)
		if err != nil {
			return err
		}
		if len(namesArray) == 0 {
			logNoResults()
			return nil
		}

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
	if opts.Addr {
		extra["single"] = "address"
		opts.Globals.NoHeader = true
	}
	ctx := context.Background()
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *NamesOptions) fetchFromGrpc(client proto.NamesClient, modelChan chan types.Modeler[types.RawName], errorChan chan error) {
	stream, err := client.SearchStream(context.Background(), &proto.SearchRequest{
		Parts: int64(opts.getType()),
		Terms: opts.Terms,
		Sort:  int64(names.SortByAddress),
	})
	if err != nil {
		errorChan <- err
	}
	hasResults := false
	for {
		result, err := stream.Recv()
		if err == io.EOF {
			break
		}
		if err != nil {
			errorChan <- err
		}
		hasResults = true
		modelChan <- types.NewNameFromGrpc(result)
	}
	if !hasResults {
		logNoResults()
	}
}

func logNoResults() {
	logger.Warn("No results for", os.Args)
}
