package tslib

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type TimestampRecord struct {
	Bn uint32 `json:"bn"`
	Ts uint32 `json:"ts"`
}

type TimestampDatabase struct {
	loaded bool
	count  uint64
	memory []TimestampRecord
}

var perChainTimestamps = map[string]TimestampDatabase{}

// NTimestamps returns the number of records in the timestamp file
func NTimestamps(chain string) (base.Blknum, error) {
	if perChainTimestamps[chain].count > 0 {
		return base.Blknum(perChainTimestamps[chain].count), nil
	}

	tsPath := config.PathToTimestamps(chain)

	fileStat, err := os.Stat(tsPath)
	if err != nil {
		return 0, err
	}

	perChainTimestamps[chain] = TimestampDatabase{
		loaded: perChainTimestamps[chain].loaded,
		count:  uint64(fileStat.Size()) / 8,
		memory: perChainTimestamps[chain].memory,
	}
	return base.Blknum(perChainTimestamps[chain].count), nil
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

	tsPath := config.PathToTimestamps(chain)
	tsFile, err := os.OpenFile(tsPath, os.O_RDONLY, 0)
	if err != nil {
		return err
	}
	defer tsFile.Close()

	memory := make([]TimestampRecord, cnt)
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

var ErrInTheFuture = errors.New("timestamp in the future")

// FromTs is a local function that returns a Timestamp record given a Unix timestamp. It
// loads the timestamp file into memory if it isn't already. If the timestamp requested
// is past the end of the timestamp file, it estimates the block number and returns and error
func FromTs(chain string, ts base.Timestamp) (*TimestampRecord, error) {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return &TimestampRecord{}, err
	}

	err = loadTimestamps(chain)
	if err != nil {
		return &TimestampRecord{}, err
	}

	if ts > base.Timestamp(perChainTimestamps[chain].memory[cnt-1].Ts) {
		last := perChainTimestamps[chain].memory[cnt-1]
		secs := ts - base.Timestamp(last.Ts)
		blks := uint32(float64(secs) / 13.3)
		last.Bn = last.Bn + blks
		last.Ts = uint32(ts)
		return &last, ErrInTheFuture
	}

	// Go docs: Search uses binary search to find and return the smallest index i in [0, n) at which f(i) is true,
	index := sort.Search(int(cnt), func(i int) bool {
		d := perChainTimestamps[chain].memory[i]
		v := base.Timestamp(d.Ts)
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

func ClearCache(chain string) {
	perChainTimestamps[chain] = TimestampDatabase{
		loaded: false,
		count:  0,
		memory: nil,
	}
}

// FromBn is a local function that returns a Timestamp record given a blockNum. It
// loads the timestamp file into memory if it isn't already loaded
func FromBn(chain string, bn base.Blknum) (*TimestampRecord, error) {
	cnt, err := NTimestamps(chain)
	if err != nil {
		return &TimestampRecord{}, err
	}

	if bn > base.Blknum(cnt) {
		return &TimestampRecord{}, errors.New("invalid block number " + fmt.Sprintf("%d of %d", bn, cnt))
	}

	err = loadTimestamps(chain)
	if err != nil {
		return &TimestampRecord{}, err
	}

	return &perChainTimestamps[chain].memory[bn], nil
}
