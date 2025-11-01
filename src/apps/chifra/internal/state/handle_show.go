package statePkg

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *StateOptions) HandleShow(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain

	previousBalance := make(map[base.Address]*base.Wei, len(opts.Addrs))
	var filters rpc.StateFilters
	if opts.Changes || opts.NoZero {
		filters = rpc.StateFilters{
			BalanceCheck: func(address base.Address, balance *base.Wei) bool {
				if opts.Changes {
					previous := previousBalance[address]
					if balance.Text(10) == previous.Text(10) {
						return false
					}
					previousBalance[address] = balance
				}

				if opts.NoZero {
					return len(balance.Bytes()) > 0
				}

				return true
			},
		}
	}

	stateFields, outFields, none := types.SliceToStateParts(opts.Parts)

	cnt := 0
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, addressStr := range opts.Addrs {
			address := base.HexToAddress(addressStr)
			currentBn := base.Blknum(0)
			currentTs := base.Timestamp(0)
			for _, br := range opts.BlockIds { // TODO: use the regular way to do this
				blockNums, err := br.ResolveBlocks(chain)
				if err != nil {
					errorChan <- err
					if errors.Is(err, ethereum.NotFound) {
						continue
					}
					rCtx.Cancel()
					return
				}

				for _, bn := range blockNums {
					if none {
						modelChan <- &types.State{
							Address:     address,
							BlockNumber: bn,
						}
						cnt++
						return
					}

					if br.StartType == identifiers.BlockHash {
						block, _ := opts.Conn.GetBlockHeaderByNumber(bn)
						if base.HexToHash(br.Orig) != block.Hash {
							errorChan <- errors.New("block hash " + br.Orig + " not found")
							continue
						}
					}

					state, err := opts.Conn.GetState(stateFields, address, bn, filters)
					if err != nil {
						errorChan <- err
					}
					// state may be nil if it was skipped by a filter for example
					if state != nil {
						if opts.Globals.Verbose {
							if bn == 0 || bn != currentBn {
								currentTs, _ = tslib.FromBnToTs(chain, bn)
							}
							state.Timestamp = currentTs
							currentBn = bn
						}
						cnt++
						modelChan <- state
					}
				}
			}
		}
		if cnt == 0 {
			errorChan <- errors.New("no state results were reported")
		}
	}

	extraOpts := map[string]any{
		"outFields": outFields,
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
