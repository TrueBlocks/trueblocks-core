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
package output

import (
	"encoding/json"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

type JsonFormatted struct {
	Data   interface{} `json:"data,omitempty"`
	Errors []string    `json:"errors,omitempty"`
	Meta   *Meta       `json:"meta,omitempty"`
}

// AsJsonBytes marshals JsonFormatted struct, populating Meta field if
// needed
func AsJsonBytes(j *JsonFormatted) ([]byte, error) {
	var result JsonFormatted

	if Format == "json" {
		if len(validate.Errors) > 0 {
			result.Errors = validate.Errors
		} else {
			if len(j.Errors) > 0 {
				result.Errors = j.Errors
			} else {
				result.Data = j.Data
			}
		}
	} else {
		if len(j.Errors) > 0 {
			result.Errors = j.Errors
		} else {
			result.Data = j.Data
			result.Meta = GetMeta()
		}
	}

	marshalled, err := json.MarshalIndent(result, "", "  ")
	if err != nil {
		return nil, err
	}

	return marshalled, err
}

// PrintJson marshals its arguments and prints JSON in a standardized format
func PrintJson(j *JsonFormatted) error {
	marshalled, err := AsJsonBytes(j)
	if err != nil {
		return err
	}
	fmt.Fprintln(os.Stdout, string(marshalled))

	return nil
}
