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
)

func (opts *NamesOptions) HandleTerms() error {
	var fetchData func(modelChan chan types.Modeler[types.RawName], errorChan chan error)

	// Try RPC
	grpcCtx, grpcCancel := proto.GetContext()
	defer grpcCancel()
	conn, client, grpcErr := proto.Connect(grpcCtx)
	if grpcErr == nil {
		defer conn.Close()
		// RPC server is running and available
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			opts.fetchFromGrpc(client, modelChan, errorChan)
		}
	} else {
		if !errors.Is(grpcErr, context.DeadlineExceeded) {
			logger.Error("gRPC connection error:", grpcErr)
		}

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
	if opts.Addr {
		extra["single"] = "address"
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
}
