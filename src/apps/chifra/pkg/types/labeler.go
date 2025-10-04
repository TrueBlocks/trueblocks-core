package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type Labeler struct {
	addr base.Address
	name string
}

func NewLabeler(addr base.Address, name string) Labeler {
	return Labeler{
		addr: addr,
		name: name,
	}
}

func labelAddress(extraOpts map[string]any, address base.Address) (Name, bool, bool) {
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

// labelAddresses adds name resolution for a list of addresses to the model
func labelAddresses(p *ModelProps, model map[string]any, namers []Labeler) map[string]any {
	if p.ExtraOpts["namesMap"] != nil {
		for _, item := range namers {
			key := item.name + "Name"
			if result, loaded, found := labelAddress(p.ExtraOpts, item.addr); found {
				model[key] = result.Name
			} else if loaded && p.Format != "json" {
				model[key] = ""
			}
		}
	}
	return model
}

func reorderFields(fields []string) []string {
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

	result := make([]string, 0, 2*len(normalFields))
	for _, field := range normalFields {
		result = append(result, field)
		if nameField, found := nameFieldsMap[field]; found {
			result = append(result, nameField)
		}
	}

	return result
}
