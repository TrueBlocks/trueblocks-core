package namesPkg

import (
	"context"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleCrud() (err error) {
	parts := opts.getType()
	if _, err = names.LoadNamesMap(opts.Globals.Chain, parts, nil); err != nil {
		return err
	}

	var name *types.SimpleName
	if opts.Create || opts.Update {
		name, err = handleCreate(opts.Globals.Chain, opts.crudData)
		if err != nil {
			return
		}
	}
	if opts.Delete {
		name, err = handleDelete(opts.Globals.Chain, opts.crudData, true, false)
		if err != nil {
			return
		}
	}
	if opts.Undelete {
		name, err = handleDelete(opts.Globals.Chain, opts.crudData, false, false)
		if err != nil {
			return
		}
	}
	if opts.Remove {
		name, err = handleDelete(opts.Globals.Chain, opts.crudData, true, true)
		// Remove doesn't print the removed item
		return
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
		modelChan <- name
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
		Extra: map[string]interface{}{
			"crud": true,
		},
	})
}

func handleCreate(chain string, data *CrudData) (name *types.SimpleName, err error) {
	var decimals uint64
	if data.Decimals.Updated {
		decimals, err = strconv.ParseUint(data.Decimals.Value, 10, 64)
		if err != nil {
			return
		}
	}

	name = &types.SimpleName{
		Address:  data.Address.Value,
		Name:     data.Name.Value,
		Tags:     data.Tag.Value,
		Source:   data.Source.Value,
		Symbol:   data.Symbol.Value,
		Decimals: decimals,
		Deleted:  false,
		Petname:  names.AddrToPetname(data.Address.Value.Hex(), "-"),
	}

	return name, names.CreateCustomName(chain, name)
}

func handleDelete(chain string, data *CrudData, deleted bool, remove bool) (name *types.SimpleName, err error) {
	if !remove {
		name, err = names.ChangeCustomNameDeletedFlag(chain, data.Address.Value, deleted)
		return
	}

	name, err = names.RemoveCustomName(chain, data.Address.Value)
	return
}
