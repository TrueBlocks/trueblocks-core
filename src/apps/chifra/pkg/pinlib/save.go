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
