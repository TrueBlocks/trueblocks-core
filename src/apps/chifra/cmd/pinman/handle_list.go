package pinman

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

// HandleList loads manifest, sorts pins and prints them out
func HandleList(format string) {
	// Load manifest
	manifestData, err := manifest.FromLocalFile()
	if err != nil {
		logger.Fatal("Cannot open local manifest file", err)
	}

	// Sort pins
	sort.Slice(manifestData.NewPins, func(i, j int) bool {
		iPin := manifestData.NewPins[i]
		jPin := manifestData.NewPins[j]

		return iPin.FileName < jPin.FileName
	})

	if format == "json" {
		err := output.PrintJson(manifestData.NewPins)
		if err != nil {
			logger.Fatal(err)
		}

		return
	}

	table := &output.Table{}
	table.New()
	table.Header([]string{
		"filename",
		"bloomhash",
		"indexhash",
	})

	for _, pin := range manifestData.NewPins {
		table.Row([]string{pin.FileName, pin.BloomHash, pin.IndexHash})
	}
	table.Print()
}
