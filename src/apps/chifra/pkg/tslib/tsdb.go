package tslibPkg

import (
	"encoding/binary"
	"errors"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type Timestamp struct {
	Bn uint32 `json:"bn"`
	Ts uint32 `json:"ts"`
}

// TODO: BOGUS -- this will help us make timestamps per-chain
type TimestampDatabase struct {
	loaded bool
	count  uint64
	memory []Timestamp
}

var perChainTimestamps = map[string]TimestampDatabase{}

var loaded = false
var memory []Timestamp
var recordCount uint64 = 0

// nRecords returns the number of records in the timestamp file
func nRecords(chain string) (uint64, error) {
	if recordCount > 0 {
		return recordCount, nil
	}

	tsPath := config.GetPathToIndex(chain) + "ts.bin"

	fileStat, err := os.Stat(tsPath)
	if err != nil {
		return 0, err
	}
	recordCount = uint64(fileStat.Size()) / 8
	return recordCount, nil
}

// loadTimestamps loads the timestamp data from the file into memory. If the timestamps are already loaded, we short circiut.
func loadTimestamps(chain string) error {
	if loaded {
		return nil
	}

	cnt, err := nRecords(chain)
	if err != nil {
		return err
	}

	tsPath := config.GetPathToIndex(chain) + "ts.bin"

	tsFile, err := os.Open(tsPath)
	if err != nil {
		return err
	}
	defer tsFile.Close()

	memory = make([]Timestamp, cnt)
	err = binary.Read(tsFile, binary.LittleEndian, memory)
	if err != nil {
		return err
	}
	loaded = true
	return nil
}

// fromTs is a local function that returns a Timestamp record given a Unix timestamp. It
// loads the timestamp file into memory if it isn't already
func fromTs(chain string, ts uint64) (*Timestamp, error) {
	cnt, err := nRecords(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	err = loadTimestamps(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	// Go docs: Search uses binary search to find and return the smallest index i in [0, n) at which f(i) is true,
	index := sort.Search(int(cnt), func(i int) bool {
		d := memory[i]
		v := uint64(d.Ts)
		return v >= ts
	})

	if uint64(memory[index].Ts) != ts {
		return &Timestamp{}, errors.New("timestamp not found")
	}

	return &memory[index], nil
}

// fromTs is a local function that returns a Timestamp record given a blockNum. It
// loads the timestamp file into memory if it isn't already
func fromBn(chain string, bn uint64) (*Timestamp, error) {
	cnt, err := nRecords(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	if bn > cnt {
		return &Timestamp{}, errors.New("invalid block number")
	}

	err = loadTimestamps(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	return &memory[bn], nil
}
