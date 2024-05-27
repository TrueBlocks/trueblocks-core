package namesPkg

import (
	"context"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleCrud() (err error) {
	chain := opts.Globals.Chain

	parts := opts.getType()
	// TODO: Why do we do this if we don't use the result?
	if _, err = names.LoadNamesMap(chain, parts, nil); err != nil {
		return err
	}

	var name *types.Name
	if opts.Create || opts.Update {
		name, err = handleCreate(chain, opts.crudData)
		if err != nil {
			return
		}
	}
	if opts.Delete {
		name, err = handleDelete(chain, opts.crudData)
		if err != nil {
			return
		}
	}
	if opts.Undelete {
		name, err = handleUndelete(chain, opts.crudData)
		if err != nil {
			return
		}
	}
	if opts.Remove {
		name, err = handleRemove(chain, opts.crudData)
		// Remove doesn't print the removed item
		return
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		modelChan <- name
	}

	extraOpts := map[string]any{
		"crud": true,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

func handleCreate(chain string, data *CrudData) (name *types.Name, err error) {
	var decimals uint64
	if data.Decimals.Updated {
		decimals, err = strconv.ParseUint(data.Decimals.Value, 10, 64)
		if err != nil {
			return
		}
	}

	name = &types.Name{
		Address:  data.Address.Value,
		Name:     data.Name.Value,
		Tags:     data.Tag.Value,
		Source:   data.Source.Value,
		Symbol:   data.Symbol.Value,
		Decimals: decimals,
		Deleted:  false,
		Petname:  base.AddrToPetname(data.Address.Value.Hex(), "-"),
	}

	return name, names.CreateName(names.DatabaseCustom, chain, name)
}

func handleDelete(chain string, data *CrudData) (*types.Name, error) {
	return names.SetDeleted(names.DatabaseCustom, chain, data.Address.Value, true)
}

func handleUndelete(chain string, data *CrudData) (*types.Name, error) {
	return names.SetDeleted(names.DatabaseCustom, chain, data.Address.Value, false)
}

func handleRemove(chain string, data *CrudData) (*types.Name, error) {
	return names.RemoveName(names.DatabaseCustom, chain, data.Address.Value)
}
