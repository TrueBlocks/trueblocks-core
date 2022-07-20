// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

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
func GetSpecials(chain string) (specials []types.SimpleNamedBlock, err error) {
	specialsPath := config.GetPathToChainConfig(chain) + "specials.csv"
	_, err = os.Stat(specialsPath)
	if err != nil {
		// It's okay if there are no specials for a certain chain
		if chain == "mainnet" {
			// should not happen, but we want to know if it does
			logger.Log(logger.Info, "No special block file found for chain", chain)
		}
		return specials, nil
	}

	file, err := os.Open(specialsPath)
	if err != nil {
		return
	}
	reader := csv.NewReader(file)
	reader.FieldsPerRecord = 4

	for {
		record, err1 := reader.Read()
		if err1 == io.EOF {
			break
		}
		if err1 != nil {
			return specials, err1
		}
		if len(record) == 4 {
			if bn, err := strconv.ParseUint(record[0], 10, 64); err == nil {
				if ts, err := strconv.ParseUint(record[2], 10, 64); err == nil {
					specials = append(specials, types.SimpleNamedBlock{
						BlockNumber: bn,
						Name:        record[1],
						TimeStamp:   ts,
						Date:        record[3],
					})
				}
			}
		}
	}

	if len(specials) == 0 {
		err = errors.New("found no special blocks")
	}

	return
}

// IsSpecialBlock returns true if the given chain-specific name is a special block
func IsSpecialBlock(chain, needle string) bool {
	_, err := FromNameToBn(chain, needle)
	return err == nil
}
