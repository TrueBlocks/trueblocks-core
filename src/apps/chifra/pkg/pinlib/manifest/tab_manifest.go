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
package manifest

import (
	"encoding/csv"
	"errors"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// ReadPinDescriptors parses pin information and returns a slice of
// PinDescriptor or error
func ReadPinDescriptors(r io.Reader) ([]PinDescriptor, error) {
	reader := csv.NewReader(r)
	reader.Comma = '\t'
	reader.FieldsPerRecord = 3

	descriptors := []PinDescriptor{}

	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}

		descriptors = append(descriptors, PinDescriptor{
			FileName:  record[0],
			BloomHash: record[1],
			IndexHash: record[2],
		})
	}

	return descriptors, nil
}

// BuildTabRange finds the first and last pin descriptor and returns
// ManifestRange. We need this function, because TSV formatted manifest
// does not carry this information explicitly
func BuildTabRange(descriptors []PinDescriptor) (ManifestRange, error) {
	if len(descriptors) == 0 {
		return ManifestRange{}, errors.New("invalid input: no pins")
	}

	firstPinRange, err := StringToManifestRange(descriptors[0].FileName)
	if err != nil {
		return ManifestRange{}, err
	}

	lastPinRange, err := StringToManifestRange(descriptors[len(descriptors)-1].FileName)
	if err != nil {
		return ManifestRange{}, err
	}

	return ManifestRange{
		firstPinRange[0],
		lastPinRange[1],
	}, nil
}

// ReadTabManifest parses TSV formatted manifest
func ReadTabManifest(r io.Reader) (*Manifest, error) {
	descriptors, err := ReadPinDescriptors(r)
	if err != nil {
		return nil, err
	}

	newBlockRange, err := BuildTabRange(descriptors)
	if err != nil {
		return nil, err
	}

	return &Manifest{
		FileName:           "",
		IndexFormat:        "",
		BloomFormat:        "",
		CommitHash:         "",
		PreviousHash:       "",
		NewBlockRange:      newBlockRange,
		PreviousBlockRange: ManifestRange{0, 0},
		NewPins:            descriptors,
		PreviousPins:       []PinDescriptor{},
	}, nil
}

// FromLocalFile loads the manifest saved in ConfigPath
func FromLocalFile() (*Manifest, error) {
	file, err := os.Open(config.GetConfigPath("manifest/manifest.txt"))
	if err != nil {
		return nil, err
	}

	return ReadTabManifest(file)
}
