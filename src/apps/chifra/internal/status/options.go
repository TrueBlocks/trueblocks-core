package statusPkg

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type StatusOptions struct {
	Modes     []string
	Details   bool
	Types     []string
	Depth     uint64
	Report    bool
	Terse     bool
	Migrate   []string
	GetConfig bool
	SetConfig bool
	TestStart uint64
	TestEnd   uint64
	Globals   globals.GlobalOptionsType
	BadFlag   error
}

func (opts *StatusOptions) TestLog() {
	logger.TestLog(len(opts.Modes) > 0, "Modes: ", opts.Modes)
	logger.TestLog(opts.Details, "Details: ", opts.Details)
	logger.TestLog(len(opts.Types) > 0, "Types: ", opts.Types)
	logger.TestLog(opts.Depth != globals.NOPOS, "Depth: ", opts.Depth)
	logger.TestLog(opts.Terse, "Terse: ", opts.Terse)
	logger.TestLog(len(opts.Migrate) > 0, "Migrate: ", opts.Migrate)
	logger.TestLog(opts.GetConfig, "GetConfig: ", opts.GetConfig)
	logger.TestLog(opts.SetConfig, "SetConfig: ", opts.SetConfig)
	logger.TestLog(opts.TestStart != 0, "TestStart: ", opts.TestStart)
	logger.TestLog(opts.TestEnd != globals.NOPOS, "TestEnd: ", opts.TestEnd)
	opts.Globals.TestLog()
}

func (opts *StatusOptions) ToCmdLine() string {
	options := ""
	if opts.Details {
		options += " --details"
	}
	for _, types := range opts.Types {
		options += " --types " + types
	}
	if opts.Depth != globals.NOPOS {
		options += (" --depth " + fmt.Sprintf("%d", opts.Depth))
	}
	if opts.Terse {
		options += " --terse"
	}
	for _, migrate := range opts.Migrate {
		options += " --migrate " + migrate
	}
	if opts.GetConfig {
		options += " --get_config"
	}
	if opts.SetConfig {
		options += " --set_config"
	}
	if opts.TestStart != 0 {
		options += (" --test_start " + fmt.Sprintf("%d", opts.TestStart))
	}
	if opts.TestEnd != globals.NOPOS {
		options += (" --test_end " + fmt.Sprintf("%d", opts.TestEnd))
	}
	options += " " + strings.Join(opts.Modes, " ")
	options += fmt.Sprintf("%s", "") // silence go compiler for auto gen
	return options
}

func FromRequest(w http.ResponseWriter, r *http.Request) *StatusOptions {
	opts := &StatusOptions{}
	for key, value := range r.URL.Query() {
		switch key {
		case "modes":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Modes = append(opts.Modes, s...)
			}
		case "details":
			opts.Details = true
		case "types":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Types = append(opts.Types, s...)
			}
		case "depth":
			opts.Depth = globals.ToUint64(value[0])
		case "report":
			opts.Report = true
		case "terse":
			opts.Terse = true
		case "migrate":
			for _, val := range value {
				s := strings.Split(val, " ") // may contain space separated items
				opts.Migrate = append(opts.Migrate, s...)
			}
		case "get_config":
			opts.GetConfig = true
		case "set_config":
			opts.SetConfig = true
		case "test_start":
			opts.TestStart = globals.ToUint64(value[0])
		case "test_end":
			opts.TestEnd = globals.ToUint64(value[0])
		default:
			if !globals.IsGlobalOption(key) {
				opts.BadFlag = validate.Usage("Invalid key ({0}) in {1} route.", key, "status")
				return opts
			}
		}
	}
	opts.Globals = *globals.FromRequest(w, r)

	return opts
}

var Options StatusOptions

func StatusFinishParse(args []string) *StatusOptions {
	Options.Modes = args
	// EXISTING_CODE
	// EXISTING_CODE
	return &Options
}
