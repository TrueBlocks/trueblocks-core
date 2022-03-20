package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"bufio"
	"encoding/binary"
	"errors"
	"fmt"
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
	Path     string         `json:"path,omitempty"`
	Count    uint32         `json:"count"`
	FileSize uint32         `json:"fileSize"`
	Bytes    []byte         `json:"bytes,omitempty"`
}

const itemSizeInBytes = 4
const recordSizeInBytes = 8 // (itemSizeInBytes * 2)

func (mon Monitor) String() string {
	return fmt.Sprintf("%s\t%d\t%d", mon.Address, mon.Count, mon.FileSize)
}

func NewMonitor(chain, addr string) Monitor {
	mon := new(Monitor)
	mon.Address = common.HexToAddress(addr)
	mon.Resolve(chain)
	return *mon
}

func (mon *Monitor) GetAddrStr() string {
	return strings.ToLower(mon.Address.Hex())
}

func (mon *Monitor) Resolve(chain string) (uint32, error) {
	mon.Path, _ = addr_2_Fn(chain, mon.Address)
	if !file.FileExists(mon.Path) {
		// Make sure the file exists since we've been told to monitor it
		file.Touch(mon.Path)
	}
	mon.FileSize = uint32(file.FileSize(mon.Path))
	mon.Count = uint32(file.FileSize(mon.Path) / recordSizeInBytes)
	return mon.Count, nil
}

func addr_2_Fn(chain string, addr common.Address) (string, error) {
	return config.GetPathToCache(chain) + "monitors/" + addr.Hex() + ".acct.bin", nil
}

func (mon *Monitor) GetTxAt(index uint32) (app index.AppearanceRecord, err error) {
	if index == 0 {
		return
	}

	err = mon.ReadBytes()
	if err != nil {
		return
	}

	// Caller wants record 1, which stands at index 0
	index = index - 1
	byteIndex := index * recordSizeInBytes
	// fmt.Println("index: ", index, "byteIndex:", byteIndex)

	// If the end of the record is past the end of the file, we have a problem...
	if index >= mon.Count {
		msg := fmt.Sprintf("index out of range in GetTxAt[%d]", index)
		return app, errors.New(msg)
	}

	f, l := int(byteIndex), int(byteIndex+itemSizeInBytes)
	app.BlockNumber = binary.LittleEndian.Uint32(mon.Bytes[f:l])

	f, l = int(byteIndex+itemSizeInBytes), int(byteIndex+recordSizeInBytes)
	app.TransactionId = binary.LittleEndian.Uint32(mon.Bytes[f:l])

	return
}

func (mon *Monitor) ReadBytes() error {
	// If we already have the bytes, don't re-read them. In order to re-freshen, empty first
	if len(mon.Bytes) > 0 {
		return nil
	}

	file, err := os.Open(mon.Path)
	if err != nil {
		return err
	}
	defer file.Close()

	stats, err := file.Stat()
	if err != nil {
		return err
	}

	mon.FileSize = uint32(stats.Size())
	mon.Bytes = make([]byte, mon.FileSize+1)

	bufr := bufio.NewReader(file)
	_, err = bufr.Read(mon.Bytes)
	if err != nil {
		return err
	}

	return nil
}
