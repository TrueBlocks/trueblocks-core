// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package whenPkg

// EXISTING_CODE
import (
	"fmt"
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunWhen handles the when command for the command line. Returns error only as per cobra.
func RunWhen(cmd *cobra.Command, args []string) (err error) {
	opts := WhenFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.WhenInternal()
	return
}

// ServeWhen handles the when command for the API. Returns error and a bool if handled
func ServeWhen(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := WhenFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.WhenInternal()
}

// WhenInternal handles the internal workings of the when command.  Returns error and a bool if handled
func (opts *WhenOptions) WhenInternal() (err error, handled bool) {
	err = opts.ValidateWhen()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	handled = true

	if opts.List {
		err = opts.HandleWhenList()
	} else if opts.Timestamps {
		if opts.Count {
			err = opts.HandleWhenTimestampCount()
		} else {
			err = opts.HandleShowTimestamps()
		}
	} else {
		err = opts.HandleWhenBlocks()
	}
	// EXISTING_CODE

	return
}

// EXISTING_CODE
func (opts *WhenOptions) HandleShowTimestamps() error {
	cnt, err := tslibPkg.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	if !opts.Check {
		err = opts.Globals.RenderHeader(tslibPkg.Timestamp{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
		defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")
		if err != nil {
			return err
		}
		for bn := uint64(0); bn < cnt; bn++ {
			obj, err := tslibPkg.FromBn(opts.Globals.Chain, bn)
			if err != nil {
				return err
			}
			err = opts.Globals.RenderObject(*obj, false, bn == 0)
			if err != nil {
				return err
			}
		}
		return nil
	}

	scanBar := progress.NewScanBar(cnt, cnt/500, cnt, (2. / 3.))
	for bn := uint64(0); bn < cnt; bn++ {
		item, err := tslibPkg.FromBn(opts.Globals.Chain, bn)
		if err != nil {
			return err
		}
		block := types.NamedBlock{} //rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		if bn == 0 {
			block.TimeStamp = utils.EarliestTs
		}
		msg := fmt.Sprintf("%d-%d-%d-%d-%d", block.BlockNumber, block.TimeStamp, bn, item.Bn, item.Ts)
		scanBar.Report(opts.Globals.Writer, "Checking", msg)
	}
	return nil
}

// EXISTING_CODE
