package namesPkg

import (
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
)

func (opts *NamesOptions) HandleCrud(rCtx *output.RenderCtx) (err error) {
	chain := opts.Globals.Chain

	parts := opts.getType()
	// TODO: Why do we do this if we don't use the result?
	if _, err = names.LoadNamesMap(chain, parts, nil); err != nil {
		return err
	}

	var name *types.Name
	if opts.Create {
		name, err = handleCreate(chain, opts.crudData)
		if err != nil {
			return
		}
	}
	if opts.Update {
		name, err = handleUpdate(chain, opts.crudData)
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

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		modelChan <- name
	}

	extraOpts := map[string]any{
		"crud": true,
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}

func handleCreate(chain string, data *crud.NameCrud) (name *types.Name, err error) {
	var decimals uint64
	if data.Decimals.Updated {
		decimals, err = strconv.ParseUint(data.Decimals.Value, 10, 64)
		if err != nil {
			return
		}
	}

	name = &types.Name{
		Address:  base.HexToAddress(data.Address.Value),
		Name:     data.Name.Value,
		Tags:     data.Tags.Value,
		Source:   data.Source.Value,
		Symbol:   data.Symbol.Value,
		Decimals: decimals,
		Deleted:  false,
	}

	return name, names.CreateName(names.DatabaseCustom, chain, name)
}

func handleUpdate(chain string, data *crud.NameCrud) (name *types.Name, err error) {
	var decimals uint64
	if data.Decimals.Updated {
		decimals, err = strconv.ParseUint(data.Decimals.Value, 10, 64)
		if err != nil {
			return
		}
	}

	name = &types.Name{
		Address:  base.HexToAddress(data.Address.Value),
		Name:     data.Name.Value,
		Tags:     data.Tags.Value,
		Source:   data.Source.Value,
		Symbol:   data.Symbol.Value,
		Decimals: decimals,
		Deleted:  false,
	}

	return name, names.UpdateName(names.DatabaseCustom, chain, name)
}

func handleDelete(chain string, data *crud.NameCrud) (*types.Name, error) {
	return names.SetDeleted(names.DatabaseCustom, chain, base.HexToAddress(data.Address.Value), true)
}

func handleUndelete(chain string, data *crud.NameCrud) (*types.Name, error) {
	return names.SetDeleted(names.DatabaseCustom, chain, base.HexToAddress(data.Address.Value), false)
}

func handleRemove(chain string, data *crud.NameCrud) (*types.Name, error) {
	return names.RemoveName(names.DatabaseCustom, chain, base.HexToAddress(data.Address.Value))
}
