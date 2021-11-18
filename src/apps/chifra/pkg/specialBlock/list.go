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
package specialBlock

var blocks = map[string]int{
	"first":          0,
	"firstTrans":     46147,
	"firstContract":  50111,
	"iceage":         200000,
	"devcon1":        543626,
	"homestead":      1150000,
	"daofund":        1428756,
	"daohack":        1718497,
	"daofork":        1920000,
	"devcon2":        2286910,
	"tangerine":      2463000,
	"spurious":       2675000,
	"stateclear":     2717576,
	"eea":            3265360,
	"ens2":           3327417,
	"parityhack1":    4041179,
	"byzantium":      4370000,
	"devcon3":        4469339,
	"parityhack2":    4501969,
	"kitties":        4605167,
	"makerdao":       4620855,
	"devcon4":        6610517,
	"uniswap":        6627917,
	"constantinople": 7280000,
	"devcon5":        8700401,
	"mcdai":          8928158,
	"istanbul":       9069000,
	"muirglacier":    9200000,
	"berlin":         12244000,
	"london":         12965000,
	"arrowglacier":   13773000,
	"latest":         -1,
}

func IsStringSpecialBlock(blockName string) bool {
	_, found := blocks[blockName]

	return found
}

func GetNameByValue(blockNumber int) (string, bool) {
	for key, value := range blocks {
		if value == blockNumber {
			return key, true
		}
	}

	return "", false
}

func GetValueByName(blockName string) (int, bool) {
	value, found := blocks[blockName]

	return value, found
}
