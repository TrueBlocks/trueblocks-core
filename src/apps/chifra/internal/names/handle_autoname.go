package namesPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleAutoname() error {
	name, err := opts.readContractAndClean()
	if err != nil {
		return err
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
	logger.Info(message)

	if opts.Globals.IsApiMode() {
		_ = output.StreamMany(context.Background(), func(modelChan chan types.Modeler[types.RawMessage], errorChan chan error) {
			modelChan <- &types.Message{
				Msg: message,
			}
		}, opts.Globals.OutputOpts())
	}
	return nil
}

// readContractAndClean will read contract data and call `cleanName` for the given address
func (opts *NamesOptions) readContractAndClean() (name *types.Name, err error) {
	chain := opts.Globals.Chain

	name = &types.Name{
		Address:  opts.AutonameAddr,
		Name:     base.AddrToPetname(opts.AutonameAddr.Hex(), "-"),
		Source:   "TrueBlocks.io",
		IsCustom: true,
	}
	if _, err = cleanName(chain, name); err != nil {
		err = fmt.Errorf("autoname %s: %w", opts.Autoname, err)
		return
	}

	if !name.IsErc20 && !name.IsErc721 {
		logger.Warn("address", name.Address, "is not a token, ignoring...")
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
