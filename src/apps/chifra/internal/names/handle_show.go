package namesPkg

import (
	"context"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/proto"
	"google.golang.org/grpc"
)

func (opts *NamesOptions) HandleShow() error {
	chain := opts.Globals.Chain
	var fetchData func(modelChan chan types.Modeler[types.Name], errorChan chan error)

	apiMode := opts.Globals.IsApiMode()
	testMode := opts.Globals.TestMode

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
		fetchData = func(modelChan chan types.Modeler[types.Name], errorChan chan error) {
			opts.fetchFromGrpc(client, modelChan, errorChan)
		}
	} else {
		// Report the error only if we know that the server is running or the user wants us
		// to be verbose
		if (grpcErr != nil && (!errors.Is(grpcErr, proto.ErrServerNotRunning) || opts.Globals.Verbose)) && !utils.IsFuzzing() {
			logger.Error("gRPC connection error:", grpcErr)
			logger.Warn("falling back to file-based search")
		}

		namesArray, err := names.LoadNamesArray(chain, opts.getType(), names.SortByAddress, opts.Terms)
		if err != nil {
			return err
		}
		if len(namesArray) == 0 {
			logger.Warn("No known names found for", opts.Terms)
			if !testMode {
				args := os.Args
				args[0] = filepath.Base(args[0])
				logger.Warn("Original command:", args)
			}
			return nil
		}

		fetchData = func(modelChan chan types.Modeler[types.Name], errorChan chan error) {
			for _, name := range namesArray {
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

func (opts *NamesOptions) fetchFromGrpc(client proto.NamesClient, modelChan chan types.Modeler[types.Name], errorChan chan error) {
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
		errorChan <- fmt.Errorf("no known names found for %v", opts.Terms)
	}
}
