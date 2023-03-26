package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// Header is the header of the Monitor file. Note that it's the same width as an index.AppearanceRecord
// therefor one should not change its size
type Header struct {
	Magic       uint16 `json:"-"`
	Unused      bool   `json:"-"`
	Deleted     bool   `json:"deleted,omitempty"`
	LastScanned uint32 `json:"lastScanned,omitempty"`
}

// Monitor carries information about a Monitor file and its header
type Monitor struct {
	Address base.Address `json:"address"`
	Staged  bool         `json:"-"`
	Chain   string       `json:"-"`
	ReadFp  *os.File     `json:"-"`
	Header
}

const (
	Ext = ".mon.bin"
)

// NewMonitor returns a Monitor (but has not yet read in the AppearanceRecords). If 'create' is
// sent, create the Monitor if it does not already exist
func NewMonitor(chain, addr string, create bool) Monitor {
	mon := new(Monitor)
	mon.Header = Header{Magic: file.SmallMagicNumber}
	mon.Address = base.HexToAddress(addr)
	mon.Chain = chain
	_, err := mon.Reload(create)
	if err != nil {
		logger.Error(err)
	}
	return *mon
}

// NewStagedMonitor returns a Monitor whose path is in the 'staging' folder
func NewStagedMonitor(chain, addr string) (Monitor, error) {
	mon := Monitor{
		Header:  Header{Magic: file.SmallMagicNumber},
		Address: base.HexToAddress(addr),
		Chain:   chain,
	}

	// Note, we are not yet staged, so Path returns the production folder
	prodPath := mon.Path()
	mon.Staged = true
	stagedPath := mon.Path()

	// either copy the existing monitor or create a new one
	if file.FileExists(prodPath) {
		_, err := file.Copy(stagedPath, prodPath)
		if err != nil {
			return mon, err
		}
	} else {
		err := mon.WriteMonHeader(false, 0)
		if err != nil {
			return mon, err
		}
	}
	return mon, nil
}

// TODO: Most other Stringer interfaces produce JSON data. Can we switch the polarity of this...
// String implements the Stringer interface
func (mon Monitor) String() string {
	if mon.Deleted {
		return fmt.Sprintf("%s\t%d\t%d\t%d\t%t", hexutil.Encode(mon.Address.Bytes()), mon.Count(), file.FileSize(mon.Path()), mon.LastScanned, mon.Deleted)
	}
	return fmt.Sprintf("%s\t%d\t%d\t%d", hexutil.Encode(mon.Address.Bytes()), mon.Count(), file.FileSize(mon.Path()), mon.LastScanned)
}

// TODO: ...and this - making this the String and the above ToTxt?
// ToJSON returns a JSON object from a Monitor
func (mon Monitor) ToJSON() string {
	sm := types.SimpleMonitor{
		Address:     mon.GetAddrStr(),
		NRecords:    int(mon.Count()),
		FileSize:    file.FileSize(mon.Path()),
		LastScanned: mon.Header.LastScanned,
	}
	bytes, _ := json.MarshalIndent(sm, "", "  ")
	return string(bytes)
}

// Path returns the path to the Monitor file
func (mon *Monitor) Path() (path string) {
	if mon.Staged {
		path = config.GetPathToCache(mon.Chain) + "monitors/staging/" + mon.GetAddrStr() + Ext
	} else {
		path = config.GetPathToCache(mon.Chain) + "monitors/" + mon.GetAddrStr() + Ext
	}
	return
}

// Reload loads information about the monitor such as the file's size and record count
func (mon *Monitor) Reload(create bool) (uint32, error) {
	if create && !file.FileExists(mon.Path()) {
		// Make sure the file exists since we've been told to monitor it
		err := mon.WriteMonHeader(false, 0)
		if err != nil {
			return 0, err
		}
	}
	return mon.Count(), nil
}

func (mon *Monitor) Count() uint32 {
	if file.FileSize(mon.Path()) == 0 {
		return 0
	}
	s := uint32(file.FileSize(mon.Path()))
	w := uint32(index.AppRecordWidth)
	n := uint32(s / w)
	return n - 1
}

// GetAddrStr returns the Monitor's address as a string
func (mon *Monitor) GetAddrStr() string {
	return hexutil.Encode(mon.Address.Bytes())
}

// IsOpen returns true if the underlying monitor file is opened.
func (mon *Monitor) IsOpen() bool {
	return mon.ReadFp != nil
}

