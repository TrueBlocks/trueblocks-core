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
package main

import (
	"fmt"
	"log"

	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/ethclient"
)

func main() {
	conn, err := ethclient.Dial("http://localhost:36963")
	if err != nil {
		log.Fatalf("Failed to connect: %v", err)
	}

	contract, err := NewUnchainedIndex(common.HexToAddress("0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"), conn)
	if err != nil {
		log.Fatalf("Could not connect to contract: %v", err)
	}

    // contract.PublishHash(nil, "0x12")
	value1, _ := contract.Owner(nil)
	fmt.Printf("Owner: 0x%x\n", value1)

	value, _ := contract.ManifestHash(nil)
	fmt.Printf("Latest Hash: %s\n", value)
}
