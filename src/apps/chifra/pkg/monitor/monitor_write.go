package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"io"
	"log"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// WriteMonHeader reads the monitor's header
func (mon *Monitor) WriteMonHeader(deleted bool, lastScanned uint32) (err error) {
	f, err := os.OpenFile(mon.Path(), os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		return
	}
	defer f.Close()

	mon.Deleted = deleted
	if lastScanned > mon.LastScanned {
		mon.LastScanned = lastScanned
	}

	f.Seek(0, io.SeekStart)
	err = binary.Write(f, binary.LittleEndian, mon.Header)
	return
}

// WriteAppendApps appends appearances to the end of the file, updates the header with
// lastScanned (if later) and returns the number of records written. Note that we should
// be writing to a temporary file.
func (mon *Monitor) WriteAppendApps(lastScanned uint32, apps *[]index.AppearanceRecord) error {
	if !mon.Staged {
		log.Fatal("Trying to write to a non-staged file. Should not happen.")

	} else if mon == nil {
		log.Fatal("Trying to write from a nil monitor. Should not happen.")
	}

	err := mon.WriteMonHeader(mon.Deleted, lastScanned)
	if err != nil {
		return err
	}

	if apps != nil {
		if len(*apps) > 0 {
			_, err := mon.WriteAppearances(*apps, os.O_WRONLY|os.O_APPEND)
			if err != nil {
				return err
			}
		}
	}

	return nil
}

// WriteAppearances writes appearances to a Monitor
func (mon *Monitor) WriteAppearances(apps []index.AppearanceRecord, mode int) (int, error) {

	path := mon.Path()
	f, err := os.OpenFile(path, mode, 0644)
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
	return int(mon.Count()), nil
}
