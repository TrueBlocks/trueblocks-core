package quotesPkg

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

func (opts *QuotesOptions) ValidateQuotes() error {
	if len(Options.Globals.File) == 0 &&
		!Options.Freshen &&
		len(Options.Period) == 0 &&
		len(Options.Pair) == 0 &&
		len(Options.Feed) == 0 &&
		len(Options.Globals.Format) == 0 {
		return validate.Usage("Please specify at least one {0}.", "command line option")
	}

	err := validate.ValidateEnum("--period", Options.Period, "[5|15|30|60|120|240|1440|10080|hourly|daily|weekly]")
	if err != nil {
		return err
	}

	err = validate.ValidateEnum("--types", Options.Feed, "[poloniex|maker|tellor]")
	if err != nil {
		return err
	}

	Options.TestLog()

	return opts.Globals.ValidateGlobals()
}
