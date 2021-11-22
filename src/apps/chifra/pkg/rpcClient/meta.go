package rpcClient

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
	"context"
	"encoding/json"
)

type Meta struct {
	Unripe    uint64 `json:"unripe"`
	Ripe      uint64 `json:"ripe"`
	Staging   uint64 `json:"staging"`
	Finalized uint64 `json:"finalized"`
	Client    uint64 `json:"client"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func (m *Meta) Latest() uint64 {
	return GetMeta(false).Client
}

func GetMeta(testMode bool) *Meta {
	if testMode {
		return &Meta{
			Unripe:    0xdeadbeef,
			Ripe:      0xdeadbeef,
			Staging:   0xdeadbeef,
			Finalized: 0xdeadbeef,
			Client:    0xdeadbeef,
		}
	}
	client := Get()
	bn, _ := client.BlockNumber(context.Background())
	return &Meta{Client: bn}
}
