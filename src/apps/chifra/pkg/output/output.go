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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

var OutputFn string
var Format string

type Meta struct {
	Unripe    string `json:"unripe"`
	Ripe      string `json:"ripe"`
	Staging   string `json:"staging"`
	Finalized string `json:"finalized"`
	Client    string `json:"client"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func GetMeta() *Meta {
	if utils.IsTestMode() {
		return &Meta{
			Unripe:    "0xdeadbeef",
			Ripe:      "0xdeadbeef",
			Staging:   "0xdeadbeef",
			Finalized: "0xdeadbeef",
			Client:    "0xdeadbeef",
		}
	}
	return &Meta{Client: "12"}
}
