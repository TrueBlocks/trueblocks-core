package pinlib

import (
	"encoding/csv"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
)

// SaveManifest saves the given manifest to the given file
func SaveManifest(filepath string, manifest *manifest.Manifest) error {
	targetFile, err := os.Create(filepath)
	if err != nil {
		return fmt.Errorf("creating file: %s", err)
	}
	defer targetFile.Close()
	err = file.Lock(targetFile)
	if err != nil {
		return fmt.Errorf("locking file: %s", err)
	}

	records := [][]string{}

	for _, pin := range manifest.NewPins {
		records = append(records, []string{
			pin.FileName,
			pin.BloomHash,
			pin.IndexHash,
		})
	}

	w := csv.NewWriter(targetFile)
	w.Comma = '\t'
	w.WriteAll(records)
	if err := w.Error(); err != nil {
		return err
	}

	return nil
}
