package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type namer struct {
	addr base.Address
	name string
}

func nameAddress(extraOpts map[string]any, address base.Address) (Name, bool, bool) {
	if extraOpts["namesMap"] == nil {
		return Name{}, false, false
	}

	name := extraOpts["namesMap"].(map[base.Address]Name)[address]
	if name.Address.IsZero() {
		return Name{}, true, false
	}

	isTesting := extraOpts["testMode"] == true
	if isTesting {
		isCustom := name.IsCustom
		isIndividual := strings.Contains(name.Tags, "90-Individuals")
		if isIndividual || isCustom {
			return Name{}, true, false // makes testing with local customizations easier
		}
	}

	return name, true, true
}

func reorderOrdering(fields []string) []string {
	var normalFields []string
	nameFieldsMap := make(map[string]string)

	for _, field := range fields {
		if strings.HasSuffix(field, "Name") {
			baseField := strings.TrimSuffix(field, "Name")
			nameFieldsMap[baseField] = field
		} else {
			normalFields = append(normalFields, field)
		}
	}

	var result []string
	for _, field := range normalFields {
		result = append(result, field)
		if nameField, found := nameFieldsMap[field]; found {
			result = append(result, nameField)
		}
	}

	return result
}
