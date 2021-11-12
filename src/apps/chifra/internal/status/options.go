package status

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

type StatusOptionsType struct {
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
}

var Options StatusOptionsType

func (opts *StatusOptionsType) TestLog() {
	if opts.Details {
		logger.Log(logger.Test, "Details: ", opts.Details)
	}
	if len(opts.Types) > 0 {
		logger.Log(logger.Test, "Types: ", opts.Types)
	}
	logger.Log(logger.Test, "Depth: ", opts.Depth)
	if opts.Terse {
		logger.Log(logger.Test, "Terse: ", opts.Terse)
	}
	if len(opts.Migrate) > 0 {
		logger.Log(logger.Test, "Migrate: ", opts.Migrate)
	}
	if opts.GetConfig {
		logger.Log(logger.Test, "GetConfig: ", opts.GetConfig)
	}
	if opts.SetConfig {
		logger.Log(logger.Test, "SetConfig: ", opts.SetConfig)
	}
	logger.Log(logger.Test, "TestStart: ", opts.TestStart)
	logger.Log(logger.Test, "TestEnd: ", opts.TestEnd)
}