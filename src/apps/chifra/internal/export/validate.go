package exportPkg

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

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ExportOptions) ValidateExport() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateAtLeastOneAddr(opts.Addrs)
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("--summarize_by", Options.SummarizeBy, "[yearly|quarterly|monthly|weekly|daily|hourly|blockly|tx]")
	if err != nil {
		return err
	}

	if len(Options.SummarizeBy) > 0 && !Options.Accounting {
		return validate.Usage("The {0} option is available only with {1}.", "--summarized_by", "--accounting")
	}

	return opts.Globals.ValidateGlobals()
}
