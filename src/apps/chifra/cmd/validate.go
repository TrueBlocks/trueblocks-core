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

func fmtError(msg string) string {
	return "\n  " + msg + "\n"
}

func isValidAddress(addr string) (bool, error) {
	if len(addr) != 42 {
		return false, errors.New(fmtError("address (" + addr + ") is not 42 characters long"))
	} else if addr[:2] != "0x" {
		return false, errors.New(fmtError("address (" + addr + ") does not start with '0x'"))
	} else if len(strings.Trim(addr[2:], "0123456789abcdefABCDEF")) > 0 {
		return false, errors.New("address (" + addr + ") does not appear to be hex")
	}
	return true, nil
}

func validateOneAddr(args []string) error {
	for _, arg := range args {
		val, _ := isValidAddress(arg)
		if val {
			return nil
			// } else {
			// 	fmt.Println("%v", err)
		}
	}
	return errors.New(fmtError("At least one valid Ethereum address is required"))
}

func validateEnum(field, value, valid string) error {
	if len(value) == 0 {
		return nil
	}
	valid = strings.Replace(valid, "[", "|", 1)
	valid = strings.Replace(valid, "]", "|", 1)
	if strings.Contains(valid, "|"+value+"|") {
		return nil
	}
	parts := strings.Split(strings.Trim(valid, "|"), "|")
	list := ""
	for _, part := range parts {
		if len(list) > 0 {
			list += " | "
		}
		list += part
	}
	msg := "The " + field + " option ("
	msg += value
	msg += ") must be one of [ " + list + " ]"
	return errors.New(fmtError(msg))
}

func validateEnumSlice(field string, values []string, valid string) error {
	if len(values) == 0 {
		return nil
	}
	for _, value := range values {
		err := validateEnum(field, value, valid)
		if err != nil {
			return err
		}
	}
	return nil
}

func validateGlobalFlags(cmd *cobra.Command, args []string) error {
	err := validateEnum("--fmt", RootOpts.fmt, "[json|txt|csv|api]")
	if err != nil {
		return err
	}
	return nil
}
