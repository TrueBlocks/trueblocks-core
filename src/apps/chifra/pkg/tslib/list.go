// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslibPkg

import (
	"encoding/csv"
	"errors"
	"io"
	"os"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetSpecials returns a chain-specific list of special block names and numbers
func GetSpecials(chain string) ([]types.NamedBlock, error) {
	specialsPath := config.GetPathToChainConfig(chain) + "specials.csv"
	_, err := os.Stat(specialsPath)
	if err != nil {
		// It's okay if there are no specials for a certain chain
		logger.Log(logger.Info, "No special block file found for chain ", chain)
		return []types.NamedBlock{}, nil
	}

	file, err := os.Open(specialsPath)
	if err != nil {
		return nil, err
	}
	reader := csv.NewReader(file)
	reader.FieldsPerRecord = 4

	first := true
	specials := []types.NamedBlock{}
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
			specials = append(specials, types.NamedBlock{
				BlockNumber: bn,
				Name:        record[1],
				TimeStamp:   ts,
				Date:        record[3],
			})
		}
		first = false
	}

	if len(specials) == 0 {
		return nil, errors.New("found no special blocks")
	}

	return specials, nil
}

// IsSpecialBlock returns true if the given chain-specific name is a special block
func IsSpecialBlock(chain, needle string) bool {
	_, err := BnFromName(chain, needle)
	return err == nil
}
