package types

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawMonitor interface {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleMonitor struct {
	Address     string      `json:"address"`
	NRecords    int         `json:"nRecords"`
	FileSize    int64       `json:"fileSize"`
	LastScanned uint32      `json:"lastScanned"`
	Deleted     bool        `json:"deleted"`
	raw         *RawMonitor `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleMonitor) Raw() *RawMonitor {
	return s.raw
}

func (s *SimpleMonitor) SetRaw(raw *RawMonitor) {
	s.raw = raw
}

func (s *SimpleMonitor) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":  s.Address,
		"nRecords": s.NRecords,
		"fileSize": s.FileSize,
	}
	order = []string{
		"address",
		"nRecords",
		"fileSize",
	}
	if verbose {
		model["lastScanned"] = s.LastScanned
		model["deleted"] = s.Deleted
		if extraOptions["testMode"] == true {
			model["lastScanned"] = "--lastScanned--"
		}
		order = append(order, "lastScanned")
		order = append(order, "deleted")
		if extraOptions["namesMap"] != nil {
			name := extraOptions["namesMap"].(map[base.Address]SimpleName)[base.HexToAddress(s.Address)]
			if name.Address.Hex() != "0x0" {
				model["name"] = name
				order = append(order, "name")
			}
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
