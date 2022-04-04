package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// Header is the header of the Monitor file. Note that it's the same width as an index.AppearanceRecord
type Header struct {
	Magic       uint16 `json:"-"`
	Unused      bool   `json:"-"`
	Deleted     bool   `json:"deleted,omitempty"`
	LastScanned uint32 `json:"lastScanned,omitempty"`
}

// Monitor carries information about a Monitor file and its header
type Monitor struct {
	Address  common.Address `json:"address"`
	Count    uint32         `json:"count"`
	FileSize uint32         `json:"fileSize"`
	Staged   bool           `json:"-"`
	Chain    string         `json:"-"`
	ReadFp   *os.File       `json:"-"`
	TestMode bool           `json:"-"`
	Header
}

const (
	HeaderRecordWidth = index.AppRecordWidth
	Ext               = ".mon.bin"
)

// NewMonitor returns a Monitor (but has not yet read in the AppearanceRecords). If 'create' is
// sent, create the Monitor if it does not already exist
func NewMonitor(chain, addr string, create, testMode bool) Monitor {
	mon := new(Monitor)
	mon.Header = Header{Magic: file.SmallMagicNumber}
	mon.Address = common.HexToAddress(addr)
	mon.Chain = chain
	mon.TestMode = testMode
	mon.Reload(create)
	return *mon
}

// NewStagedMonitor returns a Monitor whose path is in the 'staging' folder
func NewStagedMonitor(chain, addr string, testMode bool) Monitor {
	mon := new(Monitor)
	mon.Header = Header{Magic: file.SmallMagicNumber}
	mon.Address = common.HexToAddress(addr)
	mon.Chain = chain
	mon.Staged = true
	mon.TestMode = testMode
	mon.Reload(true)
	return *mon
}

// String implements the Stringer interface
func (mon Monitor) String() string {
	if mon.Deleted {
		return fmt.Sprintf("%s\t%d\t%d\t%t", hexutil.Encode(mon.Address.Bytes()), mon.Count, mon.FileSize, mon.Deleted)
	}
	return fmt.Sprintf("%s\t%d\t%d", hexutil.Encode(mon.Address.Bytes()), mon.Count, mon.FileSize)
}

// ToJSON returns a JSON object from a Monitor
func (mon Monitor) ToJSON() string {
	bytes, err := json.Marshal(mon)
	if err != nil {
		return ""
	}
	return string(bytes)
}

// Path returns the path to the Monitor file
func (mon *Monitor) Path() (path string) {
	if mon.Staged {
		path = config.GetPathToCache(mon.Chain) + "monitors/staging/" + strings.ToLower(mon.Address.Hex()) + Ext
	} else {
		path = config.GetPathToCache(mon.Chain) + "monitors/" + strings.ToLower(mon.Address.Hex()) + Ext
	}
	if mon.TestMode {
		path = strings.Replace(path, config.GetPathToCache(mon.Chain), config.GetPathToChainConfig(mon.Chain)+"mocked/", -1)
	}
	return
}

// Reload loads information about the monitor such as the file's size and record count
func (mon *Monitor) Reload(create bool) (uint32, error) {
	if create && !file.FileExists(mon.Path()) {
		// Make sure the file exists since we've been told to monitor it
		err := mon.WriteHeader(false, 0)
		if err != nil {
			return 0, err
		}
	}
	mon.FileSize = uint32(file.FileSize(mon.Path()))
	mon.Count = uint32(file.FileSize(mon.Path())/HeaderRecordWidth) - 1
	return mon.Count, nil
}

// GetAddrStr returns the Monitor's address as a string
func (mon *Monitor) GetAddrStr() string {
	return strings.ToLower(mon.Address.Hex())
}

// Close closes an open Monitor if it's open, does nothing otherwise
func (mon *Monitor) Close() {
	if mon.ReadFp != nil {
		mon.ReadFp.Close()
		mon.ReadFp = nil
	}
}

