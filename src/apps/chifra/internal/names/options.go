package names

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

type NamesOptionsType struct {
	Expand      bool
	MatchCase   bool
	All         bool
	Custom      bool
	Prefund     bool
	Named       bool
	Addr        bool
	Collections bool
	Tags        bool
	ToCustom    bool
	Clean       bool
	Autoname    string
	Create      bool
	Delete      bool
	Update      bool
	Remove      bool
	Undelete    bool
}

var Options NamesOptionsType

func (opts *NamesOptionsType) TestLog() {
	if opts.Expand {
		logger.Log(logger.Test, "Expand: ", opts.Expand)
	}
	if opts.MatchCase {
		logger.Log(logger.Test, "MatchCase: ", opts.MatchCase)
	}
	if opts.All {
		logger.Log(logger.Test, "All: ", opts.All)
	}
	if opts.Custom {
		logger.Log(logger.Test, "Custom: ", opts.Custom)
	}
	if opts.Prefund {
		logger.Log(logger.Test, "Prefund: ", opts.Prefund)
	}
	if opts.Named {
		logger.Log(logger.Test, "Named: ", opts.Named)
	}
	if opts.Addr {
		logger.Log(logger.Test, "Addr: ", opts.Addr)
	}
	if opts.Collections {
		logger.Log(logger.Test, "Collections: ", opts.Collections)
	}
	if opts.Tags {
		logger.Log(logger.Test, "Tags: ", opts.Tags)
	}
	if opts.ToCustom {
		logger.Log(logger.Test, "ToCustom: ", opts.ToCustom)
	}
	if opts.Clean {
		logger.Log(logger.Test, "Clean: ", opts.Clean)
	}
	logger.Log(logger.Test, "Autoname: ", opts.Autoname)
	if opts.Create {
		logger.Log(logger.Test, "Create: ", opts.Create)
	}
	if opts.Delete {
		logger.Log(logger.Test, "Delete: ", opts.Delete)
	}
	if opts.Update {
		logger.Log(logger.Test, "Update: ", opts.Update)
	}
	if opts.Remove {
		logger.Log(logger.Test, "Remove: ", opts.Remove)
	}
	if opts.Undelete {
		logger.Log(logger.Test, "Undelete: ", opts.Undelete)
	}
}