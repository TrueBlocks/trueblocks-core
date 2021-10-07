package specialBlock

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
