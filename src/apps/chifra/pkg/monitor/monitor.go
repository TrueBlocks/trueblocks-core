package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum/common"
)

type MonitorLight struct {
	Address  common.Address `json:"address"`
	NRecords uint32         `json:"nRecords"`
	FileSize uint32         `json:"fileSize"`
}

func NewMonitorLight(chain, addr string) MonitorLight {
	m := NewMonitor(chain, addr)
	return MonitorLight{Address: m.Address, NRecords: m.Count, FileSize: m.FileSize}
}

type Monitor struct {
	Address  common.Address `json:"address"`
	Count    uint32         `json:"count"`
	FileSize uint32         `json:"fileSize"`
	Path     string
	File     *os.File
}

func (mon Monitor) String() string {
	return fmt.Sprintf("%s\t%d\t%d", mon.Address, mon.Count, mon.FileSize)
}

func NewMonitor(chain, addr string) Monitor {
	mon := new(Monitor)
	mon.Address = common.HexToAddress(addr)
	mon.Reload(chain)
	return *mon
}

func (mon *Monitor) Reload(chain string) (uint32, error) {
	mon.Path, _ = addr_2_Fn(chain, mon.Address)
	if !file.FileExists(mon.Path) {
		// Make sure the file exists since we've been told to monitor it
		file.Touch(mon.Path)
	}
	mon.FileSize = uint32(file.FileSize(mon.Path))
	mon.Count = uint32(file.FileSize(mon.Path) / index.AppRecordWidth)
	return mon.Count, nil
}

func (mon *Monitor) GetAddrStr() string {
	return strings.ToLower(mon.Address.Hex())
}

// Peek returns the appearance at the index - 1. For example, ask for PeekAt(1) to get the
// first record in the file or Peek(Count) to get the last record.
func (mon *Monitor) Peek(idx uint32) (app index.AppearanceRecord, err error) {
	if idx == 0 || idx > mon.Count {
		// one-based index for ease in caller code
		err = errors.New(fmt.Sprintf("index out of range in Peek[%d]", idx))
		return
	}

	if mon.File == nil {
		mon.File, err = os.OpenFile(mon.Path, os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}

	// Caller wants record 1, which stands at location 0, etc.
	byteIndex := int64(idx-1) * index.AppRecordWidth
	_, err = mon.File.Seek(byteIndex, io.SeekStart)
	if err != nil {
		return
	}

	err = binary.Read(mon.File, binary.LittleEndian, &app.BlockNumber)
	if err != nil {
		return
	}
	err = binary.Read(mon.File, binary.LittleEndian, &app.TransactionId)
	return
}

func addr_2_Fn(chain string, addr common.Address) (string, error) {
	return config.GetPathToCache(chain) + "monitors/" + addr.Hex() + ".acct.bin", nil
}

func (mon *Monitor) Append(apps []index.AppearanceRecord) (err error) {
	if mon.File != nil {
		mon.File.Close()
		mon.File = nil
	}

	f, err := os.OpenFile(mon.Path, os.O_WRONLY|os.O_APPEND, 0644)
	if err != nil {
		return
	}
	defer f.Close()

	b := make([]byte, 4)
	for _, app := range apps {
		binary.LittleEndian.PutUint32(b, app.BlockNumber)
		_, err = f.Write(b)
		if err != nil {
			return
		}
		binary.LittleEndian.PutUint32(b, app.BlockNumber)
		_, err = f.Write(b)
		if err != nil {
			return
		}
	}

	return
}
