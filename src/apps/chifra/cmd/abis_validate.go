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
package cmd

import (
	"errors"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

func makeErrorEx(function, msg string, values []string) error {
	var ret string
	if len(function) > 0 {
		ret = function + ": "
	}
	ret += msg
	for index, val := range values {
		rep := "{" + strconv.FormatInt(int64(index), 10) + "}"
		ret = strings.Replace(ret, rep, val, -1)
	}
	return errors.New(fmtError(ret))
}
func makeError(msg string, values ...string) error {
	return makeErrorEx("", msg, values)
}

func validateAbisArgs(cmd *cobra.Command, args []string) error {
	if AbisOpts.classes {
		return makeError("the '{0}' option is not implemented", "--classes")
	}

	if len(AbisOpts.sol) > 0 {
		cleaned := "./" + strings.Replace(AbisOpts.sol, ".sol", "", 1) + ".sol"
		if !utils.FileExists(cleaned) {
			return makeError("file not found at {0}", cleaned)
		}
		return nil
	}

	if len(AbisOpts.find) == 0 && !AbisOpts.known {
		err := validateOneAddr(args)
		if err != nil {
			return err
		}
	}

	err := validateGlobalFlags(cmd, args)
	if err != nil {
		return err
	}
	return nil
}
