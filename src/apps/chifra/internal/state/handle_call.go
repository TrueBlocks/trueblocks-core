package statePkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/call"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *StateOptions) HandleCall() error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	isApiMode := opts.Globals.IsApiMode()

	callAddress := base.HexToAddress(opts.Addrs[0])
	if opts.ProxyFor != "" {
		callAddress = base.HexToAddress(opts.ProxyFor)
	}

	contractCall, err := call.NewContractCall(opts.Conn, callAddress, opts.Call, !isApiMode)
	if err != nil {
		return err
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawState], errorChan chan error) {
		for _, br := range opts.BlockIds {
			blockNums, err := br.ResolveBlocks(chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, bn := range blockNums {
				contractCall.BlockNumber = bn
				results, err := contractCall.Call12()
				if opts.Globals.Verbose || testMode {
					msg := fmt.Sprintf("call to %s at block %d at four-byte %s returned %v",
						contractCall.Address.Hex(), contractCall.BlockNumber, contractCall.Method.Encoding, results.Outputs)
					logger.TestLog(true, msg)
				}
				if err != nil {
					errorChan <- err
					return
				}

				modelChan <- results
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(nil))
}
