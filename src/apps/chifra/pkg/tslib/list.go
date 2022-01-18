// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"encoding/csv"
	"io"
	"os"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type NamedBlock struct {
	BlockNumber uint64 `json:"blockNumber"`
	TimeStamp   uint64 `json:"timestamp"`
	Date        string `json:"date"`
	Name        string `json:"name"`
}

// GetSpecials returns a chain-specific list of special block names and numbers
func GetSpecials(chain string) ([]NamedBlock, error) {
	specialsPath := config.GetPathToChainConfig(chain) + "specials.csv"
	file, err := os.Open(specialsPath)
	if err != nil {
		return nil, err
	}
	reader := csv.NewReader(file)
	reader.FieldsPerRecord = 4

	first := true
	specials := []NamedBlock{}
	for {
		record, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return nil, err
		}
		if !first {
			bn, _ := strconv.ParseUint(record[0], 10, 64)
			ts, _ := strconv.ParseUint(record[2], 10, 64)
			specials = append(specials, NamedBlock{
				BlockNumber: bn,
				Name:        record[1],
				TimeStamp:   ts,
				Date:        record[3],
			})
		}
		first = false
	}

	return specials, nil
}

// IsSpecialBlock returns true if the given chain-specific name is a special block
func IsSpecialBlock(chain, needle string) bool {
	_, found := BnFromName(chain, needle)
	return found
}
