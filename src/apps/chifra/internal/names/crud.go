// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package namesPkg

import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/crud"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) getType() types.Parts {
	var ret types.Parts

	if opts.Custom || opts.All || opts.anyCrud() {
		ret |= types.Custom
	}

	if opts.Prefund || opts.All {
		ret |= types.Prefund
	}

	if (!opts.Custom && !opts.Prefund) || opts.All {
		ret |= types.Regular
	}

	if opts.MatchCase {
		ret |= types.MatchCase
	}

	if opts.Expand {
		ret |= types.Expanded
	}

	if opts.Tags {
		ret |= types.Tags
	}

	if opts.Globals.TestMode {
		ret |= types.Testing
	}

	return ret
}

func (opts *NamesOptions) LoadCrudDataIfNeeded(request *http.Request) error {
	if opts.Delete || opts.Undelete || opts.Remove {
		opts.crudData = &crud.NameCrud{
			Address: crud.Field[base.Address]{
				Value: base.HexToAddress(opts.Terms[0]),
			},
		}
		return nil
	}

	if !opts.Create && !opts.Update {
		return nil
	}

	var err error
	opts.crudData, err = crud.NewNameCrud(len(opts.Autoname) > 0, request)
	return err
}
