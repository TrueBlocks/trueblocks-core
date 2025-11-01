package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
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
func (mon *Monitor) ReadAppearanceAt(idx int64, app *types.AppRecord) (err error) {
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
	err = binary.Read(mon.ReadFp, binary.LittleEndian, &app.TransactionIndex)
	return
}
