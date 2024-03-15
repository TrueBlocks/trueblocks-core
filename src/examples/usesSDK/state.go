package main

import (
	"fmt"
	"math/big"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// DoState tests the When sdk function
func DoState() {
	logger.Info("DoState")

	whenOpts := sdk.WhenOptions{
		BlockIds: []string{"2016-01-01T00:00:00", "2024-02-29T23:59:59"},
	}

	var blks []string
	blocks, _, err := whenOpts.Query()
	if err != nil {
		logger.Fatal(err)
	} else {
		blks = make([]string, 0, len(blocks))
		for _, blk := range blocks {
			blks = append(blks, fmt.Sprintf("%d", blk.BlockNumber))
			fmt.Println("blocks", blk.BlockNumber, blk.Date())
		}
	}
	blk2 := blks[0] + "-" + blks[1] + ":weekly"

	namesOpts := sdk.NamesOptions{
		Terms:  []string{"trueblocks.eth"},
		Custom: true,
	}

	var m sync.Mutex
	if names, _, _ := namesOpts.Query(); len(names) == 0 {
		logger.Fatal(fmt.Errorf("no names found"))
	} else {
		ch := make(chan sdk.StateSdk)
		var wg sync.WaitGroup
		wg.Add(len(names))
		for _, name := range names {
			go func(n types.SimpleName) {
				defer wg.Done()
				stateOpts := sdk.StateOptions{
					Addrs:    []string{name.Address.Hex()},
					BlockIds: []string{blk2},
					Parts:    sdk.SPBalance, // | sdk.SPNonce | sdk.SPCode,
					Globals: sdk.Globals{
						Ether: true,
						Cache: true,
					},
				}
				m.Lock()
				if state, _, err := stateOpts.Query(); err != nil {
					logger.Fatal(err)
				} else if len(state) == 0 {
					logger.Fatal(fmt.Errorf("weird outcome"))
				} else {
					m.Unlock()
					zero := new(big.Int)
					for _, st := range state {
						if st.Balance.Int.Cmp(zero) != 0 {
							st.Address.Address = name.Address
							ch <- st
						}
					}
				}
			}(name)
		}

		go func() {
			wg.Wait()
			close(ch)
		}()

		for state := range ch {
			msg := fmt.Sprintf("%d\t%s\t%s\t%s", state.BlockNumber, state.Date(), state.Address.Hex(), utils.FormattedValue(state.Balance.Int, true, 18))
			fmt.Println(msg)
		}
	}
}
