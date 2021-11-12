package abis

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
 * The file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type AbisOptionsType struct {
	Known   bool
	Sol     bool
	Find    []string
	Source  bool
	Classes bool
}

var Options AbisOptionsType

func (opts *AbisOptionsType) TestLog() {
	if opts.Known {
		logger.Log(logger.Test, "Known: ", opts.Known)
	}
	if opts.Sol {
		logger.Log(logger.Test, "Sol: ", opts.Sol)
	}
	if len(opts.Find) > 0 {
		logger.Log(logger.Test, "Find: ", opts.Find)
	}
	if opts.Source {
		logger.Log(logger.Test, "Source: ", opts.Source)
	}
	if opts.Classes {
		logger.Log(logger.Test, "Classes: ", opts.Classes)
	}
}