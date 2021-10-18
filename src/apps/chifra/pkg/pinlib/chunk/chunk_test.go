package chunk

import (
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

func Test_exclude(t *testing.T) {
	fileNames := map[string]bool{
		"013337527-013340418": true,
		"013340419-013343305": true,
		"013346064-013348861": true,
		"013348862-013351760": true,
	}

	pins := []manifest.PinDescriptor{
		{
			FileName: "013337527-013340418",
		},
		{
			FileName: "013340419-013343305",
		},
		{
			FileName: "013346064-013348861",
		},
		{
			FileName: "013348862-013351760",
		},
		{
			FileName: "013387069-013389874",
		},
		{
			FileName: "013389875-013392800",
		},
	}

	result := exclude(fileNames, pins)

	if len(result) != 2 {
		t.Errorf("Wrong length: %d", len(result))
	}

	if result[0].FileName != "013387069-013389874" &&
		result[1].FileName != "013389875-013392800" {
		t.Errorf("Bad values: '%s' and '%s'", result[0].FileName, result[1].FileName)
	}
}
