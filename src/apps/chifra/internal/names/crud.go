// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

import (
	"errors"
	"net/http"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
)

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

type CrudData struct {
	Address  crudDataField[base.Address]
	Name     crudDataField[string]
	Tags     crudDataField[string]
	Source   crudDataField[string]
	Symbol   crudDataField[string]
	Decimals crudDataField[string]
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
		Tags: crudDataField[string]{
			Value:   r.PostForm.Get("tags"),
			Updated: r.PostForm.Has("tags"),
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
	tags, tagsUpdated := os.LookupEnv("TB_NAME_TAGS")
	data.Tags = crudDataField[string]{
		Value:   tags,
		Updated: tagsUpdated,
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

func (opts *NamesOptions) LoadCrudDataIfNeeded(request *http.Request) error {
	if opts.Delete || opts.Undelete || opts.Remove {
		opts.crudData = &CrudData{
			Address: crudDataField[base.Address]{
				Value: base.HexToAddress(opts.Terms[0]),
			},
		}
		return nil
	}

	if !opts.Create && !opts.Update {
		return nil
	}

	var err error
	if request == nil {
		opts.crudData, err = opts.getCrudDataEnv()
	} else {
		opts.crudData, err = opts.getCrudDataHttp(request)
	}
	return err
}
