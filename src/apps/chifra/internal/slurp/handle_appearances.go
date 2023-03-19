package slurpPkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/bykof/gostradamus"
)

func (opts *SlurpOptions) HandleShowAppearances() error {
	chain := opts.Globals.Chain

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		cnt := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				txs, err := opts.GetTransactionsFromEtherscan(chain, addr, tt)
				if err != nil {
					errorChan <- err
					continue
				}

				for _, tx := range txs {
					tx := tx
					if !opts.isInRange(uint(tx.BlockNumber), errorChan) {
						continue
					}
					modelChan <- &types.SimpleAppearance{
						Address:          types.HexToAddress(addr),
						BlockNumber:      uint32(tx.BlockNumber),
						TransactionIndex: uint32(tx.TransactionIndex),
						Timestamp:        tx.Timestamp,
						Date:             gostradamus.FromUnixTimestamp(tx.Timestamp).String(),
					}
					cnt++
				}
				if opts.Globals.TestMode {
					// Without this Etherscan chokes
					time.Sleep(250 * time.Millisecond)
				}
			}
		}

		if cnt == 0 {
			errorChan <- fmt.Errorf("no transactions found")
		}
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
	})
}
