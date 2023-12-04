package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func AsSliceOfMaps2[T any](mon *Monitor, nApps int, filter *filter.AppearanceFilter) ([]map[types.SimpleAppearance]*T, int, error) {
	if ret, cnt, err := mon.ReadAndFilterAppearances2(filter); err != nil {
		return nil, 0, err
	} else if cnt == 0 {
		return nil, 0, nil
	} else {
		sort.Slice(ret, func(i, j int) bool {
			if filter.Reversed {
				i, j = j, i
			}
			if ret[i].BlockNumber == ret[j].BlockNumber {
				return ret[i].TransactionIndex < ret[j].TransactionIndex
			}
			return ret[i].BlockNumber < ret[j].BlockNumber
		})

		arrayOfMaps := make([]map[types.SimpleAppearance]*T, 0, len(ret))
		curMap := make(map[types.SimpleAppearance]*T)
		for i := 0; i < len(ret); i++ {
			if len(curMap) == nApps {
				arrayOfMaps = append(arrayOfMaps, curMap)
				curMap = make(map[types.SimpleAppearance]*T)
			}
			curMap[ret[i]] = nil
		}

		if len(curMap) > 0 {
			arrayOfMaps = append(arrayOfMaps, curMap)
		}

		return arrayOfMaps, len(ret), nil
	}
}
