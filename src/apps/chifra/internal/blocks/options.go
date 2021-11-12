package blocks

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

type BlocksOptionsType struct {
	Hashes     bool
	Uncles     bool
	Trace      bool
	Apps       bool
	Uniq       bool
	UniqTx     bool
	Logs       bool
	Emitter    []string
	Topic      []string
	Articulate bool
	BigRange   uint64
	Count      bool
	Cache      bool
	List       uint64
	ListCount  uint64
}

var Options BlocksOptionsType

func (opts *BlocksOptionsType) TestLog() {
	if opts.Hashes {
		logger.Log(logger.Test, "Hashes: ", opts.Hashes)
	}
	if opts.Uncles {
		logger.Log(logger.Test, "Uncles: ", opts.Uncles)
	}
	if opts.Trace {
		logger.Log(logger.Test, "Trace: ", opts.Trace)
	}
	if opts.Apps {
		logger.Log(logger.Test, "Apps: ", opts.Apps)
	}
	if opts.Uniq {
		logger.Log(logger.Test, "Uniq: ", opts.Uniq)
	}
	if opts.UniqTx {
		logger.Log(logger.Test, "UniqTx: ", opts.UniqTx)
	}
	if opts.Logs {
		logger.Log(logger.Test, "Logs: ", opts.Logs)
	}
	if len(opts.Emitter) > 0 {
		logger.Log(logger.Test, "Emitter: ", opts.Emitter)
	}
	if len(opts.Topic) > 0 {
		logger.Log(logger.Test, "Topic: ", opts.Topic)
	}
	if opts.Articulate {
		logger.Log(logger.Test, "Articulate: ", opts.Articulate)
	}
	logger.Log(logger.Test, "BigRange: ", opts.BigRange)
	if opts.Count {
		logger.Log(logger.Test, "Count: ", opts.Count)
	}
	if opts.Cache {
		logger.Log(logger.Test, "Cache: ", opts.Cache)
	}
	logger.Log(logger.Test, "List: ", opts.List)
	logger.Log(logger.Test, "ListCount: ", opts.ListCount)
}