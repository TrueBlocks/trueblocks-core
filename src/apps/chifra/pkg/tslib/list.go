// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package tslib

import (
	"encoding/csv"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetSpecials returns a chain-specific list of special block names and numbers
func GetSpecials(chain string) (specials []types.NamedBlock, err error) {
	path := filepath.Join(config.MustGetPathToChainConfig(chain), "specials.csv")
	_, err = os.Stat(path)
	if err != nil {
		// It's okay if there are no specials for a certain chain
		if chain == "mainnet" {
			// should not happen, but we want to know if it does
			logger.Info("No special block file found for chain", chain)
		}
		return specials, nil
	}

	if specials, err = readSpecials(path, 6); err != nil {
		if errors.Is(err, csv.ErrFieldCount) {
			if specials, err = readSpecials(path, 4); err != nil {
				return specials, err
			}
		}
	}

	if len(specials) == 0 {
		err = fmt.Errorf("found no special blocks in file %s", path)
	}

	return
}

// IsSpecialBlock returns true if the given chain-specific name is a special block
func IsSpecialBlock(chain, needle string) bool {
	_, err := FromNameToBn(chain, needle)
	return err == nil
}

func readSpecials(path string, nFields int) (specials []types.NamedBlock, err error) {
	file, err := os.OpenFile(path, os.O_RDONLY, 0)
	if err != nil {
		return
	}

	reader := csv.NewReader(file)
	reader.FieldsPerRecord = nFields
	for {
		if record, err := reader.Read(); err == io.EOF {
			break

		} else if err != nil {
			return specials, err

		} else {
			if nFields == len(record) {
				locs := map[string]int{
					"bn":        1,
					"name":      2,
					"ts":        3,
					"component": 0,
					// "date":        4, // skipped since we have timestamp
					"description": 5,
				}
				if nFields == 4 {
					locs = map[string]int{
						"bn":   0,
						"name": 1,
						"ts":   2,
						// "date": 3, // skipped since we have timestamp
					}
				}
				if strings.Contains(record[locs["bn"]], "-") {
					// before block zero
					continue
				}
				s := types.NamedBlock{
					BlockNumber: utils.MustParseUint(record[locs["bn"]]),
					Name:        record[locs["name"]],
					Timestamp:   utils.MustParseInt(record[locs["ts"]]),
				}
				// is this the header?
				if s.BlockNumber == 0 && s.Name == "name" {
					continue
				}

				if nFields == 6 {
					s.Component = record[locs["component"]]
					s.Description = record[locs["description"]]
				} else {
					s.Component = "execution"
				}

				specials = append(specials, s)
			}
		}
	}
	return
}
