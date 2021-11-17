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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
)

type IpfsHash = string

type PinDescriptor struct {
	FileName  string   `json:"fileName"`
	BloomHash IpfsHash `json:"bloomHash"`
	IndexHash IpfsHash `json:"indexHash"`
}

type Manifest struct {
	// TODO: This structure will be updated with better data shortly
	FileName           string        `json:"fileName"`
	IndexFormat        IpfsHash      `json:"indexFormat"`
	BloomFormat        IpfsHash      `json:"bloomFormat"`
	CommitHash         string        `json:"commitHash"`
	PreviousHash       IpfsHash      `json:"prevHash"`
	NewBlockRange      ManifestRange `json:"newBlockRange"`
	PreviousBlockRange ManifestRange `json:"prevBlockRange"`
	NewPins            PinsList      `json:"newPins"`
	PreviousPins       PinsList      `json:"prevPins"`
}

type PinsList []PinDescriptor

// GetCsvOutput returns data for CSV and TSV formats
func (pl *PinsList) GetCsvOutput() *globals.CsvFormatted {
	data := &globals.CsvFormatted{
		Header: []string{
			"fileName", "bloomHash", "indexHash",
		},
	}

	for _, pin := range *pl {
		data.Content = append(data.Content, []string{
			pin.FileName, pin.BloomHash, pin.IndexHash,
		})
	}

	return data
}

// GetJsonOutput returns data for JSON format. In this case
// we just return the whole slice of PinDescriptor
func (pl *PinsList) GetJsonOutput() interface{} {
	return *pl
}
