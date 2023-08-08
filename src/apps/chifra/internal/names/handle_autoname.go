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
	chain := opts.Globals.Chain

	// For --dry_run, we don't want to write to the real database
	var overrideDatabase names.Database
	if opts.DryRun {
		overrideDatabase = names.DatabaseDryRun
	}

	name, err := opts.readContractAndClean()
	if err != nil {
		return err
	}

	err = names.WriteRegularNames(chain, overrideDatabase)
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
		_ = output.StreamMany(context.Background(), func(modelChan chan types.Modeler[types.RawModeler], errorChan chan error) {
			modelChan <- &types.SimpleMessage{
				Msg: message,
			}
		}, opts.Globals.OutputOpts())
	}
	return nil
}

// readContractAndClean will read contract data and call `cleanName` for the given address
func (opts *NamesOptions) readContractAndClean() (name *types.SimpleName, err error) {
	chain := opts.Globals.Chain
	converted, ok := opts.Conn.GetEnsAddresses([]string{opts.Autoname})
	term := opts.Autoname
	if ok {
		term = converted[0]
	}

	address := base.HexToAddress(term)
	name = &types.SimpleName{
		Address:  address,
		Name:     base.AddrToPetname(address.Hex(), "-"),
		Source:   "TrueBlocks.io",
		IsCustom: false,
	}
	if _, err = cleanName(chain, name); err != nil {
		err = fmt.Errorf("autoname %s: %w", &address, err)
		return
	}

	if !name.IsErc20 && !name.IsErc721 {
		logger.Warn("address", name.Address, "is not a token, ignoring")
		name = nil
		return
	}

	if _, err = names.LoadNamesMap(chain, names.Regular, []string{}); err != nil {
		return
	}

	if err = names.CreateRegularName(name); err != nil {
		err = fmt.Errorf("while updating %s: %w", &address, err)
		return
	}

	return
}
