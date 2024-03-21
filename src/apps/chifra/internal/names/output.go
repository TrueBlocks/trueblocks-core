// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

// EXISTING_CODE
import (
	"errors"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunNames handles the names command for the command line. Returns error only as per cobra.
func RunNames(cmd *cobra.Command, args []string) error {
	opts := namesFinishParse(args)
	outputHelpers.EnableCommand("names", true)
	// EXISTING_CODE
	var err1 error
	if err1 = opts.loadCrudDataIfNeeded(nil); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("names", &opts.Globals)
	return opts.NamesInternal()
}

// ServeNames handles the names command for the API. Returns an error.
func ServeNames(w http.ResponseWriter, r *http.Request) error {
	opts := namesFinishParseApi(w, r)
	outputHelpers.EnableCommand("names", true)
	// EXISTING_CODE
	var err1 error
	if err1 = opts.loadCrudDataIfNeeded(r); err1 != nil {
		return err1
	}
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("names", w, &opts.Globals)
	err := opts.NamesInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("names", err, &opts.Globals)
	return err
}

// NamesInternal handles the internal workings of the names command.  Returns an error.
func (opts *NamesOptions) NamesInternal() error {
	var err error
	if err = opts.validateNames(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra names"
	// EXISTING_CODE
	if opts.anyCrud() {
		err = opts.HandleCrud()
	} else if len(opts.Autoname) > 0 {
		err = opts.HandleAutoname()
	} else if opts.Clean {
		err = opts.HandleClean()
	} else if opts.Tags {
		err = opts.HandleTags()
	} else {
		err = opts.HandleShow()
	}

	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetNamesOptions returns the options for this tool so other tools may use it.
func GetNamesOptions(args []string, g *globals.GlobalOptions) *NamesOptions {
	ret := namesFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
func (opts *NamesOptions) getType() names.Parts {
	var ret names.Parts

	if opts.Custom || opts.All || opts.anyCrud() {
		ret |= names.Custom
	}

	if opts.Prefund || opts.All {
		ret |= names.Prefund
	}

	if (!opts.Custom && !opts.Prefund) || opts.All {
		ret |= names.Regular
	}

	if opts.MatchCase {
		ret |= names.MatchCase
	}

	if opts.Expand {
		ret |= names.Expanded
	}

	if opts.Tags {
		ret |= names.Tags
	}

	if opts.Globals.TestMode {
		ret |= names.Testing
	}

	return ret
}

func (opts *NamesOptions) anyCrud() bool {
	return opts.Create ||
		opts.Update ||
		opts.Delete ||
		opts.Undelete ||
		opts.Remove
}

func (opts *NamesOptions) createOrUpdate() bool {
	return opts.Create || opts.Update
}

type CrudData struct {
	Address     crudDataField[base.Address]
	Name        crudDataField[string]
	Tag         crudDataField[string]
	Source      crudDataField[string]
	Symbol      crudDataField[string]
	Decimals    crudDataField[string]
	Description crudDataField[string]
}
type crudValue interface {
	base.Address | string
}
type crudDataField[CV crudValue] struct {
	Value   CV
	Updated bool
}

func (opts *NamesOptions) getCrudDataHttp(r *http.Request) (data *CrudData, err error) {
	if err = r.ParseForm(); err != nil {
		return
	}
	if len(r.PostForm) == 0 {
		err = errors.New("empty form")
		return
	}

	data = &CrudData{
		Address: crudDataField[base.Address]{
			Value:   base.HexToAddress(r.PostForm.Get("address")),
			Updated: r.PostForm.Has("address"),
		},
		Name: crudDataField[string]{
			Value:   r.PostForm.Get("name"),
			Updated: r.PostForm.Has("name"),
		},
		Tag: crudDataField[string]{
			Value:   r.PostForm.Get("tag"),
			Updated: r.PostForm.Has("tag"),
		},
		Source: crudDataField[string]{
			Value:   r.PostForm.Get("source"),
			Updated: r.PostForm.Has("source"),
		},
		Symbol: crudDataField[string]{
			Value:   r.PostForm.Get("symbol"),
			Updated: r.PostForm.Has("symbol"),
		},
		Decimals: crudDataField[string]{
			Value:   r.PostForm.Get("decimals"),
			Updated: r.PostForm.Has("decimals"),
		},
		Description: crudDataField[string]{
			Value:   r.PostForm.Get("description"),
			Updated: r.PostForm.Has("description"),
		},
	}
	if err = opts.validateCrudData(data); err != nil {
		return nil, err
	}

	return
}

func (opts *NamesOptions) getCrudDataEnv() (data *CrudData, err error) {
	data = &CrudData{}
	address, addressUpdated := os.LookupEnv("TB_NAME_ADDRESS")
	data.Address = crudDataField[base.Address]{
		Value:   base.HexToAddress(address),
		Updated: addressUpdated,
	}
	name, nameUpdated := os.LookupEnv("TB_NAME_NAME")
	data.Name = crudDataField[string]{
		Value:   name,
		Updated: nameUpdated,
	}
	tag, tagUpdated := os.LookupEnv("TB_NAME_TAG")
	data.Tag = crudDataField[string]{
		Value:   tag,
		Updated: tagUpdated,
	}
	source, sourceUpdated := os.LookupEnv("TB_NAME_SOURCE")
	data.Source = crudDataField[string]{
		Value:   source,
		Updated: sourceUpdated,
	}
	symbol, symbolUpdated := os.LookupEnv("TB_NAME_SYMBOL")
	data.Symbol = crudDataField[string]{
		Value:   symbol,
		Updated: symbolUpdated,
	}
	decimals, decimalsUpdated := os.LookupEnv("TB_NAME_DECIMALS")
	data.Decimals = crudDataField[string]{
		Value:   decimals,
		Updated: decimalsUpdated,
	}
	description, descriptionUpdated := os.LookupEnv("TB_NAME_DESCR")
	data.Description = crudDataField[string]{
		Value:   description,
		Updated: descriptionUpdated,
	}

	if err = opts.validateCrudData(data); err != nil {
		return nil, err
	}

	return
}

func (opts *NamesOptions) validateCrudData(data *CrudData) error {
	if data.Address.Value.IsZero() {
		// TODO: In the c++ this would pick up opts.Term[0] if it was present and use that
		return errors.New("address is required")
	}
	if data.Name.Value == "" {
		// TODO: In the c++ this would pick up opts.Term[0] if it was present and use that
		return errors.New("address is required")
	}
	// TODO: I think a name is also required here (but not in AutoName mode in which case
	// TODO: name is picked up from the contract)

	return nil
}

func (opts *NamesOptions) loadCrudDataIfNeeded(request *http.Request) (err error) {
	// These three read address from command arguments
	if opts.Delete || opts.Undelete || opts.Remove {
		if len(opts.Terms) != 1 {
			return errors.New("exactly 1 address required")
		}
		opts.crudData = &CrudData{
			Address: crudDataField[base.Address]{
				Value: base.HexToAddress(opts.Terms[0]),
			},
		}
		return
	}
	if !opts.createOrUpdate() {
		return
	}

	var data *CrudData
	if request == nil {
		data, err = opts.getCrudDataEnv()
	} else {
		data, err = opts.getCrudDataHttp(request)
	}

	opts.crudData = data
	return
}

// EXISTING_CODE
