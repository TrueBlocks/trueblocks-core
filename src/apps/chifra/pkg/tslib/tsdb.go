package tslib

import (
	"encoding/binary"
	"errors"
	"io"
	"os"
	"sort"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

type Timestamp struct {
	Bn uint32 `json:"bn"`
	Ts uint32 `json:"ts"`
}

type TimestampDatabase struct {
	loaded bool
	count  uint64
	memory []Timestamp
}

var perChainTimestamps = map[string]TimestampDatabase{}

// NTimestamps returns the number of records in the timestamp file
func NTimestamps(chain string) (uint64, error) {
	if perChainTimestamps[chain].count > 0 {
		return perChainTimestamps[chain].count, nil
	}

	tsPath := config.GetPathToIndex(chain) + "ts.bin"

	fileStat, err := os.Stat(tsPath)
	if err != nil {
		return 0, err
	}

	perChainTimestamps[chain] = TimestampDatabase{
		loaded: perChainTimestamps[chain].loaded,
		count:  uint64(fileStat.Size()) / 8,
		memory: perChainTimestamps[chain].memory,
	}
	return perChainTimestamps[chain].count, nil
}

// loadTimestamps loads the timestamp data from the file into memory. If the timestamps are already loaded, we short circiut.
func loadTimestamps(chain string) error {
	if perChainTimestamps[chain].loaded {
		return nil
	}

	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}

	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	tsFile, err := os.Open(tsPath)
	if err != nil {
		return err
	}
	defer tsFile.Close()

	memory := make([]Timestamp, cnt)
	err = binary.Read(tsFile, binary.LittleEndian, memory)
	if err != nil {
		return err
	}

	perChainTimestamps[chain] = TimestampDatabase{
		loaded: true,
		count:  perChainTimestamps[chain].count,
		memory: memory,
	}

	return nil
}

// FromTs is a local function that returns a Timestamp record given a Unix timestamp. It
// loads the timestamp file into memory if it isn't already
func FromTs(chain string, ts uint64) (*Timestamp, error) {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	err = loadTimestamps(chain)
	if err != nil {
		return &Timestamp{}, err
	}

	if ts > uint64(perChainTimestamps[chain].memory[cnt-1].Ts) {
		last := perChainTimestamps[chain].memory[cnt-1]
		secs := ts - uint64(last.Ts)
		// TODO: Multi-chain specific
		blks := uint32(float64(secs) / 13.3)
		last.Bn = last.Bn + blks
		last.Ts = uint32(ts)
		return &last, errors.New("timestamp in the future")
	}

	// Go docs: Search uses binary search to find and return the smallest index i in [0, n) at which f(i) is true,
	index := sort.Search(int(cnt), func(i int) bool {
		d := perChainTimestamps[chain].memory[i]
		v := uint64(d.Ts)
		return v > ts
	})

	// ts should not be before the first block
	if index == 0 {
		return nil, errors.New("timestamp is before the first block")
	}

	// The index is one past where we want to be because it's the first block larger
	index--

	return &perChainTimestamps[chain].memory[index], nil
}

// FromBn is a local function that returns a Timestamp record given a blockNum. It
// loads the timestamp file into memory if it isn't already
func FromBn(chain string, bn uint64) (*Timestamp, error) {
	cnt, err := NTimestamps(chain)
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

	return &perChainTimestamps[chain].memory[bn], nil
}

var writeMutex sync.Mutex

func Reset(chain string, maxBn uint64) error {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return err
	}

	// It's already done
	if maxBn >= cnt {
		return nil
	}

	err = loadTimestamps(chain)
	if err != nil {
		return err
	}

	truncated := perChainTimestamps[chain].memory[0:maxBn]

	// TODO: BOGUS - PROTECT WRITING AGAINST CONTROL+C AND DOUBLE ENTRY
	// writeMutex.Lock()
	// trapCh := sigintTrap.Enable(context.WithCancel(context.Background()))

	tempPath := config.GetPathToCache(chain) + "tmp/truncated.bin"
	fp, err := os.OpenFile(tempPath, os.O_WRONLY|os.O_CREATE, 0644)
	defer func() {
		// Clear the cache in even in case of failure, causes a reload at worst
		perChainTimestamps[chain] = TimestampDatabase{
			loaded: false,
			count:  0,
			memory: nil,
		}
		os.Remove(tempPath)
		// sigintTrap.Disable(trapCh)
		// writeMutex.Unlock()
	}()
	if err != nil {
		return err
	}

	fp.Seek(0, io.SeekStart)
	err = binary.Write(fp, binary.LittleEndian, truncated)
	if err != nil {
		fp.Close()
		return err
	}

	// Don't defer this because we want it to be closed before we copy it
	fp.Close()

	// TODO: BOGUS - THIS IS NOT PROTECTIVE OF THE EXISTING FILE
	// TODO: BOGUS - IT SHOULD BE UN-INTERUPTABLE
	// TODO: BOGUS - IT MAY WANT TO MAKE A BACKUP AND RECOVER IF THE COPY OR REMOVAL FAILS
	// TODO: BOGUS - IT SHOULD BE GENERALIZED INSIDE OF COPYFILE
	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	os.Remove(tsPath)
	_, err = file.Copy(tempPath, tsPath)
	if err != nil {
		return err
	}
	return nil
}
