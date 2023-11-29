package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/filter"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// ReadMonitorHeader reads the monitor's header and returns without closing the file
func (mon *Monitor) ReadMonitorHeader() (err error) {
	if mon.ReadFp == nil {
		mon.ReadFp, err = os.OpenFile(mon.Path(), os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}

	if file.FileSize(mon.Path()) > 0 {
		return binary.Read(mon.ReadFp, binary.LittleEndian, &mon.Header)
	}

	return
}

// ReadAppearanceAt returns the appearance at the one-based index. The file remains open.
func (mon *Monitor) ReadAppearanceAt(idx int64, app *index.AppearanceRecord) (err error) {
	if idx == 0 || idx > mon.Count() {
		// the file contains a header one record wide, so a one-based index eases caller code
		err = fmt.Errorf("index out of range in ReadAppearanceAt[%d]", idx)
		return
	}

	if mon.ReadFp == nil {
		path := mon.Path()
		mon.ReadFp, err = os.OpenFile(path, os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}

	// This index is one based because we have to skip over the header
	byteIndex := int64(idx) * index.AppRecordWidth
	_, err = mon.ReadFp.Seek(byteIndex, io.SeekStart)
	if err != nil {
		return
	}

	err = binary.Read(mon.ReadFp, binary.LittleEndian, &app.BlockNumber)
	if err != nil {
		return
	}
	err = binary.Read(mon.ReadFp, binary.LittleEndian, &app.TransactionId)
	return
}

// AsMap reads all appearances from the monitor and returns a map of the appearances to the given type.
func AsMap[T any](mon *Monitor, filter *filter.AppearanceFilter) (map[types.SimpleAppearance]*T, int, error) {
	if apps, cnt, err := mon.ReadAndFilterAppearances(filter); err != nil {
		return nil, 0, err
	} else if cnt == 0 {
		return nil, 0, nil
	} else {
		m := make(map[types.SimpleAppearance]*T, mon.Count())
		for _, app := range apps {
			if filter.BlocksOnly {
				app = types.SimpleAppearance{
					BlockNumber:      app.BlockNumber,
					TransactionIndex: uint32(^uint32(0)),
				}
				m[app] = new(T)
			} else {
				m[app] = new(T)
			}
		}
		return m, len(m), nil
	}
}

func AsMap2[T any](mon *Monitor, filter *filter.AppearanceFilter) ([]map[types.SimpleAppearance]*T, int, error) {
	if ret, cnt, err := mon.ReadAndFilterAppearances(filter); err != nil {
		return nil, 0, err
	} else if cnt == 0 {
		return nil, 0, nil
	} else {
		sort.Slice(ret, func(i, j int) bool {
			if ret[i].BlockNumber == ret[j].BlockNumber {
				return ret[i].TransactionIndex < ret[j].TransactionIndex
			}
			return ret[i].BlockNumber < ret[j].BlockNumber
		})

		arrayOfMaps := make([]map[types.SimpleAppearance]*T, 0, len(ret))
		curMap := make(map[types.SimpleAppearance]*T)
		for i := 0; i < len(ret); i++ {
			if len(curMap) == 10 {
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
