// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package blazePkg

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type BlazeOptions struct {
	StartBlock   uint64
	BlockCnt     uint64
	BlockChanCnt uint64
	AddrChanCnt  uint64
	RipeBlock    uint64
	Globals      globals.GlobalOptions
	BadFlag      error
}

var blazeCmdLineOptions BlazeOptions

func (opts *BlazeOptions) TestLog() {
	logger.TestLog(opts.StartBlock != 0, "StartBlock: ", opts.StartBlock)
	logger.TestLog(opts.BlockCnt != 2000, "BlockCnt: ", opts.BlockCnt)
	logger.TestLog(opts.BlockChanCnt != 10, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.TestLog(opts.AddrChanCnt != 20, "AddrChanCnt: ", opts.AddrChanCnt)
	logger.TestLog(opts.RipeBlock != 0, "RipeBlock: ", opts.RipeBlock)
	opts.Globals.TestLog()
}

func (opts *BlazeOptions) ToCmdLine() string {
	options := ""
	options += " " + strings.Join([]string{}, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *BlazeOptions {
	opts := &BlazeOptions{}
	opts.StartBlock = 0
	opts.BlockCnt = 2000
	opts.BlockChanCnt = 10
	opts.AddrChanCnt = 20
	opts.RipeBlock = 0
	for key, value := range r.URL.Query() {
		switch key {
		case "startBlock":
			opts.StartBlock = globals.ToUint64(value[0])
		case "blockCnt":
			opts.BlockCnt = globals.ToUint64(value[0])
		case "blockChanCnt":
			opts.BlockChanCnt = globals.ToUint64(value[0])
		case "addrChanCnt":
			opts.AddrChanCnt = globals.ToUint64(value[0])
		case "ripeBlock":
			opts.RipeBlock = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "blaze")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)
	// EXISTING_CODE
	// EXISTING_CODE

	return opts
}

func BlazeFinishParse(args []string) *BlazeOptions {
	opts := GetOptions()
	// EXISTING_CODE
	// EXISTING_CODE
	return opts
}

func GetOptions() *BlazeOptions {
	// EXISTING_CODE
	// EXISTING_CODE
	return &blazeCmdLineOptions
}