// Close closes an open Monitor if it's open, does nothing otherwise
func (mon *Monitor) Close() {
	if mon.ReadFp != nil {
		mon.ReadFp.Close()
		mon.ReadFp = nil
	}
}

// IsDeleted returns true if the monitor has been deleted but not removed
func (mon *Monitor) IsDeleted() bool {
	mon.ReadMonitorHeader()
	return mon.Header.Deleted
}

// Delete marks the file's delete flag, but does not physically remove the file
func (mon *Monitor) Delete() (prev bool) {
	prev = mon.Deleted
	mon.WriteMonHeader(true, mon.LastScanned)
	mon.Deleted = true
	return
}

// UnDelete unmarks the file's delete flag
func (mon *Monitor) UnDelete() (prev bool) {
	prev = mon.Deleted
	mon.WriteMonHeader(false, mon.LastScanned)
	mon.Deleted = false
	return
}

// Remove removes a previously deleted file, does nothing if the file is not deleted
func (mon *Monitor) Remove() (bool, error) {
	if !mon.IsDeleted() {
		return false, errors.New("cannot remove a monitor that is not deleted")
	}
	file.Remove(mon.Path())
	return !file.FileExists(mon.Path()), nil
}

func addressFromPath(path string) (string, error) {
	_, fileName := filepath.Split(path)
	if len(fileName) == 0 || !strings.HasPrefix(fileName, "0x") || !strings.HasSuffix(fileName, ".mon.bin") {
		return "", errors.New("path does is not a valid monitor filename")
	}
	parts := strings.Split(fileName, ".")
	return strings.ToLower(parts[0]), nil
}

// SentinalAddr is a marker to signify the end of the monitor list produced by ListMonitors
var SentinalAddr = base.HexToAddress("0xdeaddeaddeaddeaddeaddeaddeaddeaddeaddead")

// ListMonitors puts a list of Monitors into the monitorChannel. The list of monitors is built from
// a file called addresses.tsv in the current folder or, if not present, from existing monitors
func ListMonitors(chain, folder string, monitorChan chan<- Monitor) {
	defer func() {
		monitorChan <- Monitor{Address: SentinalAddr}
	}()

	pwd, _ := os.Getwd()
	path := filepath.Join(pwd, "addresses.tsv")
	info, err := os.Stat(path)
	if err == nil {
		logger.Info("Reading address list from", path)
		lines := file.AsciiFileToLines(info.Name())
		logger.Info("Found", len(lines), "unique addresses in ./addresses.tsv")
		addrMap := make(map[string]bool)
		for _, line := range lines {
			if !strings.HasPrefix(line, "#") {
				parts := strings.Split(line, "\t")
				if len(parts) > 0 {
					addr := strings.Trim(parts[0], " ")
					if !addrMap[addr] && validate.IsValidAddress(addr) && !validate.IsZeroAddress(addr) {
						monitorChan <- NewMonitor(chain, addr, true /* create */)
					}
					addrMap[addr] = true
				} else {
					logger.Warn("Invalid line in file", info.Name())
				}
			}
		}
		return
	}

	logger.Info("Building address list from current monitors")
	path = config.GetPathToCache(chain) + folder
	filepath.Walk(path, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			addr, _ := addressFromPath(path)
			if len(addr) > 0 {
				monitorChan <- NewMonitor(chain, addr, true /* create */)
			}
		}
		return nil
	})
}

var monitorMutex sync.Mutex

// MoveToProduction moves a previously staged monitor to the monitors folder.
func (mon *Monitor) MoveToProduction() error {
	if !mon.Staged {
		return fmt.Errorf("trying to move monitor that is not staged")
	}

	before, after, err := mon.RemoveDups()
	if err != nil {
		return err
	}

	if before != after {
		msg := fmt.Sprintf("%s %d duplicates removed.", mon.GetAddrStr(), (before - after))
		logger.Warn(msg)
	}

	oldPath := mon.Path()
	mon.Staged = false
	monitorMutex.Lock()
	err = os.Rename(oldPath, mon.Path())
	monitorMutex.Unlock()

	return err
}

func GetMonitorMap(chain string) map[base.Address]*Monitor {
	monitorChan := make(chan Monitor)

	go ListMonitors(chain, "monitors", monitorChan)

	monMap := make(map[base.Address]*Monitor)
	for mon := range monitorChan {
		mon := mon
		switch mon.Address {
		case SentinalAddr:
			close(monitorChan)
		default:
			monMap[mon.Address] = &mon
		}
	}

	return monMap
}
