// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package manifest

import "encoding/json"

type IpfsHash = string

type PinDescriptor struct {
	FileName  string   `json:"fileName"`
	BloomHash IpfsHash `json:"bloomHash"`
	IndexHash IpfsHash `json:"indexHash"`
	// BloomChecksum string   `json:"bloomChecksum"`
	// IndexChecksum string   `json:"indexChecksum"`
	// file size (gzipped file)
	// md5
}

type Manifest struct {
	Version     json.Number   `json:"version"`
	Chain       string        `json:"chain"`
	ChainId     json.Number   `json:"chainId"` // not sure
	IndexFormat IpfsHash      `json:"indexFormat"`
	BloomFormat IpfsHash      `json:"bloomFormat"`
	CommitHash  string        `json:"commitHash"`
	BlockRange  ManifestRange `json:"blockRange"`
	Names       IpfsHash      `json:"names"`
	Timestamps  IpfsHash      `json:"timestamps"`
	Pins        PinsList      `json:"pins"`
}

// This type is used to carry CSV layout information
type CsvFormatted struct {
	Header  []string
	Content [][]string
}

type PinsList []PinDescriptor

// GetCsvOutput returns data for CSV and TSV formats
func (pl *PinsList) GetCsvOutput() *CsvFormatted {
	data := &CsvFormatted{
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
