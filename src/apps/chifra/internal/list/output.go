// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package listPkg

// EXISTING_CODE
import (
	"io"
	"net/http"
	"os"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/blockRange"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/ethereum/go-ethereum/common"
	"github.com/spf13/cobra"
)

// AddressMonitorMap carries arrays of appearances that have not yet been written to the monitor file
type AddressMonitorMap map[common.Address]monitor.Monitor

// ListOptionsExtended stores the original 'chifra list' command line options plus
type ListOptionsExtended struct {
	opts       *ListOptions
	writer     io.Writer
	maxTasks   int
	addrMonMap AddressMonitorMap
}

// EXISTING_CODE

func RunList(cmd *cobra.Command, args []string) error {
	opts := ListFinishParse(args)

	err := opts.ValidateList()
	if err != nil {
		return err
	}

	// EXISTING_CODE
	//if opts.Newone {
	//	optsEx := NewListOptsEx(opts)
	//	err = optsEx.HandleFreshenMonitors()
	//	if err != nil {
	//		return err
	//	}
	//	if opts.Count {
	//		return opts.HandleListCount()
	//	}
	//	// return nil
	//}

	// exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	exportPkg.GetOptions().Appearances = true
	if opts.Count {
		exportPkg.GetOptions().Count = true
	}
	if opts.FirstBlock > 0 {
		exportPkg.GetOptions().FirstBlock = opts.FirstBlock
	}
	if opts.LastBlock > 0 {
		exportPkg.GetOptions().LastBlock = opts.LastBlock
	}
	exportPkg.GetOptions().Globals = opts.Globals
	return exportPkg.RunExport(cmd, args)
	// EXISTING_CODE
}

func ServeList(w http.ResponseWriter, r *http.Request) bool {
	opts := FromRequest(w, r)

	err := opts.ValidateList()
	if err != nil {
		opts.Globals.RespondWithError(w, http.StatusInternalServerError, err)
		return true
	}

	// EXISTING_CODE
	// TODO: BOGUS -- HANDLE THIS IN GOLANG
	return false
	// EXISTING_CODE
}

// EXISTING_CODE
func NewListOptsEx(opts *ListOptions) (ret ListOptionsExtended) {
	ret.opts = opts
	ret.writer = os.Stdout
	ret.maxTasks = 12
	ret.addrMonMap = make(AddressMonitorMap, len(opts.Addrs))
	for _, addr := range opts.Addrs {
		a := common.HexToAddress(addr)
		m := monitor.NewMonitor(opts.Globals.Chain, addr)
		ret.addrMonMap[a] = m
	}
	return
}

func (optsEx *ListOptionsExtended) RangesIntersect(r2 blockRange.FileRange) bool {
	var r1 blockRange.FileRange
	r1.First = int(optsEx.opts.FirstBlock)
	r1.Last = int(optsEx.opts.LastBlock)
	return !(r1.Last < r2.First || r1.First > r2.Last)
}

// EXISTING_CODE
