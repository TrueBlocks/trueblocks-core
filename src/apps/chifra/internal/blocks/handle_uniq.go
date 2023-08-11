package blocksPkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/uniq"
	"github.com/ethereum/go-ethereum"
)

func (opts *BlocksOptions) HandleUniq() (err error) {
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		procFunc := func(s *types.SimpleAppearance) error {
			modelChan <- s
			return nil
		}

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

			showProgress := !opts.Globals.TestMode && len(opts.Globals.File) == 0
			bar := logger.NewBar("", showProgress, int64(len(blockNums)))
			for _, bn := range blockNums {
				bar.Tick()
				if err := uniq.GetUniqAddressesInBlock(chain, opts.Flow, opts.Conn, procFunc, bn); err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					cancel()
					return
				}
			}
			bar.Finish(true /* newLine */)
		}
	}

	extra := map[string]interface{}{
		"uniq": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
