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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
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

// ReadAppearances returns appearances starting at the first appearance in the file. The call
// will read as many records as are available in the array. The file remains opened.
func (mon *Monitor) ReadAppearances(apps *[]index.AppearanceRecord) (err error) {
	if len(*apps) > mon.Count() {
		err = fmt.Errorf("array is larger than the size of the file in ReadAppearances (%d,%d)", len(*apps), mon.Count())
		return
	}

	if mon.ReadFp == nil {
		path := mon.Path()
		mon.ReadFp, err = os.OpenFile(path, os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}

	// Seek past the header to get to the first record
	_, err = mon.ReadFp.Seek(index.AppRecordWidth, io.SeekStart)
	if err != nil {
		return
	}

	err = binary.Read(mon.ReadFp, binary.LittleEndian, apps)
	if err != nil {
		return
	}

	return
}

type AppearanceSort int

const (
	NotSorted AppearanceSort = iota
	Sorted
	Reversed
)

func Sort(apps []index.AppearanceRecord, sortBy AppearanceSort) {
	if sortBy == Sorted || sortBy == Reversed {
		sort.Slice(apps, func(i, j int) bool {
			si := (uint64(apps[i].BlockNumber) << 32) + uint64(apps[i].TransactionId)
			sj := (uint64(apps[j].BlockNumber) << 32) + uint64(apps[j].TransactionId)
			if sortBy == Reversed {
				return sj < si
			} else {
				return si < sj
			}
		})
	}
}

func (mon *Monitor) ReadAppearancesToSlice(sortBy AppearanceSort) (apps []index.AppearanceRecord, cnt int, err error) {
	if mon.Count() == 0 {
		return nil, 0, nil
	}

	apps = make([]index.AppearanceRecord, mon.Count())
	if err := mon.ReadAppearances(&apps); err != nil {
		return nil, 0, err
	} else if len(apps) == 0 {
		return nil, 0, nil
	}

	Sort(apps, sortBy)
	mon.Close()

	return apps, len(apps), nil
}
