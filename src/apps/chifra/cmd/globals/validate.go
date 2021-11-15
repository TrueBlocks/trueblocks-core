package globals

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
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func ValidateGlobals(opts *GlobalOptionsType) error {
	if len(opts.File) > 0 && !utils.FileExists(opts.File) {
		return validate.Usage("The {0} option ({1}) must {2}", "file", opts.File, "exist")
	}

	err := validate.ValidateEnum("--fmt", opts.Format, "[json|txt|csv|api]")
	if err != nil {
		return err
	}
	return nil
}

func ToFloat64(val string) float64 {
	f, _ := strconv.ParseFloat(val, 64)
	return f
}

func ToUint64(val string) uint64 {
	f, _ := strconv.ParseUint(val, 10, 64)
	return f
}
