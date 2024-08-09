package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"sort"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Header is the header of the Monitor file. Note that it's the same width as an types.AppRecord
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
func NewMonitor(chain string, addr base.Address, create bool) (Monitor, error) {
	mon := new(Monitor)
	mon.Header = Header{Magic: file.SmallMagicNumber}
	mon.Address = addr
	mon.Chain = chain
	if _, err := mon.Reload(create); err != nil {
		logger.Error(err)
	}
	return *mon, nil
}

// NewMonitorStaged returns a Monitor whose path is in the 'staging' folder
func NewMonitorStaged(chain, addr string) (Monitor, error) {
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
		err := mon.WriteMonHeader(false, 0, false /* force */)
		if err != nil {
			return mon, err
		}
	}
	return mon, nil
}

// TODO: Most other Stringer interfaces produce JSON data. Can we switch the polarity of this...

func (mon Monitor) String() string {
	if mon.Deleted {
		return fmt.Sprintf("%s\t%d\t%d\t%d\t%t", mon.Address.Hex(), mon.Count(), file.FileSize(mon.Path()), mon.LastScanned, mon.Deleted)
	}
	return fmt.Sprintf("%s\t%d\t%d\t%d", mon.Address.Hex(), mon.Count(), file.FileSize(mon.Path()), mon.LastScanned)
}

// Path returns the path to the Monitor file
func (mon *Monitor) Path() (path string) {
	if mon.Staged {
		path = filepath.Join(config.PathToCache(mon.Chain), "monitors", "staging", mon.Address.Hex()+Ext)
	} else {
		path = filepath.Join(config.PathToCache(mon.Chain), "monitors", mon.Address.Hex()+Ext)
	}
	return
}

// Reload loads information about the monitor such as the file's size and record count
func (mon *Monitor) Reload(create bool) (int64, error) {
	if create && !file.FileExists(mon.Path()) {
		// Make sure the file exists since we've been told to monitor it
		err := mon.WriteMonHeader(false, 0, false /* force */)
		if err != nil {
			return 0, err
		}
	}
	return mon.Count(), nil
}

func (mon *Monitor) Count() int64 {
	if file.FileSize(mon.Path()) == 0 {
		return 0
	}
	s := file.FileSize(mon.Path())
	w := int64(index.AppRecordWidth)
	n := s / w
	return n - 1
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
	_ = mon.ReadMonitorHeader()
	return mon.Header.Deleted
}

// Delete marks the file's delete flag, but does not physically remove the file
func (mon *Monitor) Delete() (prev bool) {
	prev = mon.Deleted
	_ = mon.WriteMonHeader(true, mon.LastScanned, false /* force */)
	mon.Deleted = true
	return
}

// UnDelete unmarks the file's delete flag
func (mon *Monitor) UnDelete() (prev bool) {
	prev = mon.Deleted
	_ = mon.WriteMonHeader(false, mon.LastScanned, false /* force */)
	mon.Deleted = false
	return
}

// Remove removes a previously deleted file, does nothing if the file is not deleted
func (mon *Monitor) Remove() (bool, error) {
	if !mon.IsDeleted() {
		return false, errors.New("cannot remove a monitor that is not deleted")
	}
	if mon.Staged {
		file.Remove(mon.Path())
		mon.Staged = false
	}
	file.Remove(mon.Path())
	return !file.FileExists(mon.Path()), nil
}

// ListMonitors puts a list of Monitors into the monitorChannel. The list of monitors is built from
// a file called addresses.tsv in the current folder or, if not present, from existing monitors
func ListMonitors(chain, watchList string, monitorChan chan<- Monitor) {
	defer func() {
		monitorChan <- Monitor{Address: base.NotAMonitor}
	}()

	if watchList != "existing" {
		logger.Info("Reading address list from", watchList)
		lines := file.AsciiFileToLines(watchList)
		addrMap := make(map[base.Address]bool)
		for _, line := range lines {
			line = utils.StripComments(line)
			addr := base.HexToAddress(line)
			if !addrMap[addr] && !addr.IsZero() && base.IsValidAddress(addr.Hex()) {
				mon, _ := NewMonitor(chain, addr, true /* create */)
				monitorChan <- mon
			}
			addrMap[addr] = true
		}
		return
	}

	walkFunc := func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() && strings.HasSuffix(path, ".mon.bin") {
			addr, _ := base.AddressFromPath(path, ".mon.bin")
			if !addr.IsZero() {
				mon, _ := NewMonitor(chain, addr, true /* create */)
				monitorChan <- mon
			}
		}
		return nil
	}

	path := filepath.Join(config.PathToCache(chain), "monitors")
	_ = filepath.Walk(path, walkFunc)
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
		msg := fmt.Sprintf("%s %d duplicates removed.", mon.Address.Hex(), (before - after))
		logger.Warn(msg)
	}

	oldPath := mon.Path()
	mon.Staged = false
	monitorMutex.Lock()
	err = os.Rename(oldPath, mon.Path())
	monitorMutex.Unlock()

	return err
}

func GetMonitorMap(chain string) (map[base.Address]*Monitor, []*Monitor) {
	monitorChan := make(chan Monitor)

	go ListMonitors(chain, "existing", monitorChan)

	monMap := make(map[base.Address]*Monitor)
	monArray := []*Monitor{}
	for mon := range monitorChan {
		switch mon.Address {
		case base.NotAMonitor:
			close(monitorChan)
		default:
			monMap[mon.Address] = &mon
			monArray = append(monArray, &mon)
		}
	}

	sort.Slice(monArray, func(i, j int) bool {
		return monArray[i].Address.Hex() < monArray[j].Address.Hex()
	})

	return monMap, monArray
}
