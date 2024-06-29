package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type namer struct {
	addr base.Address
	name string
}

func nameAddress(extraOpts map[string]any, address base.Address) (Name, bool) {
	if extraOpts["namesMap"] == nil {
		return Name{}, false
	}

	name := extraOpts["namesMap"].(map[base.Address]Name)[address]
	if name.Address.Hex() == "0x0" {
		return Name{}, false
	}

	isTesting := extraOpts["testMode"] == true
	if isTesting {
		isCustom := name.IsCustom
		isIndividual := strings.Contains(name.Tags, "90-Individuals")
		if isIndividual || isCustom {
			return Name{}, false // makes testing with local customizations easier
		}
	}

	isNotZero := name.Address.Hex() != "0x0"
	name.Address = base.Address{}
	return name, isNotZero
}
