package namesPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient/ens"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleAutoname() error {
	// For --dry_run, we don't want to write to the real database
	var overrideDatabase names.Database
	if opts.DryRun {
		overrideDatabase = names.DatabaseDryRun
	}

	if err := opts.readContractAndClean(); err != nil {
		return err
	}

	return names.WriteRegularNames(opts.Globals.Chain, overrideDatabase)
}

// readContractAndClean will read contract data and call `cleanName` for the given address
func (opts *NamesOptions) readContractAndClean() error {
	converted, ok := ens.ConvertEns(opts.Globals.Chain, []string{opts.Autoname})
	term := opts.Autoname
	if ok {
		term = converted[0]
	}

	address := base.HexToAddress(term)
	name := &types.SimpleName{
		Address:  address,
		Name:     names.AddrToPetname(address.Hex(), "-"),
		Source:   "TrueBlocks.io",
		IsCustom: false,
	}
	_, err := cleanName(opts.Globals.Chain, name)
	if err != nil {
		return fmt.Errorf("autoname %s: %w", &address, err)
	}

	if !name.IsErc20 && !name.IsErc721 {
		logger.Warn("address", name.Address, "is not a token, ignoring")
		return nil
	}

	if _, err = names.LoadNamesMap(opts.Globals.Chain, names.Regular, []string{}); err != nil {
		return err
	}

	if err = names.CreateRegularName(name); err != nil {
		return fmt.Errorf("updating %s: %w", &address, err)
	}

	return nil
}
