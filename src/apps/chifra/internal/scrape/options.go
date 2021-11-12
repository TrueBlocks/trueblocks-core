package scrape

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

type ScrapeOptionsType struct {
	Action       string
	Sleep        float64
	Pin          bool
	Publish      bool
	BlockCnt     uint64
	BlockChanCnt uint64
	AddrChanCnt  uint64
}

var Options ScrapeOptionsType

func (opts *ScrapeOptionsType) TestLog() {
	logger.Log(logger.Test, "Action: ", opts.Action)
	logger.Log(logger.Test, "Sleep: ", opts.Sleep)
	if opts.Pin {
		logger.Log(logger.Test, "Pin: ", opts.Pin)
	}
	if opts.Publish {
		logger.Log(logger.Test, "Publish: ", opts.Publish)
	}
	logger.Log(logger.Test, "BlockCnt: ", opts.BlockCnt)
	logger.Log(logger.Test, "BlockChanCnt: ", opts.BlockChanCnt)
	logger.Log(logger.Test, "AddrChanCnt: ", opts.AddrChanCnt)
}