// ReadHeader reads the monitor's header
func (mon *Monitor) ReadHeader() (err error) {
	if mon.ReadFp == nil {
		mon.ReadFp, err = os.OpenFile(mon.Path(), os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}
	err = binary.Read(mon.ReadFp, binary.LittleEndian, &mon.Header)
	if err != nil {
		return
	}
	return
}

// WriteHeader reads the monitor's header
func (mon *Monitor) WriteHeader(deleted bool, lastScanned uint32) (err error) {
	f, err := os.OpenFile(mon.Path(), os.O_WRONLY|os.O_CREATE, 0644)
	if err != nil {
		return
	}
	defer f.Close()

	mon.Deleted = deleted
	// we want to store nextBlockToVisit (one more than the last block scanned)
	if lastScanned > mon.LastScanned {
		mon.LastScanned = lastScanned
	}

	f.Seek(0, io.SeekStart)
	err = binary.Write(f, binary.LittleEndian, mon.Header)
	return
}

// ReadApp returns the appearance at the one-based index.
func (mon *Monitor) ReadApp(idx uint32, app *index.AppearanceRecord) (err error) {
	if idx == 0 || idx > mon.Count {
		// one-based index for ease in caller code
		err = errors.New(fmt.Sprintf("index out of range in ReadApp[%d]", idx))
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

// ReadApps returns appearances starting at the first appearance in the file.
func (mon *Monitor) ReadApps(apps *[]index.AppearanceRecord) (err error) {
	if mon.ReadFp == nil {
		path := mon.Path()
		mon.ReadFp, err = os.OpenFile(path, os.O_RDONLY, 0644)
		if err != nil {
			return
		}
	}

	// Seek past the header to get to the first record
	_, err = mon.ReadFp.Seek(HeaderRecordWidth, io.SeekStart)
	if err != nil {
		return
	}

	err = binary.Read(mon.ReadFp, binary.LittleEndian, apps)
	if err != nil {
		return
	}
	return
}

// WriteApps writes appearances to a Monitor and updates lastScanned
func (mon *Monitor) WriteApps(apps []index.AppearanceRecord, lastScanned uint32) (count int, err error) {
	// close the reader if it's open
	mon.Close()

	// Order matters
	mon.WriteHeader(mon.Deleted, lastScanned+1)

	mode := os.O_WRONLY | os.O_CREATE
	if file.FileExists(mon.Path()) {
		// log.Println("Appending to existing monitor", mon.GetAddrStr())
		mode = os.O_WRONLY | os.O_APPEND
	}

	path := mon.Path()
	f, err := os.OpenFile(path, mode, 0644)
	if err != nil {
		return
	}

	f.Seek(HeaderRecordWidth, io.SeekStart)

	b := make([]byte, 4, 4)
	for _, app := range apps {
		binary.LittleEndian.PutUint32(b, app.BlockNumber)
		_, err = f.Write(b)
		if err != nil {
			f.Close()
			return
		}
		binary.LittleEndian.PutUint32(b, app.TransactionId)
		_, err = f.Write(b)
		if err != nil {
			f.Close()
			return
		}
	}

	f.Close() // do not defer this, we need to close it so the fileSize is right
	mon.Reload(false /* create */)
	count = int(mon.Count)

	return
}

// IsMonitor returns true if the monitor file exists
func (mon *Monitor) IsMonitor() bool {
	exists := file.FileExists(mon.Path())
	if !exists {
		// TODO: BOGUS
		oldAcctFile := strings.Replace(mon.Path(), Ext, ".acct.bin", -1)
		exists = file.FileExists(oldAcctFile)
	}
	return exists
}

// Delete marks the file's delete flag, but does not physically remove the file
func (mon *Monitor) Delete() (prev bool) {
	prev = mon.Deleted
	mon.WriteHeader(true, mon.LastScanned)
	// TODO: BOGUS
	oldDelFile := strings.Replace(mon.Path(), Ext, ".acct.bin", -1) + ".deleted"
	file.Touch(oldDelFile)
	return
}

// IsDeleted returns true if the monitor has been deleted but not removed
func (mon *Monitor) IsDeleted() bool {
	mon.ReadHeader()
	return mon.Header.Deleted
}

// UnDelete unmarks the file's delete flag
func (mon *Monitor) UnDelete() (prev bool) {
	prev = mon.Deleted
	mon.WriteHeader(false, mon.LastScanned)
	// TODO: BOGUS
	oldDelFile := strings.Replace(mon.Path(), Ext, ".acct.bin", -1) + ".deleted"
	file.Remove(oldDelFile)
	return
}

// ToggleDelete toggles the file's delete flag
func (mon *Monitor) ToggleDelete() (prev bool) {
	prev = mon.Deleted
	if mon.Deleted {
		mon.UnDelete()
	} else {
		mon.Delete()
	}
	return
}

// Remove removes a previously deleted file, does nothing if the file is not deleted
func (mon *Monitor) Remove() (bool, error) {
	// TODO: BOGUS
	oldAcctFile := strings.Replace(mon.Path(), Ext, ".acct.bin", -1)
	oldCntFile := strings.Replace(mon.Path(), Ext, ".last.txt", -1)
	oldDelFile := strings.Replace(mon.Path(), Ext, ".acct.bin", -1) + ".deleted"
	exists := file.FileExists(oldDelFile)
	if !mon.Deleted && !exists {
		return false, errors.New("cannot remove a file that has not been deleted")
	}
	file.Remove(oldAcctFile)
	file.Remove(oldCntFile)
	file.Remove(oldDelFile)
	file.Remove(mon.Path())
	return !file.FileExists(mon.Path()), nil
}

// TODO: This should be non-interuptable
// MoveToProduction moves a previously staged monitor to the monitors folder.
func (mon *Monitor) MoveToProduction() error {
	if !mon.Staged {
		return errors.New("trying to move monitor that is not staged")
	}
	oldpath := mon.Path()
	mon.Staged = false
	return os.Rename(oldpath, mon.Path())
}

func AddressFromMonitorPath(path string) (string, error) {
	// TODO: BOGUS
	if !strings.Contains(path, ".") && !strings.HasSuffix(path, ".acct.bin") && !strings.HasSuffix(path, Ext) {
		return "", errors.New("path does not contain an address")
	}
	_, fileName := filepath.Split(path)
	parts := strings.Split(fileName, ".")
	return strings.ToLower(parts[0]), nil
}
