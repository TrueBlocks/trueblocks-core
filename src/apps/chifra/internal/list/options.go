package list

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type ListOptionsType struct {
	Count       bool
	Appearances bool
	FirstBlock  uint64
	LastBlock   uint64
}

var Options ListOptionsType

func (opts *ListOptionsType) TestLog() {
	if opts.Count {
		logger.Log(logger.Test, "Count: ", opts.Count)
	}
	if opts.Appearances {
		logger.Log(logger.Test, "Appearances: ", opts.Appearances)
	}
	if opts.FirstBlock != 0 {
		logger.Log(logger.Test, "FirstBlock: ", opts.FirstBlock)
	}
	if opts.LastBlock != utils.NOPOS {
		logger.Log(logger.Test, "LastBlock: ", opts.LastBlock)
	}
}