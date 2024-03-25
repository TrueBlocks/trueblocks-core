package statePkg

import (
	"context"
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *StateOptions) HandleShow() error {
	chain := opts.Globals.Chain

	previousBalance := make(map[base.Address]*base.MyWei, len(opts.Addrs))
	var filters rpc.StateFilters
	if opts.Changes || opts.NoZero {
		filters = rpc.StateFilters{
			Balance: func(address base.Address, balance *base.MyWei) bool {
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

	stateFields, outputFields, none := opts.Conn.GetFieldsFromParts(opts.Parts, opts.Globals.Ether)

	cnt := 0
	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawState], errorChan chan error) {
		for _, addressStr := range opts.Addrs {
			address := base.HexToAddress(addressStr)
			currentBn := uint64(0)
			currentTs := base.Timestamp(0)
			for _, br := range opts.BlockIds { // TODO: use the regular way to do this
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
					if none {
						modelChan <- &types.SimpleState{
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

	extra := map[string]interface{}{
		"fields": outputFields,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
