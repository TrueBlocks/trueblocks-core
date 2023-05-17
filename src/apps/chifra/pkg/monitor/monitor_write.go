package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// TODO: Protect against overwriting files on disc
// WriteMonHeader writes the monitor's header
func (mon *Monitor) WriteMonHeader(deleted bool, lastScanned uint32, force bool) (err error) {
	f, err := os.OpenFile(mon.Path(), os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		return
	}
	defer f.Close()

	mon.Deleted = deleted
	if force || lastScanned > mon.LastScanned {
		mon.LastScanned = lastScanned
	}

	f.Seek(0, io.SeekStart)
	err = binary.Write(f, binary.LittleEndian, mon.Header)
	return
}

// WriteAppearancesAppend appends appearances to the end of the file, updates the header with
// lastScanned (if later) and returns the number of records written. Note that we should
// be writing to a temporary file.
func (mon *Monitor) WriteAppearancesAppend(lastScanned uint32, apps *[]index.AppearanceRecord) error {
	if !mon.Staged {
		logger.Fatal("Trying to write to a non-staged file. Should not happen.")

	} else if mon == nil {
		logger.Fatal("Trying to write from a nil monitor. Should not happen.")
	}

	err := mon.WriteMonHeader(mon.Deleted, lastScanned, false /* force */)
	if err != nil {
		return err
	}

	if apps != nil {
		if len(*apps) > 0 {
			_, err := mon.WriteAppearances(*apps, true /* append */)
			if err != nil {
				return err
			}
		}
	}

	return nil
}

// TODO: Protect against overwriting files on disc
// WriteAppearances writes appearances to a Monitor
func (mon *Monitor) WriteAppearances(apps []index.AppearanceRecord, append bool) (uint32, error) {
	var f *os.File
	var err error
	path := mon.Path()
	if append {
		f, err = os.OpenFile(path, os.O_WRONLY|os.O_APPEND, 0644)
	} else {
		f, err = os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0644)
	}
	if err != nil {
		return 0, err
	}

	f.Seek(index.AppRecordWidth, io.SeekStart)

	b := make([]byte, 4)
	for _, app := range apps {
		binary.LittleEndian.PutUint32(b, app.BlockNumber)
		_, err = f.Write(b)
		if err != nil {
			f.Close()
			return 0, err
		}
		binary.LittleEndian.PutUint32(b, app.TransactionId)
		_, err = f.Write(b)
		if err != nil {
			f.Close()
			return 0, err
		}
	}

	f.Close() // do not defer this, we need to close it so the fileSize is right
	mon.Reload(false /* create */)
	return mon.Count(), nil
}
