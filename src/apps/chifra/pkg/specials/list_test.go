package specials

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
	"testing"
)

func TestIsStringSpecialBlock(t *testing.T) {
	result := IsStringSpecialBlock("devcon1")

	if !result {
		t.Error("Fails for valid block name")
	}

	shouldBeFalse := IsStringSpecialBlock("nosuchblock")

	if shouldBeFalse {
		t.Error("Passes for invalid block name")
	}
}

func TestGetNameByValue(t *testing.T) {
	name, found := GetNameByValue(2463000)

	if !found {
		t.Error("Block name not found")
	}

	if name != "tangerine" {
		t.Errorf("Wrong name: %s", name)
	}
}

func TestGetValueByName(t *testing.T) {
	value, found := GetValueByName("tangerine")

	if !found {
		t.Error("Block not found by name")
	}

	if value != 2463000 {
		t.Errorf("Wrong value: %d", value)
	}
}
