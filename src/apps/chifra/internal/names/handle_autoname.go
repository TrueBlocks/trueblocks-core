package namesPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *NamesOptions) HandleAutoname() error {
	ctx := context.Background()
	name, err := opts.readContractAndClean("")
	if err != nil {
		return err
	} else if name == nil {
		latest := opts.Conn.GetLatestBlockNumber()
		proxy, err := opts.Conn.GetContractProxyAt(opts.AutonameAddr, latest)
		if err != nil {
			return err
		}
		if proxy != base.ZeroAddr {
			opts.AutonameAddr = proxy
			name, err = opts.readContractAndClean("proxy")
			if err != nil {
				return err
			}
		}
	}

	message := "No name has been updated. Is the contract a token?"
	if name != nil {
		message = fmt.Sprintf(
			"Updated name for %s, %s. ERC-20 token: %t, ERC-721 NFT: %t, Symbol: %s, Decimals: %d",
			name.Address,
			name.Name,
			name.IsErc20,
			name.IsErc721,
			name.Symbol,
			name.Decimals,
		)
	}

	if !utils.IsFuzzing() {
		logger.Info(message)
	}

	if opts.Globals.IsApiMode() {
		fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
			modelChan <- &types.Message{
				Msg: message,
			}
		}
		_ = output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
	}
	return nil
}

// readContractAndClean will read contract data and call `cleanName` for the given address
func (opts *NamesOptions) readContractAndClean(s string) (name *types.Name, err error) {
	chain := opts.Globals.Chain

	name = &types.Name{
		Address:  opts.AutonameAddr,
		Source:   "TrueBlocks.io",
		IsCustom: true,
	}
	if _, err = cleanName(chain, name); err != nil {
		err = fmt.Errorf("autoname %s: %w", opts.Autoname, err)
		return
	}

	if !name.IsErc20 && !name.IsErc721 {
		a := "address"
		if s != "" {
			a = s
		}
		logger.Warn(a, name.Address, "is not a token, ignoring...")
		name = nil
		return
	}

	if _, err = names.LoadNamesMap(chain, names.Custom, []string{}); err != nil {
		return
	}

	if err = names.CreateName(names.DatabaseCustom, chain, name); err != nil {
		err = fmt.Errorf("while updating %s: %w", opts.Autoname, err)
		return
	}

	return
}
