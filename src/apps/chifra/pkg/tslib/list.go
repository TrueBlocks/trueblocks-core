// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

import (
	"encoding/csv"
	"errors"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetSpecials returns a chain-specific list of special block names and numbers
func GetSpecials(chain string) (specials []types.SimpleNamedBlock, err error) {
	specialsPath := config.GetPathToChainConfig(chain) + "specials.csv"
	_, err = os.Stat(specialsPath)
	if err != nil {
		// It's okay if there are no specials for a certain chain
		if chain == "mainnet" {
			// should not happen, but we want to know if it does
			logger.Info("No special block file found for chain", chain)
		}
		return specials, nil
	}

	file, err := os.OpenFile(specialsPath, os.O_RDONLY, 0)
	if err != nil {
		return
	}
	reader := csv.NewReader(file)
	//	component,block/epoch,name,timestamp,date,description
	reader.FieldsPerRecord = 6

	for {
		record, err1 := reader.Read()
		if err1 == io.EOF {
			break
		}
		if err1 != nil {
			return specials, err1
		}
		if len(record) == 6 {
			bn := utils.MustParseUint(record[1])
			name := record[2]
			ts := utils.MustParseInt(record[3])
			if bn == 0 && name != "frontier" {
				continue
			}
			s := types.SimpleNamedBlock{
				BlockNumber: bn,
				Name:        name,
				Timestamp:   ts,
				Component:   record[0],
				Description: record[5],
			}
			specials = append(specials, s)
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
